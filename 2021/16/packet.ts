declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/16/packet.txt';

function HexToBinary(str:string):string {
  let output = '';
  for (let i = 0; i < str.length; i++) {
    let bits = parseInt(str.charAt(i), 16).toString(2);
    while (bits.length < 4) {
      bits = '0' + bits;
    }
    output += bits;
  }
  return output;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines = data.split('\n').filter((line) => {
    return line !== '';
  });

  console.log(HexToBinary(lines[0]));
  console.log(HexToBinary(lines[0]) === '110100101111111000101000');
});
