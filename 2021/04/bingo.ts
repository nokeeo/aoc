declare function require(name:string);
declare namespace console {
  function log(param:any);
}

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/4/bingo.txt';

function SetFromArray<Type>(arr:Type[]):Set<Type> {
  const set:Set<Type> = new Set();
  for (let element of arr) {
    set.add(element);
  }

  return set;
}

function Intersection(setA, setB) {
    let _intersection = new Set()
    setB.forEach((elem) => {
      if (setA.has(elem)) {
        _intersection.add(elem)
      }
    });
    return _intersection
}

function Union(setA, setB) {
  let union = new Set();
  setA.forEach((elem) => {
    union.add(elem);
  })

  setB.forEach((elem) => {
    union.add(elem);
  });

  return union;
}

function SetDiff(setA, setB) {
  let diff = new Set();
  setA.forEach((elem) => {
    if (!setB.has(elem)) {
      diff.add(elem);
    }
  });

  return diff;
}

class Board {
  private rowSets:Set<number>[] = [];
  private colSets:Set<number>[] = [];

  constructor(rows:string[]) {
    const boardArray  = [];
    for(let row of rows) {
      boardArray.push(row.split(' ').filter((element) => {
        return element != '';
      }).map((element) => {
        return Number(element);
      }));
    }

    // Parse rows
    for (let row of boardArray) {
      this.rowSets.push(SetFromArray(row));
    }

    // Parse columns
    for (let i = 0; i < boardArray[0].length; i++) {
      const set:Set<number> = new Set();
      for (let row of boardArray) {
        set.add(row[i]);
      }
      this.colSets.push(set);
    }
  }

  isWinner(nums:Set<number>):boolean {
    for (let row of this.rowSets) {
      if (Intersection(row, nums).size === row.size) {
        return true;
      }
    }

    for (let col of this.colSets) {
      if (Intersection(col, nums).size === col.size) {
        return true;
      }
    }

    return false;
  }

  sumUnmarked(marked:Set<number>):number {
    let unmarked = new Set();
    for (let row of this.rowSets) {
      unmarked = Union(unmarked, SetDiff(row, marked));
    }

    let sum = 0;
    unmarked.forEach((elem) => {
      sum += elem;
    });

    return sum;
  }
}

function ParseNextBoard(lines:string[]):Board {
  let nextLine = lines.shift();
  const boardLines:string[] = [];
  while (nextLine != '') {
    boardLines.push(nextLine);
    nextLine = lines.shift();
  }

  if (boardLines.length > 0) {
    return new Board(boardLines);
  }

  return null;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n');

  const draws:number[] = lines.shift().split(',').map((draw) => {
    return Number(draw);
  });
  lines.shift();  // Shift off extra newline.

  const boards = [];
  while(lines.length > 0) {
    boards.push(ParseNextBoard(lines));
  }

  const currentDraws:Set<number> = new Set();
  const winners:Set<Board> = new Set();
  let lastWinner = null;
  let num = 0;
  while (draws.length > 0) {
    num = draws.shift();
    currentDraws.add(num);
    for (let board of boards) {
      if(!winners.has(board) && board.isWinner(currentDraws)) {
        lastWinner = board;
        winners.add(board);
      }
    }

    if (winners.size === boards.length) {
      break;
    }
  }

  console.log(lastWinner.sumUnmarked(currentDraws) * num);
});
