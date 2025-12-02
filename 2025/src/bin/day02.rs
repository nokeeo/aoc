use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Range;

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let ranges = parse_ranges(filename)?;
  let mut count: u64 = 0;
  for range in ranges {
    count += invalid_id_count_p2(&range);
  }
  Ok(count)
}

fn invalid_id_count(range: &Range<u64>) -> u64 {
  let mut sum: u64 = 0;
  // Just brute force it.
  for digits in 2..=20 {
        if digits % 2 != 0 { continue; }
        let half = digits / 2;
        let pow_half = 10u64.pow(half as u32);
        let half_min = 10u64.pow((half - 1) as u32);
        let half_max = 10u64.pow(half as u32) - 1;
        for h in half_min..=half_max {
            let n = h * pow_half + h;
            if n < range.start { continue; }
            if n >= range.end   { break; }
            sum += n;
        }
    }
    sum
}

fn invalid_id_count_p2(range: &Range<u64>) -> u64 {
  let mut sum: u64 = 0;
  for num in range.clone() {
    if repeating_digits(num) {
      sum += num;
    }
  }
  sum
}

fn repeating_digits(num: u64) -> bool {
  if num == 0 { return false; }
    let digits = num.ilog10() + 1;
    'digitLoop: for n in 1..=digits/2 {
        if digits % n != 0 { continue; }
        let seq = get_least_sig(num, n);
        let mut remainder = pop_least_sig(num, n);
        while remainder > 0 {
            let next_seq = get_least_sig(remainder, n);
            if next_seq != seq {
                continue 'digitLoop;
            }
            remainder = pop_least_sig(remainder, n);
        }
        return true;
    }
    false
}

fn get_least_sig(num: u64, n: u32) -> u64 {
  num - num / 10u64.pow(n) * 10u64.pow(n)
}

fn pop_least_sig(num: u64, n: u32) -> u64 {
  num / 10_u64.pow(n)
}

fn parse_ranges(filename: &str) -> Result<Vec<Range<u64>>, Box<dyn std::error::Error>> {
  let file = File::open(filename)?;
  let lines = io::BufReader::new(file).lines();
  let mut result = Vec::new();
  for line in lines.map_while(Result::ok) {
    for range_str in line.split(",") {
      let bounds: Vec<&str> = range_str.split("-").collect();
      result.push(Range{ start: bounds[0].parse::<u64>()?, end: bounds[1].parse::<u64>()? + 1});
    }
  }
  Ok(result)
}