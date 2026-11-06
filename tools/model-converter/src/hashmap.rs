use std::io::{self, Bytes, Write};
use zerocopy::little_endian::{U32, U64};
use zerocopy::{Immutable, IntoBytes};

const SIZE_MULTIPLIER: f64 = 2.0; // Buckets = elements * SIZE_MULTIPLIER

//////// Convenience wrappers
pub fn build_simple_utf32str_u64_hashmap<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = (String, u64)>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert data to utf32 bytes (little endian)
    let data = data.into_iter().map(|(key, value)| {
        (
            key.chars()
                .map(|c| U32::new(u32::from(c)))
                .collect::<Vec<U32>>()
                .as_bytes()
                .to_vec(),
            value,
        )
    });

    build_simple_bytes_u64_hashmap(data)
}

pub fn build_simple_utf8str_u64_hashmap<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = (String, u64)>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert data to utf8 bytes
    let data = data
        .into_iter()
        .map(|(key, value)| (key.as_bytes().to_vec(), value));

    build_simple_bytes_u64_hashmap(data)
}

pub fn build_bidir_utf32str_u32_map<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = String>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert data to utf32 bytes (little endian)
    let data = data.into_iter().map(|key| {
        key.chars()
            .map(|c| U32::new(u32::from(c)))
            .collect::<Vec<U32>>()
            .as_bytes()
            .to_vec()
    });

    build_bidir_bytes_u32_map(data)
}

pub fn build_bidir_utf8str_u32_map<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = String>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert data to utf8 bytes
    let data = data.into_iter().map(|key| key.as_bytes().to_vec());

    build_bidir_bytes_u32_map(data)
}

//////// Binary representation
#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct SimpleBytesU64HashMapHeader {
    pub n_buckets: U32,
    pub _pad: U32,
}

#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct SimpleBytesU64HashMapBucket {
    // Key pointer
    pub offset: U32,
    pub len: U32,

    // Value
    pub value: U64,
}

/// Build a simple hashmap with a vector of bytes as key and u64 value
pub fn build_simple_bytes_u64_hashmap<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = (Vec<u8>, u64)>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert to iterator
    let data = data.into_iter();

    // Compute number of buckets
    let n_buckets = (data.len() as f64 * SIZE_MULTIPLIER + 1.0) as u32;

    // String arena and buckets
    let mut arena: Vec<u8> = vec![];
    let mut buckets = vec![
        SimpleBytesU64HashMapBucket {
            offset: 0.into(),
            len: 0.into(), // 0 length = empty slot
            value: 0.into(),
        };
        n_buckets as usize
    ];

    // Process all keys
    for (key, value) in data {
        let offset = arena.len();

        // Compute bucket for this key
        let bucket = hash_sdbm(&key) % n_buckets;

        // Linear probing
        for i in 0..n_buckets {
            let probed_bucket = (bucket + i) % n_buckets;

            // Found an empty bucket
            if buckets[probed_bucket as usize].len == 0 {
                buckets[probed_bucket as usize] = SimpleBytesU64HashMapBucket {
                    offset: U32::new(u32::try_from(offset)?),
                    len: U32::new(u32::try_from(key.len())?),
                    value: U64::new(value),
                };

                break;
            }
        }

        // Add string to arena
        arena.extend_from_slice(key.as_bytes());
    }

    // Consturct hashmap header
    let header = SimpleBytesU64HashMapHeader {
        n_buckets: n_buckets.into(),
        _pad: 0.into(),
    };

    // Build full hashmap section
    // Header -> buckets -> Arena
    let mut res = vec![];
    res.extend_from_slice(header.as_bytes());
    res.extend_from_slice(buckets.as_bytes());
    res.extend_from_slice(arena.as_bytes());

    Ok(res)
}

#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct BidirBytesU32MapHeader {
    pub n_items: U32,
    pub n_buckets: U32,
}

#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct BidirBytesU32MapBucket {
    pub offset: U64, // Key offset
    pub len: U32,    // Key length
    pub idx: U32,    // Index (value)
}

/// Build a bidirectional sequential hashmap between byte vectors and uint64_t
/// The values must be sequential.
pub fn build_bidir_bytes_u32_map<T>(data: T) -> anyhow::Result<Vec<u8>>
where
    T: IntoIterator<Item = Vec<u8>>,
    T::IntoIter: ExactSizeIterator,
{
    // Convert to iterator
    let data = data.into_iter();

    // Compute number of buckets
    let n_items = data.len();
    let n_buckets = (n_items as f64 * SIZE_MULTIPLIER + 1.0) as u32;

    // String arena and buckets
    let mut arena: Vec<u8> = vec![];
    let mut buckets = vec![
        BidirBytesU32MapBucket {
            offset: 0.into(),
            len: 0.into(), // 0 length = empty slot
            idx: 0.into(),
        };
        n_buckets as usize
    ];
    let mut labels: Vec<U32> = vec![0.into(); n_items];
    // NOTE: labels are indexed by the index (value), and point to buckets

    // Process all keys
    for (idx, key) in data.enumerate() {
        let offset = arena.len();

        // Compute bucket for this key
        let bucket = hash_sdbm(&key) % n_buckets;

        // Linear probing
        for i in 0..n_buckets {
            let probed_bucket = (bucket + i) % n_buckets;

            // Found an empty bucket
            if buckets[probed_bucket as usize].len == 0 {
                // Fill bucket
                buckets[probed_bucket as usize] = BidirBytesU32MapBucket {
                    offset: U64::new(u64::try_from(offset)?),
                    len: U32::new(u32::try_from(key.len())?),
                    idx: U32::new(u32::try_from(idx)?),
                };

                // Add label to labels array
                labels[idx] = bucket.into();

                break;
            }
        }

        // Add string to arena
        arena.extend_from_slice(key.as_bytes());
    }

    // Consturct hashmap header
    let header = BidirBytesU32MapHeader {
        n_items: u32::try_from(n_items)?.into(),
        n_buckets: n_buckets.into(),
    };

    // Build full hashmap section
    // Header |  Buckets | Labels | Arena
    let mut res = vec![];
    res.extend_from_slice(header.as_bytes());
    res.extend_from_slice(buckets.as_bytes());
    res.extend_from_slice(labels.as_bytes());
    res.extend_from_slice(arena.as_bytes());

    Ok(res)
}

/// Compute byte-per-byte SDBM hash
fn hash_sdbm(data: impl AsRef<[u8]>) -> u32 {
    let mut hash: u32 = 0;
    for byte in data.as_ref() {
        hash = (*byte as u32)
            .wrapping_add(hash << 6)
            .wrapping_add(hash << 16)
            .wrapping_sub(hash);
    }
    hash
}

#[cfg(test)]
mod tests {
    use crate::hashmap::{
        build_bidir_bytes_u32_map, build_bidir_utf8str_u32_map, build_bidir_utf32str_u32_map,
        build_simple_utf8str_u64_hashmap, build_simple_utf32str_u64_hashmap, hash_sdbm,
    };
    use std::fs;

    #[test]
    #[ignore]
    fn test_sdbm_hash_consistency() {
        for _ in 0..100 {
            let mut data = vec![0u8; 1000];
            // Generate random data
            for val in data.iter_mut() {
                *val = rand::random();
            }

            // Hash data
            let hash = hash_sdbm(&data);

            for _ in 0..100 {
                assert_eq!(hash_sdbm(&data), hash);
            }
        }
    }

    #[test]
    #[ignore]
    fn write_test_hashmap_1() {
        let data = (0..1024u16).map(|i| {
            let key = format!("key{}", i);
            let value = i * 2;
            (key, value as u64)
        });
        let hashmap_bytes =
            build_simple_utf32str_u64_hashmap(data).expect("SimpleHashMap build error");

        println!("Built hashmap ({} bytes)", hashmap_bytes.len());

        fs::write("test_hashmap_1.bin", hashmap_bytes).expect("Error writing file");
    }

    #[test]
    #[ignore]
    fn write_test_bidir_map_1() {
        let data = (0..1024u16).map(|i| {
            let key = format!("key{}", i);
            key
        });
        let hashmap_bytes = build_bidir_utf32str_u32_map(data).expect("BidirMap build error");

        println!("Built hashmap ({} bytes)", hashmap_bytes.len());

        fs::write("test_bidir_map_1.bin", hashmap_bytes).expect("Error writing file");
    }

    #[test]
    #[ignore]
    fn print_test_hashmap_2() {
        let data = (0..16).map(|i| {
            let key = format!("key{}", i);
            let value = i * 2;
            (key, value as u64)
        });
        let hashmap_bytes =
            build_simple_utf8str_u64_hashmap(data).expect("SimpleHashMap build error");

        println!("{:?}", hashmap_bytes);
    }

    #[test]
    #[ignore]
    fn print_test_bidir_map_2() {
        let data = (0..16).map(|i| {
            let key = format!("key{}", i);
            key
        });
        let hashmap_bytes = build_bidir_utf8str_u32_map(data).expect("BidirMap build error");

        println!("{:?}", hashmap_bytes);
    }
}
