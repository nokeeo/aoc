declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode2020/5/boardingPass.txt';

class BinaryRange {
  min:number;
  max:number;

  constructor(min:number, max:number) {
    this.min = min;
    this.max = max;
  }

  subdivideFront() {
    this.max -= this.getHalfElementCount();
  }

  subdivideBack() {
    this.min += this.getHalfElementCount();
  }

  private getHalfElementCount() {
    return Math.ceil((this.max - this.min) / 2)
  }
}

function GetIDForBoardingPass(pass:string) : number {
  const rowRange = new BinaryRange(1, 128);
  const columnRange = new BinaryRange(1, 8);

  for(let i = 0; i < pass.length; i++) {
    const char:string = pass[i];
    if (char === 'F') {
      rowRange.subdivideFront();
    } else if (char === 'B') {
      rowRange.subdivideBack();
    } else if (char === 'L') {
      columnRange.subdivideFront();
    } else if (char === 'R') {
      columnRange.subdivideBack();
    }
  }

  return (rowRange.min - 1) * 8 + (columnRange.min - 1);
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  const boardingPassIDs:number[] = [];
  for (let line of lines) {
    boardingPassIDs.push(GetIDForBoardingPass(line));
  }

  boardingPassIDs.sort((a:number, b:number) => {
    return a - b;
  });

  for (let i = 0; i < boardingPassIDs.length - 1; i++) {
    let currentPass = boardingPassIDs[i];
    const expectedNextPassID = currentPass + 1;
    if (expectedNextPassID != boardingPassIDs[i + 1]) {
      console.log(expectedNextPassID);
      break;
    }
  }
});
