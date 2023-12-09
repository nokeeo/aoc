declare function require(name:string);
import * as IC from '../IntComputer.js';
import { InputAdapter } from '../InputAdapter.js';
import { OutputAdapter } from '../OutputAdapter.js';

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/5/test.txt';

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const memory:number[] = data.split(',').map((opStr:string) => {
    return Number(opStr);
  });

  const computer = new IC.IntComputer();
  const input:InputAdapter = new InputAdapter();
  const output:OutputAdapter = new OutputAdapter();
  input.setValue(5);
  computer.setInput(input as IC.Input);
  computer.setOutput(output);
  computer.loadMemory(memory);
  computer.executeAtPosition(0, () => {});

  output.setCallback((value:number) => {
    console.log(value);
  });
});
