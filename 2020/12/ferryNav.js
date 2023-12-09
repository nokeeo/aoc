var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/12/ferryNav.txt';
var Ferry = /** @class */ (function () {
    function Ferry() {
        this.wayPointX = 10;
        this.wayPointY = 1;
        this.x = 0;
        this.y = 0;
    }
    Ferry.prototype.navigate = function (cmd, value) {
        if (cmd === 'N') {
            this.wayPointY += value;
        }
        else if (cmd === 'S') {
            this.wayPointY -= value;
        }
        else if (cmd === 'E') {
            this.wayPointX += value;
        }
        else if (cmd === 'W') {
            this.wayPointX -= value;
        }
        else if (cmd === 'L') {
            value *= Math.PI / 180;
            var x = this.wayPointX;
            var y = this.wayPointY;
            this.wayPointX = (x * Math.cos(value)) - (y * Math.sin(value));
            this.wayPointY = (x * Math.sin(value)) + (y * Math.cos(value));
        }
        else if (cmd === 'R') {
            value *= Math.PI / -180;
            var x = this.wayPointX;
            var y = this.wayPointY;
            this.wayPointX = (x * Math.cos(value)) - (y * Math.sin(value));
            this.wayPointY = (x * Math.sin(value)) + (y * Math.cos(value));
        }
        else if (cmd === 'F') {
            this.forward(value);
        }
    };
    Ferry.prototype.forward = function (value) {
        this.x += this.wayPointX * value;
        this.y += this.wayPointY * value;
    };
    return Ferry;
}());
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line !== '';
    });
    var ferry = new Ferry();
    for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
        var line = lines_1[_i];
        var cmd = line.slice(0, 1);
        var value = Number(line.slice(1, line.length));
        ferry.navigate(cmd, value);
        console.log(ferry);
    }
    console.log(ferry);
    console.log(Math.abs(ferry.x) + Math.abs(ferry.y));
});
