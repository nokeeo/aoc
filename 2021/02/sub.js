var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/2/sub.txt';
var Sub;
(function (Sub) {
    var Position = /** @class */ (function () {
        function Position() {
            this.x = 0;
            this.y = 0;
            this.a = 0;
        }
        Position.prototype.moveCommand = function (cmd) {
            var values = cmd.split(' ');
            var numValue = Number(values[1]);
            if (values[0] === 'forward') {
                this.x += numValue;
                this.y += this.a * numValue;
            }
            else {
                // Aim correction
                var direction = values[0] === 'down' ? 1 : -1;
                this.a += numValue * direction;
            }
        };
        return Position;
    }());
    fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
        var lines = data.split('\n').filter(function (line) {
            return line != '';
        });
        var pos = new Position();
        for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
            var line = lines_1[_i];
            pos.moveCommand(line);
        }
        console.log(pos.x * pos.y);
    });
})(Sub || (Sub = {}));
