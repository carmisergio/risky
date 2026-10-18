use clap::Parser;
use std::path::PathBuf;

/// Simple program to greet a person
#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
pub struct Args {
    /// Tokenizer model JSON
    #[arg(short, long)]
    pub tokenizer_path: PathBuf,

    /// Output filename
    #[arg(short, long, default_value = "converted.bin")]
    pub output_path: PathBuf,
}
