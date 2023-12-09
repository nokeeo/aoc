var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode2020/1/reportRepair.txt';
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var numbers = data.split('\n').map(function (line) {
        return Number(line);
    }).sort(function (a, b) {
        return a - b;
    }).filter(function (a) {
        return a != 0;
    });
    // for (let i = 0; i < numbers.length; i++) {
    //   for (let j = numbers.length - 1; j > i; j--) {
    //     const sum = numbers[i] + numbers[j];
    //     if (sum == 2020) {
    //       console.log(numbers[i] * numbers[j]);
    //       return;
    //     } else if (sum < 2020) {
    //       break;
    //     }
    //   }
    // }
    for (var i = 0; i < numbers.length; i++) {
        for (var j = 0; j < numbers.length; j++) {
            for (var k = numbers.length - 1; (k > j && k > i); k--) {
                var sum = numbers[i] + numbers[j] + numbers[k];
                if (sum == 2020) {
                    console.log(numbers[i] * numbers[j] * numbers[k]);
                    return;
                }
                else if (sum < 2020) {
                    break;
                }
            }
        }
    }
});
