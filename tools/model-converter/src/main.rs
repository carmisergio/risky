mod cli;
mod hashmap;

use serde::Deserialize;
use std::{
    collections::HashMap,
    fs::{self, File},
    io::{self, Read, Write},
    path::PathBuf,
};
use zerocopy::little_endian;
use zerocopy::{Immutable, IntoBytes};

//use binbin::endian::Endian;
use clap::Parser;
use cli::Args;

use crate::hashmap::build_simple_utf32str_u32_hashmap;

const MAGIC: u32 = 0x52534B59; // 'RSKY'

fn main() {
    let args = Args::parse();

    // Create output file
    let mut file = File::create(args.output_path).unwrap();

    let tokenizer_file =
        fs::read_to_string(args.tokenizer_path).expect("Unable to read tokenizer file");
    let tokenizer: Tokenizer =
        serde_json::from_str(&tokenizer_file).expect("Unable to parse tokenizer file");

    let hashmap_data: Vec<(String, u32)> = tokenizer.model.vocab.into_iter().collect();
    let hashmap_bytes = build_simple_utf32str_u32_hashmap(hashmap_data).unwrap();

    dbg!(hashmap_bytes.len());

    //// Write output file
    //binbin::write_be(&mut file, |w| {
    //    // Magic byte(s)
    //    w.write(MAGIC)?;

    //    // Seciton locations

    //    //// Tokenizer model
    //    //convert_tokenizer(&args.tokenizer_path, w)?;

    //    write_simple_hashmap() ->

    //    Ok(())
    //})
    //.unwrap();
}

#[derive(Deserialize)]
struct Tokenizer {
    model: TokenizerModel,
}

#[derive(Deserialize)]
struct TokenizerModel {
    vocab: HashMap<String, u32>,
    merges: Vec<String>,
}

//fn convert_tokenizer<W, E>(file_path: &PathBuf, w: &mut binbin::Writer<W, E>) -> io::Result<()>
//where
//    W: Write,
//    E: Endian,
//{
//    Ok(())
//}

//fn write_simple_static_hashmap<W, E>(w: &mut binbin::Writer<W, E>, data: HashMap<String, u32>) -> io::Result<()>
//where
//    W: Write,
//    E: Endian,
//{

//    let n_buckets: u32 = 10;
//    let bucket_size: u32 = 10;

//    // Static Hashmap Header
//    w.write(n_buckets)?;
//    w.write()

//    Ok(())
//}
