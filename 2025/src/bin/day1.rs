use std::env;
use std::fs::File;
use std::io::{self, BufRead};

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  let file = File::open(&args[2])?;
  let lines = io::BufReader::new(file).lines();
  println!("{}", part2(lines));
  return Ok(());
}

fn part1(lines: std::io::Lines<io::BufReader<File>>) -> isize {
  let mut dial = 50;
  let mut count = 0;
  for line in lines.map_while(Result::ok) {
   dial += parse_rotation(&line);
   dial = normalizeDial(dial);

   if dial == 0 {
    count += 1;
   }
  }
  return count;
}

fn part2(lines: std::io::Lines<io::BufReader<File>>) -> isize {
  let mut dial = 50;
  let mut count = 0;
  for line in lines.map_while(Result::ok) {
    let oldDial = dial;
    dial += parse_rotation(&line);
    count += dial.abs() / 100;
    if dial == 0 {
      count += 1;
    } else if oldDial != 0 && oldDial.signum() != dial.signum() {
      count += 1;
    }

    dial = normalizeDial(dial);
  }
  return count;
}

fn normalizeDial(dial: isize) -> isize {
  if dial >= 0 {
    return dial % 100;
  }
  return normalizeDial(100 - (dial.abs() % 100));
}

fn parse_rotation(line: &str) -> isize {
  let direction = match line.chars().nth(0).unwrap() {
    'L' => -1,
    _ => 1,
  };

  let value = &line[1..].parse::<isize>().unwrap();
  return direction * value;
}