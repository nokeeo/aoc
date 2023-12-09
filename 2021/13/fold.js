var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/13/fold.txt';
var Dot = /** @class */ (function () {
    function Dot(x, y) {
        this.x = x;
        this.y = y;
    }
    Dot.prototype.toString = function () {
        return this.x + "," + this.y;
    };
    Dot.prototype.add = function (dot) {
        return new Dot(this.x + dot.x, this.y + dot.y);
    };
    Dot.prototype.subtract = function (dot) {
        return new Dot(this.x - dot.x, this.y - dot.y);
    };
    Dot.prototype.divide = function (denom) {
        var x = (denom.x === 0) ? 0 : this.x / denom.x;
        var y = (denom.y === 0) ? 0 : this.y / denom.y;
        return new Dot(x, y);
    };
    Dot.prototype.mult = function (dot) {
        return new Dot(this.x * dot.x, this.y * dot.y);
    };
    return Dot;
}());
var DotCollection = /** @class */ (function () {
    function DotCollection() {
        this.maxX = Number.MIN_VALUE;
        this.maxY = Number.MIN_VALUE;
        this.dots = {};
    }
    DotCollection.prototype.add = function (dot) {
        this.maxX = Math.max(this.maxX, dot.x);
        this.maxY = Math.max(this.maxY, dot.y);
        this.dots[dot.toString()] = dot;
    };
    DotCollection.prototype.forEach = function (func) {
        for (var _i = 0, _a = Object.keys(this.dots); _i < _a.length; _i++) {
            var key = _a[_i];
            if (!func(this.dots[key])) {
                break;
            }
        }
    };
    DotCollection.prototype.size = function () {
        return Object.keys(this.dots).length;
    };
    DotCollection.prototype.toString = function () {
        var _this = this;
        var dots = Object.keys(this.dots).map(function (key) {
            return _this.dots[key];
        });
        dots.sort(function (a, b) {
            if (a.y === b.y) {
                return a.x - b.x;
            }
            return a.y - b.y;
        });
        var str = '';
        var dot = dots.shift();
        for (var y = 0; y <= this.maxY; y++) {
            for (var x = 0; x <= this.maxX; x++) {
                if (dot.x === x && dot.y === y) {
                    str += '#';
                    dot = dots.shift();
                }
                else {
                    str += '.';
                }
            }
            if (y !== this.maxY) {
                str += '\n';
            }
        }
        return str;
    };
    return DotCollection;
}());
var Input = /** @class */ (function () {
    function Input() {
        this.dots = new DotCollection();
        this.instructions = [];
    }
    return Input;
}());
function ParseLines(lines) {
    var input = new Input();
    // Parse dots.
    var i = 0;
    while (i < lines.length) {
        var line = lines[i];
        i++;
        if (line === '') {
            break;
        }
        var parts = line.split(',');
        input.dots.add(new Dot(Number(parts[0]), Number(parts[1])));
    }
    for (var k = i; k < lines.length; k++) {
        var line = lines[k];
        if (line === '') {
            continue;
        }
        var parts = line.split(' ');
        var directionValue = parts[2].split('=');
        var x = directionValue[0] === 'x' ? Number(directionValue[1]) : 0;
        var y = directionValue[0] === 'y' ? Number(directionValue[1]) : 0;
        input.instructions.push(new Dot(x, y));
    }
    return input;
}
function Fold(dots, fold) {
    var newCollection = new DotCollection();
    dots.forEach(function (dot) {
        if (dot.x < fold.x || dot.y < fold.y) {
            newCollection.add(dot);
        }
        else {
            var unit = fold.divide(fold);
            var diff = dot.subtract(fold).mult(unit);
            var offset = dot.mult(new Dot(unit.y, unit.x));
            var folded = fold.add(offset).subtract(diff);
            newCollection.add(folded);
        }
        return true;
    });
    return newCollection;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n');
    var input = ParseLines(lines);
    var collection = input.dots;
    for (var _i = 0, _a = input.instructions; _i < _a.length; _i++) {
        var fold = _a[_i];
        collection = Fold(collection, fold);
    }
    console.log(collection.toString());
});
