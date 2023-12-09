var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/7/crab.txt';
function oldCost(crabs, num) {
    return crabs.reduce(function (acc, crab) {
        return acc + Math.abs(num - crab);
    }, 0);
}
function newCost(crabs, num) {
    return crabs.reduce(function (acc, crab) {
        var diff = Math.abs(num - crab);
        return acc + (diff / 2 * (1 + diff));
    }, 0);
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var crabs = data.split(',').map(function (item) {
        return Number(item.trim());
    }).sort(function (a, b) {
        return a - b;
    });
    var current = crabs[Math.floor(crabs.length / 2)];
    var currentCost = newCost(crabs, current);
    var above = current + 1;
    var aboveCost = newCost(crabs, above);
    var below = current - 1;
    var belowCost = newCost(crabs, below);
    var direction = currentCost > aboveCost ? 1 : -1;
    var next = current;
    var nextCost = currentCost;
    while (current <= crabs[crabs.length - 1]) {
        current = current + direction;
        currentCost = newCost(crabs, current);
        if (currentCost <= nextCost) {
            nextCost = currentCost;
            next = current;
        }
    }
    console.log(next + " " + nextCost);
});
