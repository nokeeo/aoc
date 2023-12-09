"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const IC = require("../IntComputer.js");
const InputAdapter_js_1 = require("../InputAdapter.js");
const OutputAdapter_js_1 = require("../OutputAdapter.js");
const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2019/5/test.txt';
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const memory = data.split(',').map((opStr) => {
        return Number(opStr);
    });
    const computer = new IC.IntComputer();
    const input = new InputAdapter_js_1.InputAdapter();
    const output = new OutputAdapter_js_1.OutputAdapter();
    input.setValue(5);
    computer.setInput(input);
    computer.setOutput(output);
    computer.loadMemory(memory);
    computer.executeAtPosition(0, () => { });
    output.setCallback((value) => {
        console.log(value);
    });
});
