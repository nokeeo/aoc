var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode2020/5/boardingPass.txt';
var BinaryRange = /** @class */ (function () {
    function BinaryRange(min, max) {
        this.min = min;
        this.max = max;
    }
    BinaryRange.prototype.subdivideFront = function () {
        this.max -= this.getHalfElementCount();
    };
    BinaryRange.prototype.subdivideBack = function () {
        this.min += this.getHalfElementCount();
    };
    BinaryRange.prototype.getHalfElementCount = function () {
        return Math.ceil((this.max - this.min) / 2);
    };
    return BinaryRange;
}());
function GetIDForBoardingPass(pass) {
    var rowRange = new BinaryRange(1, 128);
    var columnRange = new BinaryRange(1, 8);
    for (var i = 0; i < pass.length; i++) {
        var char = pass[i];
        if (char === 'F') {
            rowRange.subdivideFront();
        }
        else if (char === 'B') {
            rowRange.subdivideBack();
        }
        else if (char === 'L') {
            columnRange.subdivideFront();
        }
        else if (char === 'R') {
            columnRange.subdivideBack();
        }
    }
    return (rowRange.min - 1) * 8 + (columnRange.min - 1);
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line != '';
    });
    var boardingPassIDs = [];
    for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
        var line = lines_1[_i];
        boardingPassIDs.push(GetIDForBoardingPass(line));
    }
    boardingPassIDs.sort(function (a, b) {
        return a - b;
    });
    for (var i = 0; i < boardingPassIDs.length - 1; i++) {
        var currentPass = boardingPassIDs[i];
        var expectedNextPassID = currentPass + 1;
        if (expectedNextPassID != boardingPassIDs[i + 1]) {
            console.log(expectedNextPassID);
            break;
        }
    }
});
