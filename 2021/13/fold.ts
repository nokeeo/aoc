declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/13/fold.txt';

class Dot {
  readonly x:number;
  readonly y:number;

  constructor(x:number, y:number) {
    this.x = x;
    this.y = y;
  }

  toString():string {
    return `${this.x},${this.y}`;
  }

  add(dot:Dot):Dot {
    return new Dot(this.x + dot.x, this.y + dot.y);
  }

  subtract(dot:Dot):Dot {
    return new Dot(this.x - dot.x, this.y - dot.y);
  }

  divide(denom:Dot):Dot {
    const x = (denom.x === 0) ? 0 : this.x / denom.x;
    const y = (denom.y === 0) ? 0 : this.y / denom.y;
    return new Dot(x, y);
  }

  mult(dot:Dot):Dot {
    return new Dot(this.x * dot.x, this.y * dot.y);
  }
}

class DotCollection {
  private dots:{[id:string]: Dot};
  private maxX = Number.MIN_VALUE;
  private maxY = Number.MIN_VALUE;

  constructor() {
    this.dots = {};
  }

  add(dot:Dot) {
    this.maxX = Math.max(this.maxX, dot.x);
    this.maxY= Math.max(this.maxY, dot.y);
    this.dots[dot.toString()] = dot;
  }

  forEach(func:(dot:Dot) => boolean) {
    for (const key of Object.keys(this.dots)) {
      if (!func(this.dots[key])) {
        break;
      }
    }
  }

  size():number {
    return Object.keys(this.dots).length;
  }

  toString():string {
    const dots = Object.keys(this.dots).map((key) => {
      return this.dots[key];
    });

    dots.sort((a, b) => {
      if (a.y === b.y) {
        return a.x - b.x;
      }
      return a.y - b.y;
    });

    let str = '';
    let dot = dots.shift();
    for (let y = 0; y <= this.maxY; y++) {
      for (let x = 0; x <= this.maxX; x++) {
        if (dot.x === x && dot.y === y) {
          str += '#';
          dot = dots.shift();
        } else {
          str += '.';
        }
      }
      if (y !== this.maxY) {
        str += '\n';
      }
    }

    return str;
  }
}

class Input {
  dots:DotCollection;
  instructions:Dot[];

  constructor() {
    this.dots = new DotCollection();
    this.instructions = [];
  }
}

function ParseLines(lines:string[]):Input {
  const input = new Input();
  // Parse dots.
  let i = 0;
  while (i < lines.length) {
    const line = lines[i];
    i++;
    if (line === '') {
      break;
    }
    const parts = line.split(',');
    input.dots.add(new Dot(Number(parts[0]), Number(parts[1])));
  }

  for (let k = i; k < lines.length; k++) {
    const line = lines[k];
    if (line === '') {
      continue;
    }

    const parts = line.split(' ');
    const directionValue = parts[2].split('=');
    const x = directionValue[0] === 'x' ? Number(directionValue[1]) : 0;
    const y = directionValue[0] === 'y' ? Number(directionValue[1]) : 0;
    input.instructions.push(new Dot(x, y));
  }

  return input;
}

function Fold(dots:DotCollection, fold:Dot):DotCollection {
  const newCollection = new DotCollection();
  dots.forEach((dot) => {
    if (dot.x < fold.x || dot.y < fold.y) {
      newCollection.add(dot);
    } else {
      const unit = fold.divide(fold);
      const diff = dot.subtract(fold).mult(unit);
      const offset = dot.mult(new Dot(unit.y, unit.x));
      const folded = fold.add(offset).subtract(diff);
      newCollection.add(folded);
    }
    return true;
  });
  return newCollection;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines = data.split('\n');
  const input = ParseLines(lines);

  let collection = input.dots;
  for (let fold of input.instructions) {
    collection = Fold(collection, fold);
  }

  console.log(collection.toString());
});
