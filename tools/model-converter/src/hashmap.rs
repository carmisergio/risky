use std::io::{self, Write};
use zerocopy::little_endian::U32;
use zerocopy::{Immutable, IntoBytes};

const SIZE_MULTIPLIER: f32 = 1.5;

const BUCKET_OFFSET_EMPTY: u32 = 0xFFFFFFFF;

pub trait Section {
    /// Get size of the section
    fn size() -> usize;

    /// Write section to a sink
    fn write_to<W: Write>(w: &mut W) -> io::Result<()>;
}

#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct SimpleUnistrU32HashMapHeader {
    pub n_buckets: U32,
    pub _pad: U32,
}

#[derive(Clone, Debug, IntoBytes, Immutable)]
#[repr(C)]
struct SimpleUnistrU32HashMapBucket {
    // Key offset in the arena
    pub offset: U32,

    // Value
    pub value: U32,
}

pub fn build_simple_utf32str_u32_hashmap(data: Vec<(String, u32)>) -> anyhow::Result<Vec<u8>> {
    // Compute number of buckets
    let n_buckets = (data.len() as f32 * SIZE_MULTIPLIER + 1.0) as u32;

    // String arena and buckets
    let mut arena: Vec<u8> = vec![];
    let mut buckets = vec![
        SimpleUnistrU32HashMapBucket {
            value: 0.into(),
            offset: BUCKET_OFFSET_EMPTY.into(),
        };
        n_buckets as usize
    ];

    // Process all keys
    for (key, value) in data {
        let offset = arena.len();

        // Convert key to UTF32 codepoints
        let key: Vec<U32> = key.chars().map(|c| (c as u32).into()).collect();

        // Compute bucket for this key
        let bucket = hash_sdbm(key.clone().into_iter()) % n_buckets;

        // Linear probing
        for i in 0..n_buckets {
            let probed_bucket = (bucket + i) % n_buckets;

            // Found an empty bucket
            if buckets[probed_bucket as usize].offset == BUCKET_OFFSET_EMPTY {
                buckets[probed_bucket as usize] = SimpleUnistrU32HashMapBucket {
                    offset: (offset as u32).into(),
                    value: value.into(),
                };

                break;
            }
        }

        // Add string to arena
        arena.extend_from_slice(key.as_bytes());
    }

    // Consturct hashmap header
    let header = SimpleUnistrU32HashMapHeader {
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

/// Compute byte-per-byte SDBM hash
fn hash_sdbm<'a, I, T>(s: I) -> u32
where
    I: Iterator<Item = T>,
    T: IntoBytes + Immutable,
{
    let mut hash: u32 = 0;
    for element in s {
        for byte in element.as_bytes() {
            hash = (*byte as u32)
                .wrapping_add(hash << 6)
                .wrapping_add(hash << 16)
                .wrapping_sub(hash);
        }
    }
    hash
}

#[cfg(test)]
mod tests {
    use zerocopy::little_endian;

    use super::{build_simple_utf32str_u32_hashmap, hash_sdbm};

    #[test]
    fn test_hash_sdbm() {
        let input: [little_endian::U32; 3] =
            [0x74736574.into(), 0x31676E69.into(), 0x35343332.into()];

        let output = hash_sdbm(input.into_iter());

        assert_eq!(output, 0xc0de8883);
    }

    #[test]
    fn test_hashmap_empty() {
        let data = vec![];

        let map_bytes = build_simple_utf32str_u32_hashmap(data).unwrap();

        assert_eq!(map_bytes.len(), 16);
        assert_eq!(map_bytes[0..8], [1, 0, 0, 0, 0, 0, 0, 0]);
    }

    #[test]
    fn test_hashmap_two_items() {
        let data = vec![("test".into(), 0xAAAABBBB), ("😀".into(), 0x10101010)];

        let map_bytes = build_simple_utf32str_u32_hashmap(data).unwrap();

        assert_eq!(map_bytes.len(), 60);
        assert_eq!(map_bytes[0..8], [4, 0, 0, 0, 0, 0, 0, 0]); // Hashmap header
    }
}
