var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/9/xmas.txt';
function MinMax(numbers) {
    var min = Number.MAX_VALUE;
    var max = Number.MIN_VALUE;
    for (var _i = 0, numbers_1 = numbers; _i < numbers_1.length; _i++) {
        var number = numbers_1[_i];
        min = Math.min(min, number);
        max = Math.max(max, number);
    }
    return min + max;
}
function IsNumberSumOfTwoNumbers(num, numbers) {
    for (var i = 0; i < numbers.length; i++) {
        for (var j = i + 1; j < numbers.length; j++) {
            if (numbers[i] + numbers[j] === num) {
                return true;
            }
        }
    }
    return false;
}
function GetEncryptionWeaknes(num, numbers) {
    var index1 = 0;
    var index2 = 1;
    while (index1 < numbers.length - 1 && index2 < numbers.length - 1) {
        var sum = numbers[index1];
        while (sum <= num) {
            var slice = numbers.slice(index1, index2 + 1);
            sum = slice.reduce(function (acc, value) {
                return acc + value;
            });
            if (sum === num) {
                return MinMax(slice);
            }
            index2++;
        }
        index1++;
        index2 = index1 + 1;
    }
    return 0;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var numbers = data.split('\n').filter(function (line) {
        return line !== '';
    }).map(function (line) {
        return Number(line);
    });
    var preambleLength = 25;
    for (var i = preambleLength; i < numbers.length; i++) {
        var num = numbers[i];
        var preamble = numbers.slice(i - preambleLength, i);
        if (!IsNumberSumOfTwoNumbers(num, preamble)) {
            console.log(GetEncryptionWeaknes(num, numbers));
            break;
        }
    }
});
