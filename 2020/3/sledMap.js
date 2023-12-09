var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode2020/3/sledMap.txt';
var Row = /** @class */ (function () {
    function Row(pattern) {
        this.pattern = pattern;
    }
    Row.prototype.isColumnOpen = function (index) {
        var patternIndex = index % this.pattern.length;
        var char = this.pattern[patternIndex];
        return char == '.';
    };
    return Row;
}());
function GetTreeCount(right, down, rows) {
    var index = 0;
    var treeCount = 0;
    for (var i = 0; i < rows.length; i += down) {
        var row = rows[i];
        if (!row.isColumnOpen(index)) {
            treeCount++;
        }
        index += right;
    }
    return treeCount;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var rows = data.split('\n').filter(function (line) {
        return line != '';
    }).map(function (line) {
        return new Row(line);
    });
    var run1 = GetTreeCount(1, 1, rows);
    var run2 = GetTreeCount(3, 1, rows);
    var run3 = GetTreeCount(5, 1, rows);
    var run4 = GetTreeCount(7, 1, rows);
    var run5 = GetTreeCount(1, 2, rows);
    console.log(run1 * run2 * run3 * run4 * run5);
});
