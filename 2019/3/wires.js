var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2019/3/wires.txt';
var Point = /** @class */ (function () {
    function Point(x, y) {
        this.x = x;
        this.y = y;
    }
    Point.prototype.toString = function () {
        return '(' + String(this.x) + ',' + String(this.y) + ')';
    };
    return Point;
}());
var Line = /** @class */ (function () {
    function Line(start, end) {
        this.start = start;
        this.end = end;
    }
    Line.prototype.maxY = function () {
        return Math.max(this.start.y, this.end.y);
    };
    Line.prototype.minY = function () {
        return Math.min(this.start.y, this.end.y);
    };
    Line.prototype.maxX = function () {
        return Math.max(this.start.x, this.end.x);
    };
    Line.prototype.minX = function () {
        return Math.min(this.start.x, this.end.x);
    };
    Line.prototype.isPointOnLine = function (point) {
        var x = point.x >= this.minX() && point.x <= this.maxX();
        var y = point.y >= this.minY() && point.y <= this.maxY();
        return x && y;
    };
    Line.prototype.length = function () {
        return Math.abs(this.maxX() - this.minX()) + Math.abs(this.maxY() - this.minY());
    };
    /** Assumes point is on the line. **/
    Line.prototype.lengthToPoint = function (point) {
        return Math.abs(this.start.x - point.x) + Math.abs(this.start.y - point.y);
    };
    return Line;
}());
function GetDeltaX(op) {
    var direction = op[0];
    if (direction === 'R') {
        return Number(op.slice(1));
    }
    else if (direction === 'L') {
        return -1 * Number(op.slice(1));
    }
    return 0;
}
function GetDeltaY(op) {
    var direction = op[0];
    if (direction === 'U') {
        return Number(op.slice(1));
    }
    else if (direction === 'D') {
        return -1 * Number(op.slice(1));
    }
    return 0;
}
function ParseWire(line) {
    var lines = [];
    var ops = line.split(',');
    var point = new Point(0, 0);
    for (var _i = 0, ops_1 = ops; _i < ops_1.length; _i++) {
        var op = ops_1[_i];
        var deltaX = GetDeltaX(op);
        var deltaY = GetDeltaY(op);
        var newPoint = new Point(point.x + deltaX, point.y + deltaY);
        var line_1 = new Line(point, newPoint);
        lines.push(line_1);
        point = newPoint;
    }
    return lines;
}
function LineSort(a, b) {
    return a.minX() - b.minX();
}
function IsVerticalLine(line) {
    return line.start.x === line.end.x;
}
function GetVerticalLine(line1, line2) {
    var verticalLine = null;
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
function CalculateWireIntersections(wire1, wire2) {
    var intersections = [];
    for (var _i = 0, wire1_1 = wire1; _i < wire1_1.length; _i++) {
        var line1 = wire1_1[_i];
        for (var _a = 0, wire2_1 = wire2; _a < wire2_1.length; _a++) {
            var line2 = wire2_1[_a];
            if (line2.start.x > line1.end.x) {
                break;
            }
            var verticalLine = GetVerticalLine(line1, line2);
            if (verticalLine === null) {
                continue;
            }
            var horizontalLine = (verticalLine === line1) ? line2 : line1;
            var verticalX = verticalLine.start.x;
            var horizontalY = horizontalLine.start.y;
            var xCheck = verticalX >= horizontalLine.minX() && verticalX <= horizontalLine.maxX();
            var yCheck = horizontalY >= verticalLine.minY() && horizontalY <= verticalLine.maxY();
            if (xCheck && yCheck) {
                intersections.push(new Point(verticalX, horizontalLine.start.y));
            }
        }
    }
    return intersections;
}
function CalculateMinDistance(wire1, wire2) {
    var intersections = CalculateWireIntersections(wire1.slice().sort(LineSort), wire2.slice().sort(LineSort));
    var minDistance = Number.MAX_VALUE;
    for (var _i = 0, intersections_1 = intersections; _i < intersections_1.length; _i++) {
        var intersection = intersections_1[_i];
        if (intersection.x === 0 && intersection.y === 0) {
            continue;
        }
        console.log(intersection.toString());
        var distance = Math.abs(intersection.x) + Math.abs(intersection.y);
        minDistance = Math.min(distance, minDistance);
    }
    return minDistance;
}
function WireDistanceToPoint(wire, point) {
    var distance = 0;
    for (var _i = 0, wire_1 = wire; _i < wire_1.length; _i++) {
        var line = wire_1[_i];
        if (line.isPointOnLine(point)) {
            return distance + line.lengthToPoint(point);
        }
        else {
            distance += line.length();
        }
    }
    return distance;
}
function MinSignalDelay(wire1, wire2) {
    var intersections = CalculateWireIntersections(wire1.slice().sort(LineSort), wire2.slice().sort(LineSort));
    var minDistance = Number.MAX_VALUE;
    for (var _i = 0, intersections_2 = intersections; _i < intersections_2.length; _i++) {
        var intersection = intersections_2[_i];
        var distance = WireDistanceToPoint(wire1, intersection) + WireDistanceToPoint(wire2, intersection);
        minDistance = Math.min(distance, minDistance);
    }
    return minDistance;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line != '';
    });
    var wire1 = ParseWire(lines[0]);
    var wire2 = ParseWire(lines[1]);
    console.log(MinSignalDelay(wire1, wire2));
});
