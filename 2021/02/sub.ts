declare function require(name:string);
const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/2/sub.txt';

namespace Sub {

class Position {
  x: number;
  y: number;
  a: number;

  constructor() {
    this.x = 0;
    this.y = 0;
    this.a = 0;
  }

  moveCommand(cmd:string) {
    const values = cmd.split(' ');
    const numValue = Number(values[1]);
    if (values[0] === 'forward') {
      this.x += numValue;
      this.y += this.a * numValue;
    } else {
      // Aim correction
      const direction = values[0] === 'down' ? 1 : -1;
      this.a += numValue * direction;
    }
  }
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  let pos = new Position();
  for (let line of lines) {
    pos.moveCommand(line);
  }
  console.log(pos.x * pos.y);
});

}
