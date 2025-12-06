use std::env;
use std::fs::File;
use std::io::{self, BufRead};

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);
  println!("{}", part_2(&args[1])?);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let mut problems: Vec<Vec<u64>> = Vec::new();
  let mut operations: Vec<Vec<char>> = Vec::new();
  for line in reader.lines().map_while(Result::ok) {
    for (i, num) in line.split(' ').filter(|l| l != &"").enumerate() {
      let Some(num) = num.parse::<u64>().ok() else {
        if let Some(problem_ops) = operations.get_mut(i) {
          problem_ops.push(num.chars().nth(0).expect("Need char"));
        } else {
          operations.push(vec!(num.chars().nth(0).expect("Need char")));
        }
        continue;
      };

      if let Some(problem) = problems.get_mut(i) {
        problem.push(num);
      } else {
        problems.push(vec!(num));
      }
    }
  }

  let mut sum = 0;
  for (i, problem) in problems.iter().enumerate() {
    let op = operations[i][0];
    sum += problem.iter().copied().reduce(|acc, n| execute(acc, n, op)).unwrap();
  }

  Ok(sum)
}

fn part_2(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let mut lines: Vec<String> = Vec::new();
  for line in reader.lines().map_while(Result::ok) {
    lines.push(line.chars().rev().collect());
  }

  // Hardcoded for input.
  let digits = &lines[..4];
  let ops = &lines[4];
  let i = 0;

  let mut nums: Vec<u64> = Vec::new();
  let mut skip = false;
  let mut sum = 0;
  for i in 0..lines[0].len() {
    if skip {
      skip = false;
      continue;
    }

    let mut num = 0;
    let mut pow = 0;
    for (j, digit_line) in digits.iter().rev().enumerate() {
      let digit = digit_line.chars().nth(i).expect("No char");
      if digit == ' ' {
        continue;
      }

      let digit = digit.to_digit(10).unwrap_or(0) as u64;
      num += 10_u64.pow(pow) * digit;
      pow += 1;
    }
    nums.push(num);

    let op = ops.chars().nth(i).unwrap();
    if op != ' ' {
      sum += nums.iter().copied().reduce(|acc, n| execute(acc, n, op)).unwrap();
      nums.clear();
      skip = true;
    }
  }

  Ok(sum)
}

fn execute(n1: u64, n2: u64, op: char) -> u64 {
  if op == '+' {
    return n1 + n2;
  }
  n1 * n2
}