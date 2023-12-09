var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/11/light.txt';
var Point = /** @class */ (function () {
    function Point(x, y) {
        this.x = x;
        this.y = y;
    }
    Point.prototype.toString = function () {
        return this.x + "," + this.y;
    };
    return Point;
}());
var GridNeighborCardinal = function (p, grid) {
    var neighbors = [];
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
};
var GridNeighborDiagnol = function (p, grid) {
    var neighbors = [
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
    return neighbors.filter(function (n) {
        return grid.isValidPoint(n);
    });
};
var Grid = /** @class */ (function () {
    function Grid(neighborFunc) {
        if (neighborFunc === void 0) { neighborFunc = GridNeighborCardinal; }
        this.rows = [];
        this.neighborFunc = neighborFunc;
    }
    Grid.prototype.addRow = function (line) {
        var row = [].concat(line.split('').map(function (num) {
            return Number(num);
        }));
        this.rows.push(row);
    };
    Grid.prototype.width = function () {
        if (this.rows.length === 0) {
            return 0;
        }
        return this.rows[0].length;
    };
    Grid.prototype.height = function () {
        return this.rows.length;
    };
    Grid.prototype.isValidPoint = function (p) {
        return p.x >= 0 && p.x < this.width() && p.y >= 0 && p.y < this.height();
    };
    Grid.prototype.get = function (p) {
        if (p.x >= this.width() || p.y >= this.height()) {
            return null;
        }
        return this.rows[p.y][p.x];
    };
    Grid.prototype.set = function (p, value) {
        this.rows[p.y][p.x] = value;
    };
    Grid.prototype.getNeighbors = function (p) {
        return this.neighborFunc(p, this);
    };
    /** @param func first parm is value, second is point. **/
    Grid.prototype.forEach = function (func) {
        for (var y = 0; y < this.height(); y++) {
            for (var x = 0; x < this.width(); x++) {
                var point = new Point(x, y);
                var shouldContinue = func(this.get(point), point);
                if (!shouldContinue) {
                    return;
                }
            }
        }
    };
    return Grid;
}());
function AreAllFlashing(grid) {
    var allFlash = true;
    grid.forEach(function (val, point) {
        if (val !== 0) {
            allFlash = false;
            return false;
        }
        return true;
    });
    return allFlash;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line !== '';
    });
    var lights = new Grid(GridNeighborDiagnol);
    for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
        var line = lines_1[_i];
        lights.addRow(line);
    }
    var steps = 0;
    var flashCount = 0;
    var _loop_1 = function () {
        steps++;
        lights.forEach(function (val, point) {
            lights.set(point, val + 1);
            return true;
        });
        var didFlash = true;
        while (didFlash) {
            didFlash = false;
            lights.forEach(function (val, point) {
                if (val > 9) {
                    flashCount++;
                    lights.set(point, -1);
                    var neighbors = lights.getNeighbors(point).filter(function (neighbor) {
                        return lights.get(neighbor) > -1;
                    });
                    for (var _i = 0, neighbors_1 = neighbors; _i < neighbors_1.length; _i++) {
                        var neighbor = neighbors_1[_i];
                        var neighborVal = lights.get(neighbor);
                        lights.set(neighbor, neighborVal + 1);
                    }
                    didFlash = true;
                }
                return true;
            });
        }
        lights.forEach(function (val, point) {
            if (val === -1) {
                lights.set(point, 0);
            }
            return true;
        });
        if (AreAllFlashing(lights)) {
            console.log(steps);
            return "break";
        }
    };
    while (true) {
        var state_1 = _loop_1();
        if (state_1 === "break")
            break;
    }
});
