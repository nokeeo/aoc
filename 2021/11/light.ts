declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/11/light.txt';

class Point {
  x:number;
  y:number;

  constructor(x:number, y:number) {
    this.x = x;
    this.y = y;
  }

  toString():string {
    return `${this.x},${this.y}`;
  }
}
const GridNeighborCardinal = function(p:Point, grid:Grid) {
  const neighbors = [];
    // Left
    if (p.x - 1 >= 0) {
      neighbors.push(new Point(p.x - 1, p.y));
    }
    // Top
    if (p.y - 1 >= 0) {
      neighbors.push(new Point(p.x, p.y - 1));
    }
    // Right
    if (p.x + 1 < grid.width()) {
      neighbors.push(new Point(p.x + 1, p.y));
    }
    // Bottom
    if (p.y + 1 < grid.height()) {
      neighbors.push(new Point(p.x, p.y + 1));
    }
    return neighbors;
}

const GridNeighborDiagnol = function(p:Point, grid:Grid) {
  const neighbors = [
    // Left
    new Point(p.x - 1, p.y),
    // Top Left.
    new Point(p.x - 1, p.y - 1),
    // Top.
    new Point(p.x, p.y - 1),
    // Top Right
    new Point(p.x + 1, p.y - 1),
    // Right.
    new Point(p.x + 1, p.y),
    // Bottom Right
    new Point(p.x + 1, p.y + 1),
    // Bottom
    new Point(p.x, p.y + 1),
    // Bottom Left.
    new Point(p.x - 1, p.y + 1),
  ];

  return neighbors.filter((n) => {
    return grid.isValidPoint(n);
  });
}


class Grid {
  private rows:number[][];
  private neighborFunc:(point:Point, grid:Grid) => Point[];

  constructor(neighborFunc:(point:Point, grid:Grid) => Point[] = GridNeighborCardinal) {
    this.rows = [];
    this.neighborFunc = neighborFunc;
  }

  addRow(line:string) {
    const row = [].concat(line.split('').map((num)=> {
      return Number(num);
    }));
    this.rows.push(row);
  }

  width():number {
    if (this.rows.length === 0) {
      return 0;
    }
    return this.rows[0].length;
  }

  height():number {
    return this.rows.length;
  }

  isValidPoint(p:Point):boolean {
    return p.x >= 0 && p.x < this.width() && p.y >= 0 && p.y < this.height();
  }

  get(p:Point):number {
    if (p.x >= this.width() || p.y >= this.height()) {
      return null;
    }
    return this.rows[p.y][p.x];
  }

  set(p:Point, value:number) {
    this.rows[p.y][p.x] = value;
  }

  getNeighbors(p:Point):Point[] {
    return this.neighborFunc(p, this);
  }

  /** @param func first parm is value, second is point. **/
  forEach(func) {
    for(let y = 0; y < this.height(); y++) {
      for(let x = 0; x < this.width(); x++) {
        const point = new Point(x, y);
        const shouldContinue = func(this.get(point), point);
        if (!shouldContinue) {
          return;
        }
      }
    }
  }
}

function AreAllFlashing(grid:Grid):boolean {
  let allFlash = true;
  grid.forEach((val, point) => {
    if (val !== 0) {
      allFlash = false;
      return false;
    }
    return true;
  });
  return allFlash;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines = data.split('\n').filter((line) => {
    return line !== '';
  });

  const lights = new Grid(GridNeighborDiagnol);
  for (let line of lines) {
    lights.addRow(line);
  }

  let steps = 0;
  let flashCount = 0;
  while(true) {
    steps++;
    lights.forEach((val, point) => {
      lights.set(point, val + 1);
      return true;
    });

    let didFlash = true;
    while (didFlash) {
      didFlash = false;
      lights.forEach((val, point) => {
        if (val > 9) {
          flashCount++;
          lights.set(point, -1);
          const neighbors = lights.getNeighbors(point).filter((neighbor) => {
            return lights.get(neighbor) > -1;
          });

          for (let neighbor of neighbors) {
            const neighborVal = lights.get(neighbor);
            lights.set(neighbor, neighborVal + 1);
          }
          didFlash = true;
        }
        return true;
      });
    }

    lights.forEach((val, point) => {
      if (val === -1) {
        lights.set(point, 0);
      }
      return true;
    });
    if (AreAllFlashing(lights)) {
      console.log(steps);
      break;
    }
  }
});
