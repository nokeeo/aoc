declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/12/ferryNav.txt';

class Ferry {
  wayPointX:number;
  wayPointY:number;

  x:number;
  y:number;

  constructor() {
    this.wayPointX = 10;
    this.wayPointY = 1;
    this.x = 0;
    this.y = 0;
  }

  navigate(cmd:string, value:number) {
    if (cmd === 'N') {
      this.wayPointY += value;
    } else if(cmd === 'S') {
      this.wayPointY -= value;
    } else if (cmd === 'E') {
      this.wayPointX += value;
    } else if (cmd === 'W') {
      this.wayPointX -= value;
    } else if(cmd === 'L') {
      value *= Math.PI/180;
      const x = this.wayPointX;
      const y = this.wayPointY;
      this.wayPointX = (x * Math.cos(value)) - (y * Math.sin(value));
      this.wayPointY = (x * Math.sin(value)) + (y * Math.cos(value));
    } else if (cmd === 'R') {
      value *= Math.PI/-180;
      const x = this.wayPointX;
      const y = this.wayPointY;
      this.wayPointX = (x * Math.cos(value)) - (y * Math.sin(value));
      this.wayPointY = (x * Math.sin(value)) + (y * Math.cos(value));
    } else if (cmd === 'F') {
      this.forward(value);
    }
  }

  private forward(value:number) {
    this.x += this.wayPointX * value;
    this.y += this.wayPointY * value;
  }
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line !== '';
  });

  const ferry = new Ferry();
  for (let line of lines) {
    const cmd = line.slice(0, 1);
    const value = Number(line.slice(1, line.length));
    ferry.navigate(cmd, value);
  }

  console.log(ferry);
  console.log(Math.abs(ferry.x) + Math.abs(ferry.y))
});
