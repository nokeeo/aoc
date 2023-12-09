var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/4/bingo.txt';
function SetFromArray(arr) {
    var set = new Set();
    for (var _i = 0, arr_1 = arr; _i < arr_1.length; _i++) {
        var element = arr_1[_i];
        set.add(element);
    }
    return set;
}
function Intersection(setA, setB) {
    var _intersection = new Set();
    setB.forEach(function (elem) {
        if (setA.has(elem)) {
            _intersection.add(elem);
        }
    });
    return _intersection;
}
function Union(setA, setB) {
    var union = new Set();
    setA.forEach(function (elem) {
        union.add(elem);
    });
    setB.forEach(function (elem) {
        union.add(elem);
    });
    return union;
}
function SetDiff(setA, setB) {
    var diff = new Set();
    setA.forEach(function (elem) {
        if (!setB.has(elem)) {
            diff.add(elem);
        }
    });
    return diff;
}
var Board = /** @class */ (function () {
    function Board(rows) {
        this.rowSets = [];
        this.colSets = [];
        var boardArray = [];
        for (var _i = 0, rows_1 = rows; _i < rows_1.length; _i++) {
            var row = rows_1[_i];
            boardArray.push(row.split(' ').filter(function (element) {
                return element != '';
            }).map(function (element) {
                return Number(element);
            }));
        }
        // Parse rows
        for (var _a = 0, boardArray_1 = boardArray; _a < boardArray_1.length; _a++) {
            var row = boardArray_1[_a];
            this.rowSets.push(SetFromArray(row));
        }
        // Parse columns
        for (var i = 0; i < boardArray[0].length; i++) {
            var set = new Set();
            for (var _b = 0, boardArray_2 = boardArray; _b < boardArray_2.length; _b++) {
                var row = boardArray_2[_b];
                set.add(row[i]);
            }
            this.colSets.push(set);
        }
    }
    Board.prototype.isWinner = function (nums) {
        for (var _i = 0, _a = this.rowSets; _i < _a.length; _i++) {
            var row = _a[_i];
            if (Intersection(row, nums).size === row.size) {
                return true;
            }
        }
        for (var _b = 0, _c = this.colSets; _b < _c.length; _b++) {
            var col = _c[_b];
            if (Intersection(col, nums).size === col.size) {
                return true;
            }
        }
        return false;
    };
    Board.prototype.sumUnmarked = function (marked) {
        var unmarked = new Set();
        for (var _i = 0, _a = this.rowSets; _i < _a.length; _i++) {
            var row = _a[_i];
            unmarked = Union(unmarked, SetDiff(row, marked));
        }
        var sum = 0;
        unmarked.forEach(function (elem) {
            sum += elem;
        });
        return sum;
    };
    return Board;
}());
function ParseNextBoard(lines) {
    var nextLine = lines.shift();
    var boardLines = [];
    while (nextLine != '') {
        boardLines.push(nextLine);
        nextLine = lines.shift();
    }
    if (boardLines.length > 0) {
        return new Board(boardLines);
    }
    return null;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n');
    var draws = lines.shift().split(',').map(function (draw) {
        return Number(draw);
    });
    lines.shift(); // Shift off extra newline.
    var boards = [];
    while (lines.length > 0) {
        boards.push(ParseNextBoard(lines));
    }
    var currentDraws = new Set();
    var winners = new Set();
    var lastWinner = null;
    var num = 0;
    while (draws.length > 0) {
        num = draws.shift();
        currentDraws.add(num);
        for (var _i = 0, boards_1 = boards; _i < boards_1.length; _i++) {
            var board = boards_1[_i];
            if (!winners.has(board) && board.isWinner(currentDraws)) {
                lastWinner = board;
                winners.add(board);
            }
        }
        if (winners.size === boards.length) {
            break;
        }
    }
    console.log(lastWinner.sumUnmarked(currentDraws) * num);
});
