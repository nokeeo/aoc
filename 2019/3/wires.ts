declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/3/wires.txt';

class Point {
  public x:number;
  public y:number;

  constructor(x:number, y:number) {
    this.x = x;
    this.y = y;
  }

  toString() : string {
    return '(' + String(this.x) + ',' + String(this.y) + ')';
  }
}

class Line {
  public start:Point;
  public end:Point;

  constructor(start:Point, end:Point) {
    this.start = start;
    this.end = end;
  }

  maxY() : number {
    return Math.max(this.start.y, this.end.y);
  }

  minY() : number {
    return Math.min(this.start.y, this.end.y);
  }

  maxX() : number {
    return Math.max(this.start.x, this.end.x);
  }

  minX() : number {
    return Math.min(this.start.x, this.end.x);
  }

  isPointOnLine(point:Point) : boolean {
    const x = point.x >= this.minX() && point.x <= this.maxX();
    const y = point.y >= this.minY() && point.y <= this.maxY();
    return x && y;
  }

  length() : number {
    return Math.abs(this.maxX() - this.minX()) + Math.abs(this.maxY() - this.minY());
  }

  /** Assumes point is on the line. **/
  lengthToPoint(point:Point) : number {
    return Math.abs(this.start.x - point.x) + Math.abs(this.start.y - point.y);
  }
}

function GetDeltaX(op:string) {
  const direction = op[0];
  if (direction === 'R') {
    return Number(op.slice(1));
  } else if (direction === 'L') {
    return -1 * Number(op.slice(1));
  }

  return 0;
}

function GetDeltaY(op:string) {
  const direction = op[0];
  if (direction === 'U') {
    return Number(op.slice(1));
  } else if (direction === 'D') {
    return -1 * Number(op.slice(1));
  }

  return 0;
}

function ParseWire(line:string) : Line[] {
  const lines:Line[] = [];

  const ops:string[] = line.split(',');
  let point = new Point(0, 0);
  for(let op of ops) {
    let deltaX = GetDeltaX(op);
    let deltaY = GetDeltaY(op);

    const newPoint = new Point(point.x + deltaX, point.y + deltaY);
    const line = new Line(point, newPoint);
    lines.push(line);
    point = newPoint;
  }

  return lines;
}

function LineSort(a:Line, b:Line) {
  return a.minX() - b.minX();
}

function IsVerticalLine(line:Line) : boolean {
  return line.start.x === line.end.x;
}

function GetVerticalLine(line1:Line, line2:Line) : Line {
  let verticalLine:Line = null;
  if (IsVerticalLine(line1)) {
    verticalLine = line1;
  }

  if (IsVerticalLine(line2)) {
    if (verticalLine !== null) {
      return null;
    }
    verticalLine = line2;
  }

  return verticalLine;
}

function CalculateWireIntersections(wire1:Line[], wire2:Line[]) : Point[] {
  const intersections:Point[] = [];
  for (let line1 of wire1) {
    for (let line2 of wire2) {
      if (line2.start.x > line1.end.x) {
        break;
      }

      const verticalLine:Line = GetVerticalLine(line1, line2);
      if (verticalLine === null) {
        continue;
      }

      const horizontalLine:Line = (verticalLine === line1) ? line2 : line1;
      const verticalX = verticalLine.start.x;
      const horizontalY = horizontalLine.start.y;

      const xCheck = verticalX >= horizontalLine.minX() && verticalX <= horizontalLine.maxX();
      const yCheck = horizontalY >= verticalLine.minY() && horizontalY <= verticalLine.maxY();
      if(xCheck && yCheck) {
        intersections.push(new Point(verticalX, horizontalLine.start.y));
      }
    }
  }

  return intersections;
}

function CalculateMinDistance(wire1:Line[], wire2:Line[]) : number {
  const intersections:Point[] = CalculateWireIntersections([...wire1].sort(LineSort), [...wire2].sort(LineSort));
  let minDistance:number = Number.MAX_VALUE;
  for (let intersection of intersections) {
    if (intersection.x === 0 && intersection.y === 0) {
      continue;
    }
    console.log(intersection.toString());
    const distance = Math.abs(intersection.x) + Math.abs(intersection.y);
    minDistance = Math.min(distance, minDistance);
  }

  return minDistance;
}

function WireDistanceToPoint(wire:Line[], point:Point) : number {
  let distance:number = 0;
  for (let line of wire) {
    if (line.isPointOnLine(point)) {
      return distance + line.lengthToPoint(point);
    } else {
      distance += line.length();
    }
  }

  return distance;
}

function MinSignalDelay(wire1:Line[], wire2:Line[]) : number {
  const intersections:Point[] = CalculateWireIntersections([...wire1].sort(LineSort), [...wire2].sort(LineSort));
  let minDistance:number = Number.MAX_VALUE;
  for (let intersection of intersections) {
    const distance = WireDistanceToPoint(wire1, intersection) + WireDistanceToPoint(wire2, intersection);
    minDistance = Math.min(distance, minDistance);
  }

  return minDistance;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  const wire1:Line[] = ParseWire(lines[0]);
  const wire2:Line[] = ParseWire(lines[1]);
  console.log(MinSignalDelay(wire1, wire2));
});
