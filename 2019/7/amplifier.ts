declare function require(name:string);
import * as IC from '../IntComputer.js';
import { InputAdapter } from '../InputAdapter.js';
import { OutputAdapter } from '../OutputAdapter.js';

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/7/amplifier.txt';

class Amplifier {
  public phase:number;

  private computer:IC.IntComputer;
  private program:number[];
  private input:InputAdapter;
  private output:OutputAdapter;

  constructor(program:number[]) {
    this.input = new InputAdapter();
    this.output = new OutputAdapter();
    this.computer = new IC.IntComputer();
    this.computer.setInput(this.input);
    this.computer.setOutput(this.output);
    this.program = program;
  }

  readyAmplifier(phase:number) {
    this.computer.loadMemory([...this.program]);
    this.input.setValue(phase);
    this.computer.executeAtPosition(0, () => {});
  }

  amplifyValue(value:number) {
    this.input.setValue(value);
  }

  linkAmplifier(amp:Amplifier) {
    this.output.setCallback((value:number) => {
      amp.amplifyValue(value);
    });
  }

  setOutput(callback:(value:number) => void) {
    this.output.setCallback(callback);
  }
}

function FindPermutations(arr:number[]) : number[][] {
   let res = []
   const helper = (arr2) => {
      if (arr2.length==arr.length)
      return res.push(arr2)
      for(let e of arr)
      if (!arr2.includes(e))
      helper([...arr2, e])
   };
   helper([])
   return res;
};

function RunAmplification(amplifiers:Amplifier[], phases:number[]) {
  for (let i = 0; i < amplifiers.length; i++) {
    const amp = amplifiers[i];
    amp.readyAmplifier(phases[i]);
  }
  amplifiers[0].amplifyValue(0);
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const program:number[] = data.split(',').map((numStr:string) => {
    return Number(numStr);
  });

  let phasePermutations = FindPermutations([9, 8, 7, 6, 5]);

  let maxValue = 0;
  for (let permutation of phasePermutations) {
    const ampA = new Amplifier(program);
    const ampB = new Amplifier(program);
    const ampC = new Amplifier(program);
    const ampD = new Amplifier(program);
    const ampE = new Amplifier(program);

    ampA.linkAmplifier(ampB);
    ampB.linkAmplifier(ampC);
    ampC.linkAmplifier(ampD);
    ampD.linkAmplifier(ampE);

    ampA.readyAmplifier(permutation[0]);
    ampB.readyAmplifier(permutation[1]);
    ampC.readyAmplifier(permutation[2]);
    ampD.readyAmplifier(permutation[3]);
    ampE.readyAmplifier(permutation[4]);

    let loopCounter = 0;
    ampE.setOutput((value:number) => {
      loopCounter++;
      maxValue = Math.max(value, maxValue);
      ampA.amplifyValue(value);
    });

    ampA.amplifyValue(0);
  }

  console.log(maxValue);
});
