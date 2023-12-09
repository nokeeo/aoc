declare function require(name:string);
import * as IC from '../IntComputer.js';


const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/2/intCodeAdd.txt';

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const memory:number[] = data.split(',').map((opString:string) => {
    return Number(opString);
  });

  const computer = new IC.IntComputer();
  for (let i = 0; i < 100; i++) {
    for (let j = 0; j < 100; j++) {
      computer.loadMemory([...memory]);
      computer.memory.assignValueAtPosition(1, i);
      computer.memory.assignValueAtPosition(2, j);
      computer.executeAtPosition(0, () => {
        if (computer.memory.valueAtPosition(0) === 19690720) {
          console.log(100 * i + j);
          return;
        }
      });
    }
  }
});
