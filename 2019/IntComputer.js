"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const process = require('process');
function CreateInvalidPosition(position) {
    return new HaltException(0, 'Invalid memory position, ' + String(position));
}
function CreateInvalidDereferencePosition(position) {
    return new HaltException(0, 'Invalid dereferenced position, ' + String(position));
}
function CreateInvalidReferenceAssign(position) {
    return new HaltException(0, 'Invalid reference assign to position, ' + String(position));
}
function CreateInvalidValueAssign(position) {
    return new HaltException(0, 'Invalid value assign to position, ' + String(position));
}
function CreateNanInputError() {
    return new HaltException(0, 'Input must be a number.');
}
function CreateInvalidParameterMode(modeIndex, op, position) {
    return new HaltException(0, 'Invalid parameter mode at index:' + String(modeIndex) + ', op:' + String(op) + 'at pos:' + String(position));
}
class HaltException {
    constructor(code, message) {
        this.code = code;
        this.message = message;
    }
}
class MemoryBlock {
    constructor(memory) {
        this.memory = memory;
    }
    valueAtPosition(position) {
        if (position >= this.memory.length) {
            throw CreateInvalidPosition(position);
        }
        return this.memory[position];
    }
    assignValueAtPosition(position, value) {
        if (position >= this.memory.length) {
            throw CreateInvalidValueAssign(position);
        }
        this.memory[position] = value;
    }
    dereference(position) {
        const referencePosition = this.valueAtPosition(position);
        if (referencePosition >= this.memory.length) {
            throw CreateInvalidDereferencePosition(referencePosition);
        }
        return this.memory[referencePosition];
    }
    assignReference(position, value) {
        const referencePosition = this.valueAtPosition(position);
        if (referencePosition >= this.memory.length) {
            throw CreateInvalidReferenceAssign(referencePosition);
        }
        this.memory[referencePosition] = value;
    }
}
exports.MemoryBlock = MemoryBlock;
class OpCode {
    constructor(opCode, position, memory) {
        this.opCode = opCode;
        this.position = position;
        this.memory = memory;
    }
    code() {
        return this.opCode - Math.floor(this.opCode / 100) * 100;
    }
    /** Zero indexed. **/
    parameter(index) {
        const numberOfDigits = this.numberOfCodeDigits();
        if (index >= numberOfDigits - 2) {
            return 0;
        }
        let shift = Math.pow(10, 3 + index);
        const leftAdjustedCode = this.opCode - Math.floor(this.opCode / shift) * shift;
        shift /= 10;
        return Math.floor(leftAdjustedCode / shift);
    }
    valueForParameter(index) {
        const mode = this.parameter(index);
        const parameterPosition = this.indexToPosition(index);
        if (mode === 0) {
            return this.memory.dereference(parameterPosition);
        }
        else if (mode === 1) {
            return this.memory.valueAtPosition(parameterPosition);
        }
        throw CreateInvalidParameterMode(index, this.opCode, this.position);
    }
    writeToParameter(index, value) {
        const position = this.indexToPosition(index);
        this.memory.assignReference(position, value);
    }
    nextPositionAfterIndex(index) {
        return this.indexToPosition(index) + 1;
    }
    numberOfCodeDigits() {
        return Math.max(1, Math.ceil(Math.log10(this.opCode)));
    }
    indexToPosition(index) {
        return this.position + index + 1;
    }
}
class ConsoleInput {
    getNextInput(completion) {
        console.log('Please input value:');
        process.stdin.once('readable', () => {
            const val = Number(process.stdin.read());
            if (isNaN(val)) {
                throw CreateNanInputError();
            }
            completion(val);
        });
    }
}
class ConsoleOutput {
    outputValue(value, completion) {
        console.log(value);
        completion();
    }
}
class AddInstruction {
    perform(opCode, completion) {
        const val1 = opCode.valueForParameter(0);
        const val2 = opCode.valueForParameter(1);
        opCode.writeToParameter(2, val1 + val2);
        completion(opCode.nextPositionAfterIndex(2));
    }
}
class MultiplyInstruction {
    perform(opCode, completion) {
        const val1 = opCode.valueForParameter(0);
        const val2 = opCode.valueForParameter(1);
        opCode.writeToParameter(2, val1 * val2);
        completion(opCode.nextPositionAfterIndex(2));
    }
}
class ExitInstruction {
    perform(opCode, completion) {
        throw new HaltException(1, '');
    }
}
class OutputInstruction {
    constructor() {
        this.output = new ConsoleOutput();
    }
    perform(opCode, completion) {
        const val = opCode.valueForParameter(0);
        this.output.outputValue(val, () => {
            completion(opCode.nextPositionAfterIndex(0));
        });
    }
    setOutput(output) {
        this.output = output;
    }
}
class InputInstruction {
    constructor() {
        this.input = new ConsoleInput();
    }
    perform(opCode, completion) {
        this.input.getNextInput((input) => {
            opCode.writeToParameter(0, input);
            completion(opCode.nextPositionAfterIndex(0));
        });
    }
    setInput(input) {
        if (input === null) {
            throw new Error('Cannout set InputInstruction\' input to null.');
        }
        this.input = input;
    }
}
class JumpIfTrueInstruction {
    perform(opCode, completion) {
        const val = opCode.valueForParameter(0);
        if (val !== 0) {
            const jumpPosition = opCode.valueForParameter(1);
            completion(jumpPosition);
        }
        else {
            completion(opCode.nextPositionAfterIndex(1));
        }
    }
}
class JumpIfFalseInstruction {
    perform(opCode, completion) {
        const val = opCode.valueForParameter(0);
        if (val === 0) {
            const jumpPosition = opCode.valueForParameter(1);
            completion(jumpPosition);
        }
        else {
            completion(opCode.nextPositionAfterIndex(1));
        }
    }
}
class LessThanInstruction {
    perform(opCode, completion) {
        const val1 = opCode.valueForParameter(0);
        const val2 = opCode.valueForParameter(1);
        const out = (val1 < val2) ? 1 : 0;
        opCode.writeToParameter(2, out);
        completion(opCode.nextPositionAfterIndex(2));
    }
}
class EqualsInstruction {
    perform(opCode, completion) {
        const val1 = opCode.valueForParameter(0);
        const val2 = opCode.valueForParameter(1);
        const out = (val1 === val2) ? 1 : 0;
        opCode.writeToParameter(2, out);
        completion(opCode.nextPositionAfterIndex(2));
    }
}
class IntComputer {
    constructor() {
        this.memory = new MemoryBlock([]);
        this.inputInstruction = new InputInstruction();
        this.outputInstruction = new OutputInstruction();
        this.instructionMap = {
            1: new AddInstruction(),
            2: new MultiplyInstruction(),
            3: this.inputInstruction,
            4: this.outputInstruction,
            5: new JumpIfTrueInstruction(),
            6: new JumpIfFalseInstruction(),
            7: new LessThanInstruction(),
            8: new EqualsInstruction(),
            99: new ExitInstruction(),
        };
    }
    loadMemory(memory) {
        this.memory = new MemoryBlock(memory);
    }
    loadProgram(program) {
        this.loadMemory(program);
        this.executeAtPosition(0, () => { });
    }
    setInput(input) {
        this.inputInstruction.setInput(input);
    }
    setOutput(output) {
        this.outputInstruction.setOutput(output);
    }
    executeAtPosition(position, completion) {
        const op = this.memory.valueAtPosition(position);
        const opCode = new OpCode(op, position, this.memory);
        const instruction = this.instructionMap[opCode.code()];
        try {
            instruction.perform(opCode, (position) => {
                this.executeAtPosition(position, completion);
            });
        }
        catch (e) {
            if (e instanceof HaltException) {
                if (e.code === 0) {
                    console.log(e.message);
                }
                else if (e.code === 1) {
                    completion();
                }
            }
            else {
                console.log(e.stack);
                throw e;
            }
        }
    }
}
exports.IntComputer = IntComputer;
