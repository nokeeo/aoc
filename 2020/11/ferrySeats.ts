declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/11/ferrySeats.txt';

function GetValue(x:number, y:number, rows:string[]) : string {
  return rows[y].charAt(x);
}

function GetSeatInDirection(x:number, y:number, deltaX:number, deltaY:number, rows:string[]) : string {
  while (true) {
    let newX = x + deltaX;
    let newY = y + deltaY;
    if (newY >= 0 && newY < rows.length && newX >= 0 && newX < rows[y].length) {
      const value = GetValue(newX, newY, rows);
      if (value !== '.') {
        return value;
      }
    } else {
      break;
    }

    x = newX;
    y = newY;
  }

  return null;
}

function GetAdjacentSeats(x:number, y:number, rows:string[]) : string[] {
  const neighbors:string[] = [];
  const upperCheck = y > 0;
  const leftCheck = x > 0;
  const lowerCheck = y < rows.length - 1;
  const rightCheck = x < rows[y].length - 1;
  // Up.
  if (upperCheck) {
    neighbors.push(GetValue(x, y - 1, rows));

    // Up left.
    if (leftCheck) {
      neighbors.push(GetValue(x - 1, y - 1, rows));
    }
    // Up right.
    if (rightCheck) {
      neighbors.push(GetValue(x + 1, y - 1, rows));
    }
  }

  // Down.
  if (lowerCheck) {
    neighbors.push(GetValue(x, y + 1, rows));
    // Down left.
    if (leftCheck) {
      neighbors.push(GetValue(x - 1, y + 1, rows));
    }
    // Down right.
    if (rightCheck) {
      neighbors.push(GetValue(x + 1, y + 1, rows));
    }
  }
  // Left.
  if (leftCheck) {
    neighbors.push(GetValue(x - 1, y, rows));
  }
  // Right.
  if (rightCheck) {
    neighbors.push(GetValue(x + 1, y, rows));
  }

  return neighbors;
}

function GetAdjacentVisibleSeats(x:number, y:number, rows:string[]) : string[] {
  const neighbors = [];

  // Up.
  const up = GetSeatInDirection(x, y, 0, -1, rows);
  if (up !== null) {
    neighbors.push(up);
  }
  // Up left.
  const upLeft = GetSeatInDirection(x, y, -1, -1, rows);
  if (upLeft !== null) {
    neighbors.push(upLeft)
  }
  // Up right.
  const upRight = GetSeatInDirection(x, y, 1, -1, rows);
  if (upRight !== null) {
    neighbors.push(upRight);
  }

  // Down.
  const down = GetSeatInDirection(x, y, 0, 1, rows);
  if (down !== null) {
    neighbors.push(down);
  }
  // Down left.
  const downLeft = GetSeatInDirection(x, y, -1, 1, rows);
  if (downLeft !== null) {
    neighbors.push(downLeft);
  }
  // Down right.
  const downRight = GetSeatInDirection(x, y, 1, 1, rows);
  if (downRight !== null) {
    neighbors.push(downRight);
  }

  // Left.
  const left = GetSeatInDirection(x, y, -1, 0, rows);
  if (left !== null) {
    neighbors.push(left);
  }

  // Right.
  const right = GetSeatInDirection(x, y, 1, 0, rows);
  if (right !== null) {
    neighbors.push(right);
  }

  return neighbors;
}

function AreAnySeatsOccupied(seats:string[]) {
  return seats.reduce((acc:boolean, currentValue:string) => {
    return acc || (currentValue === '#');
  }, false);
}

function HandleEmptySeat(adjacentSeats:string[]) : string {
  if (!AreAnySeatsOccupied(adjacentSeats)) {
    return '#';
  }

  return 'L';
}

function HandleOccupiedSeat(adjacentSeats:string[]) : string {
  let occupiedSeatCount = 0;
  for (let adjacentSeat of adjacentSeats) {
    if (adjacentSeat === '#') {
      occupiedSeatCount++;
    }
  }

  return (occupiedSeatCount >= 5) ? 'L' : '#';
}

function CountOccupiedSeatsOnFerry(rows:string[]) : number {
  let count = 0;
  for (let row of rows) {
    for (let i = 0; i < row.length; i++) {
      if (row.charAt(i) === '#') {
        count++;
      }
    }
  }

  return count;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  let rows:string[] = data.split('\n').filter((line:string) => {
    return line !== '';
  });

  let seatsChanged = false;
  let newSeats = [];
  while (true) {
    for (let y = 0; y < rows.length; y++) {
      const row = rows[y];
      let newRow = '';
      for (let x = 0; x < row.length; x++) {
        const currentValue = GetValue(x, y, rows);
        const adjacentSeats = GetAdjacentVisibleSeats(x, y, rows);
        let newValue = currentValue;
        if (currentValue === 'L') {
          newValue = HandleEmptySeat(adjacentSeats);
        } else if (currentValue === '#') {
          newValue = HandleOccupiedSeat(adjacentSeats);
        }
        newRow += newValue;
        seatsChanged = seatsChanged || (newValue !== currentValue);
      }
      newSeats.push(newRow);
    }


    rows = newSeats;
    newSeats = [];
    if (!seatsChanged) {
      break;
    }
    seatsChanged = false;
  }

  console.log(CountOccupiedSeatsOnFerry(rows));
});
