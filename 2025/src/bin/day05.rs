use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use regex::Regex;

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
  let ranges = parse_ranges(&mut iter);
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

fn part_2(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);

  let mut iter = reader.lines().map_while(Result::ok);
  let ranges = parse_ranges(&mut iter);

  let merged_ranges = merge_ranges(&ranges);

  let mut sum = 0;
  for r in merged_ranges {
    sum += r.end() - r.start() + 1;
  }
  Ok(sum)
}

fn merge_ranges(ranges: &Vec<std::ops::RangeInclusive<u64>>) -> Vec<std::ops::RangeInclusive<u64>> {
  let mut ranges = ranges.clone();

  ranges.sort_by(|a, b| {
    a.start().cmp(b.start()).then(a.end().cmp(b.end()))
  });

  if ranges.len() < 2 {
    return ranges;
  }

  let mut merged_ranges: Vec<std::ops::RangeInclusive<u64>> = Vec::new();
  let mut i = 0;
  while i < ranges.len() {
    let mut r1 = ranges[i].clone();
    let mut j = i + 1;
    while j < ranges.len() {
      let r2 = &ranges[j];
      if (r2.start() >= r1.start() && r2.start() <= r1.end()) ||
         (r2.end() >= r1.start() && r2.end() <= r1.end()) {
        r1 = *std::cmp::min(r1.start(), r2.start())..=*std::cmp::max(r1.end(), r2.end());
      } else {
        break;
      }
      j += 1;
    }
    merged_ranges.push(r1);
    i = j;
  }
  merged_ranges

  // let mut merged = true;
  // let mut ranges = ranges.clone();

  // let mut merged_ranges: Vec<std::ops::RangeInclusive<u64>> = Vec::new();
  // while merged {
  //   merged = false;
  //   for (index, r1) in ranges.iter().enumerate() {
  //     let mut merged_r1 = false;
  //     for r2 in &ranges[index + 1..] {
  //       if r2.start() < r1.end() || r1.start() < r2.end() {
  //         merged = true;
  //         let lower = std::cmp::min(r1.start(), r2.start());
  //         let upper = std::cmp::max(r1.end(), r2.end()); 
  //         merged_ranges.push(*lower..=*upper);
  //         merged_r1 = true;
  //         break;
  //       }
  //     }

  //     if !merged_r1 {
  //       merged_ranges.push(r1.clone());
  //     }
  //   }

  //   if merged {
  //     ranges = merged_ranges.clone();
  //   }
  // }
  // merged_ranges
}

fn parse_ranges<T>(iter: &mut T) -> Vec<std::ops::RangeInclusive<u64>>
  where T: Iterator<Item = String> {
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
  ranges
}