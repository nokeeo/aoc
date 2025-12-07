// use std::collections::HashSet;
use std::env;
use std::fs::File;
use std::io::{self, BufRead};

// --- Day 7: Laboratories ---

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);
  println!("{}", part_2(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let mut iter = reader.lines().map_while(Result::ok);

  let first_line = iter.nth(0).expect("Need a first line");
  let mut beams = vec![0; first_line.len()];
  let start_i = first_line.chars().position(|c| c == 'S').expect("No start position in input");

  beams[start_i] = 1;
  let mut count = 0;
  for line in iter {
    for (splitter_index, _) in line.chars().enumerate().filter(|(_, c)| *c != '.') {
      if beams[splitter_index] == 0 {
        continue;
      }
      beams[splitter_index] = 0;
      beams[splitter_index + 1] = 1;
      beams[splitter_index - 1] = 1;
      count += 1;
    }
  }
  Ok(count)
}

fn part_2(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let mut iter = reader.lines().map_while(Result::ok);

  let first_line = iter.nth(0).expect("Need a first line");
  let mut beams = vec![0; first_line.len()];
  let start_i = first_line.chars().position(|c| c == 'S').expect("No start position in input");
  beams[start_i] = 1;

  for line in iter {
    for (splitter_index, _) in line.chars().enumerate().filter(|(_, c)| *c != '.') {
      if beams[splitter_index] == 0 {
        continue;
      }
      beams[splitter_index + 1] += beams[splitter_index];
      beams[splitter_index - 1] += beams[splitter_index];
      beams[splitter_index] = 0;
    }
  }
  Ok(beams.iter().sum())
}