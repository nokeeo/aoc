var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/1/increase.txt';
function increaseCount(numbers) {
    var increased = 0;
    for (var i = 1; i < numbers.length; i++) {
        var before = numbers[i - 1];
        var current = numbers[i];
        if (current > before) {
            increased++;
        }
    }
    return increased;
}
function slidingIncreaseCount(numbers) {
    var increased = 0;
    var windowSize = 3;
    var lastSum = 0;
    for (var i = 0; i < windowSize && numbers.length; i++) {
        lastSum += numbers[i];
    }
    for (var i = windowSize; i < numbers.length; i++) {
        var currentSum = lastSum - numbers[i - windowSize] + numbers[i];
        if (currentSum > lastSum) {
            increased++;
        }
        lastSum = currentSum;
    }
    return increased;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var numbers = data.split('\n').map(function (line) {
        return Number(line);
    });
    console.log(slidingIncreaseCount(numbers));
});
