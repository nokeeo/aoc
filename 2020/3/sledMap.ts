declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode2020/3/sledMap.txt';

class Row {
  private pattern:string;

  constructor(pattern:string) {
    this.pattern = pattern;
  }

  isColumnOpen(index:number) {
    const patternIndex:number = index % this.pattern.length;
    const char:string = this.pattern[patternIndex];
    return char == '.';
  }
}

function GetTreeCount(right:number, down:number, rows:Row[]) {
  let index:number = 0;
  let treeCount:number = 0;
  for (let i = 0; i < rows.length; i += down) {
    const row = rows[i];
    if(!row.isColumnOpen(index)) {
      treeCount++;
    }
    index += right;
  }

  return treeCount;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const rows:Row[] = data.split('\n').filter((line:string) => {
    return line != '';
  }).map((line:string) => {
    return new Row(line);
  });

  const run1 = GetTreeCount(1, 1, rows);
  const run2 = GetTreeCount(3, 1, rows);
  const run3 = GetTreeCount(5, 1, rows);
  const run4 = GetTreeCount(7, 1, rows);
  const run5 = GetTreeCount(1, 2, rows);

  console.log(run1 * run2 * run3 * run4 * run5);
});
