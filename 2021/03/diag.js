var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/3/diag.txt';
function FindOxNumber(lines) {
    var _loop_1 = function (i) {
        var oneCount = 0;
        for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
            var line = lines_1[_i];
            if (line.charAt(i) === '1') {
                oneCount++;
            }
        }
        var filterValue = oneCount >= (lines.length / 2) ? '1' : '0';
        lines = lines.filter(function (line) {
            return line.charAt(i) !== filterValue;
        });
        if (lines.length === 1) {
            return { value: parseInt(lines[0], 2) };
        }
    };
    for (var i = 0; i < lines[0].length; i++) {
        var state_1 = _loop_1(i);
        if (typeof state_1 === "object")
            return state_1.value;
    }
    return 0;
}
function FindCo2Number(lines) {
    var _loop_2 = function (i) {
        var oneCount = 0;
        for (var _i = 0, lines_2 = lines; _i < lines_2.length; _i++) {
            var line = lines_2[_i];
            if (line.charAt(i) === '1') {
                oneCount++;
            }
        }
        var filterValue = oneCount >= (lines.length / 2) ? '1' : '0';
        lines = lines.filter(function (line) {
            return line.charAt(i) === filterValue;
        });
        if (lines.length === 1) {
            return { value: parseInt(lines[0], 2) };
        }
    };
    for (var i = 0; i < lines[0].length; i++) {
        var state_2 = _loop_2(i);
        if (typeof state_2 === "object")
            return state_2.value;
    }
    return 0;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line != '';
    });
    // let gama = '';
    // let epsilon = '';
    //
    // const bitLength = lines[0].length;
    // for (let i = 0; i < bitLength; i++) {
    //   let oneCount = 0;
    //   for (let line of lines) {
    //     if (line.charAt(i) === '1') {
    //       oneCount++;
    //     }
    //   }
    //
    //   if (oneCount > (lines.length / 2)) {
    //     gama += '1';
    //     epsilon += '0';
    //   } else {
    //     gama += '0';
    //     epsilon += '1';
    //   }
    // }
    //
    // console.log(gama);
    // console.log(epsilon);
    // const gamaDec = parseInt(gama, 2);
    // const epsilonDec = parseInt(epsilon, 2);
    //
    // console.log(gamaDec * epsilonDec);
    console.log(FindOxNumber(lines) * FindCo2Number(lines));
});
