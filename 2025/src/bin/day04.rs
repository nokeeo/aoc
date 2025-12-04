use array2d::Array2D;
use std::env;
use std::fs::File;
use std::io::{self, BufRead};

// --- Day 4: Printing Department ---

fn main() -> Result<(), Box<dyn std::error::Error>> {
  let args: Vec<String> = env::args().collect();
  println!("{}", part_1(&args[1])?);

  use std::time::Instant;
  let now = Instant::now();
  println!("{}", part_2(&args[1])?);
  let elapsed = now.elapsed();
  println!("Elapsed: {:.2?}", elapsed);
  Ok(())
}

fn part_1(filename: &str) -> Result<u64, Box<dyn std::error::Error>>{
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let grid_size = get_grid_size(reader);

  let mut grid = Array2D::filled_with('.', grid_size.0, grid_size.1);
  file = File::open(filename)?;
  reader = io::BufReader::new(file);
  let mut row = 0;
  for line in reader.lines().map_while(Result::ok) {
    for c in 0..line.len() {
      grid.set(row, c, line.chars().nth(c).expect("Out out bounds"));
    }
    row += 1;
  }

  let mut count = 0;
  for (row_index, row) in grid.rows_iter().enumerate() {
    for (column_index, element) in row.enumerate() {
      if *element == '@' && adjacent_roll_count(row_index, column_index, &grid) < 4 {
        count += 1;
      }
    }
  }
  Ok(count)
}

fn part_2(filename: &str) -> Result<u64, Box<dyn std::error::Error>> {
  let mut file = File::open(filename)?;
  let mut reader = io::BufReader::new(file);
  let grid_size = get_grid_size(reader);

  let mut grid = Array2D::filled_with('.', grid_size.0, grid_size.1);
  file = File::open(filename)?;
  reader = io::BufReader::new(file);
  let mut row = 0;
  for line in reader.lines().map_while(Result::ok) {
    for c in 0..line.len() {
      grid.set(row, c, line.chars().nth(c).expect("Out out bounds"));
    }
    row += 1;
  }

  let mut removed = true;
  let mut removed_count = 0;
  while removed {
    removed = false;
    let mut indices_to_remove: Vec<(usize, usize)> = Vec::new();
    for (row_index, row) in grid.rows_iter().enumerate() {
      for (column_index, element) in row.enumerate() {
        if *element == '@' && adjacent_roll_count(row_index, column_index, &grid) < 4 {
          removed = true;
          indices_to_remove.push((row_index, column_index));
        }
      }
    }

    if removed {
      removed_count += indices_to_remove.len();
      for index in indices_to_remove {
        grid.set(index.0, index.1, '.');
      }
    }
  }
  Ok(removed_count as u64)
}

fn adjacent_roll_count(row: usize, column: usize, grid: &Array2D<char>) -> u32 {
  let mut count = 0;
  // Top.
  if row > 0 && column > 0 {
    count += is_roll(row - 1, column - 1, grid);
  }

  if row > 0 {
    count += is_roll(row - 1, column, grid);
    count += is_roll(row - 1, column + 1, grid);
  }

  if column > 0 {
    count += is_roll(row, column - 1, grid);
    count += is_roll(row + 1, column - 1, grid);
  }
  count += is_roll(row, column + 1, grid);

  // Bottom.
  count += is_roll(row + 1, column, grid);
  count += is_roll(row + 1, column + 1, grid);

  count
}

fn is_roll(row: usize, column: usize, grid: &Array2D<char>) -> u32 {
  let Some(space) = grid.get(row, column) else {
    return 0;
  };
  if *space == '@' { 1 } else { 0 }
}

fn get_grid_size(reader: io::BufReader<File>) -> (usize, usize) {
  let mut width = 0;
  let mut height = 0;
  for line in reader.lines().map_while(Result::ok) {
    width = line.len();
    height += 1; 
  }
  return (width, height);
}