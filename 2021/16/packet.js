var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/16/packet.txt';
function HexToBinary(str) {
    var output = '';
    for (var i = 0; i < str.length; i++) {
        var bits = parseInt(str.charAt(i), 16).toString(2);
        while (bits.length < 4) {
            bits = '0' + bits;
        }
        output += bits;
    }
    return output;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line !== '';
    });
    console.log(HexToBinary(lines[0]));
    console.log(HexToBinary(lines[0]) === '110100101111111000101000');
});
