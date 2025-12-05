use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use regex::Regex;

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);

  let mut iter = reader.lines().map_while(Result::ok);
  // let mut ranges = Vec<std::ops::RangeInclusive<u64>>::new();
  let range_regex = Regex::new(r"(\d+)-(\d+)").unwrap();
  let mut ranges: Vec<std::ops::RangeInclusive<u64>> = Vec::new(); 
  for line in iter.by_ref().take_while(|l| !l.is_empty()) {
    let Some(bounds) = range_regex.captures(&line) else {
      panic!("Failed to parse line");
    };
      let lower = bounds[1].parse::<u64>().unwrap();
      let upper = bounds[2].parse::<u64>().unwrap();
      ranges.push(lower..=upper);
  }

  let mut count = 0;
  for line in iter {
    let n = line.parse::<u64>().unwrap();
    for range in ranges.iter() {
      if range.contains(&n) {
        count += 1;
        break;
      }
    }
  }
  Ok(count)
}