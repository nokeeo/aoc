"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const IC = require("../IntComputer.js");
const InputAdapter_js_1 = require("../InputAdapter.js");
const OutputAdapter_js_1 = require("../OutputAdapter.js");
const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2019/7/amplifier.txt';
class Amplifier {
    constructor(program) {
        this.input = new InputAdapter_js_1.InputAdapter();
        this.output = new OutputAdapter_js_1.OutputAdapter();
        this.computer = new IC.IntComputer();
        this.computer.setInput(this.input);
        this.computer.setOutput(this.output);
        this.program = program;
    }
    readyAmplifier(phase) {
        this.computer.loadMemory([...this.program]);
        this.input.setValue(phase);
        this.computer.executeAtPosition(0, () => { });
    }
    amplifyValue(value) {
        this.input.setValue(value);
    }
    linkAmplifier(amp) {
        this.output.setCallback((value) => {
            amp.amplifyValue(value);
        });
    }
    setOutput(callback) {
        this.output.setCallback(callback);
    }
}
function FindPermutations(arr) {
    let res = [];
    const helper = (arr2) => {
        if (arr2.length == arr.length)
            return res.push(arr2);
        for (let e of arr)
            if (!arr2.includes(e))
                helper([...arr2, e]);
    };
    helper([]);
    return res;
}
;
function RunAmplification(amplifiers, phases) {
    for (let i = 0; i < amplifiers.length; i++) {
        const amp = amplifiers[i];
        amp.readyAmplifier(phases[i]);
    }
    amplifiers[0].amplifyValue(0);
}
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const program = data.split(',').map((numStr) => {
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
        ampE.setOutput((value) => {
            loopCounter++;
            maxValue = Math.max(value, maxValue);
            ampA.amplifyValue(value);
        });
        ampA.amplifyValue(0);
    }
    console.log(maxValue);
});
