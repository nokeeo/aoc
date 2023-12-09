var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2019/1/moduleMass.txt';
function CalculateFuelRequirement(mass) {
    var fuel = Math.max(0, Math.floor(mass / 3) - 2);
    if (fuel != 0) {
        fuel += CalculateFuelRequirement(fuel);
    }
    return fuel;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line != '';
    });
    var fuelSum = 0;
    for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
        var line = lines_1[_i];
        fuelSum += CalculateFuelRequirement(Number(line));
    }
    console.log(fuelSum);
});
