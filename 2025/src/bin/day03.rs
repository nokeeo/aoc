use std::env;
use std::fs::File;
use std::io::{self, BufRead};

struct FindMaxResult {
  i: usize,
  value: u32,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>>{
  let file = File::open(filename)?;
  let lines = io::BufReader::new(file).lines();
  let mut sum = 0;
  for line in lines.map_while(Result::ok) {
    sum += get_joltage(&line);
  }
  Ok(sum)
}

fn get_joltage(bank: &str) -> u64 {
  let first = find_max(&bank[..bank.len() - 1]);
  let second = find_max(&bank[first.i + 1..]);
  (first.value * 10 + second.value) as u64
}

fn find_max(bank: &str) -> FindMaxResult {
  let mut max: u32 = 0;
  let mut index: usize = 0;
  for (pos, c) in bank.chars().enumerate() {
    let value = c.to_digit(10).expect("Failed to parse integer") as u32;
    if value > max {
      max = value;
      index = pos;
    }
  }
  FindMaxResult {
    i: index,
    value: max,
  }
}