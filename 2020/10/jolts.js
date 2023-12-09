var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/10/jolts.txt';
function GetJoltDifferences(jolts) {
    var oneDiff = 0;
    var threeDiff = 1; // Your device rating.
    for (var i = 0; i < jolts.length; i++) {
        var diff = 0;
        if (i === 0) {
            diff = jolts[i];
        }
        else {
            diff = jolts[i] - jolts[i - 1];
        }
        if (diff === 1) {
            oneDiff++;
        }
        else if (diff === 3) {
            threeDiff++;
        }
    }
    return oneDiff * threeDiff;
}
function GetTotalCombinations(adapters, index, cache) {
    var sum = 0;
    var currentValue = adapters[index];
    if (cache[currentValue]) {
        return cache[currentValue];
    }
    for (var i = index + 1; i <= adapters.length; i++) {
        if (i === adapters.length) {
            sum += 1;
            continue;
        }
        var nextValue = adapters[i];
        var diff = nextValue - currentValue;
        if (diff > 0 && diff < 4) {
            sum += GetTotalCombinations(adapters, i, cache);
        }
        else {
            break;
        }
    }
    cache[currentValue] = sum;
    return sum;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var numbers = data.split('\n').filter(function (line) {
        return line !== '';
    }).map(function (line) {
        return Number(line);
    }).sort(function (a, b) {
        return a - b;
    });
    // numbers.unshift(0);
    console.log(GetTotalCombinations(numbers, 0, {}));
});
