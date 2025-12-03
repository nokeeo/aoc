use std::env;
use std::fs::File;
use std::io::{self, BufRead};

// --- Day 3: Lobby ---

struct FindMaxResult {
  i: usize,
  value: u32,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_2(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>>{
  let file = File::open(filename)?;
  let lines = io::BufReader::new(file).lines();
  let mut sum = 0;
  for line in lines.map_while(Result::ok) {
    sum += get_joltage(&line, /*n=*/2);
  }
  Ok(sum)
}

fn part_2(filename: &str) -> Result<u64, Box<dyn std::error::Error>>{
  let file = File::open(filename)?;
  let lines = io::BufReader::new(file).lines();
  let mut sum = 0;
  for line in lines.map_while(Result::ok) {
    sum += get_joltage(&line, /*n=*/12);
  }
  Ok(sum)
}

fn get_joltage(bank: &str, n: usize) -> u64 {
  let mut sum: u64 = 0;
  let mut search_str: &str = bank;
  for i in 0..n {
    let result = find_max(&search_str[..=(search_str.len() - (n - i))]);
    let max = result.value as u64 * 10u64.pow((n - i - 1) as u32);
    if result.i != search_str.len() - 1 {
      search_str = &search_str[result.i + 1..];
    }
    sum += max;
  }
  sum
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