declare function require(name:string);

const process = require('process');

function CreateInvalidPosition(position:number) : HaltException {
  return new HaltException(0, 'Invalid memory position, ' + String(position));
}

function CreateInvalidDereferencePosition(position:number) : HaltException {
  return new HaltException(0, 'Invalid dereferenced position, ' + String(position));
}

function CreateInvalidReferenceAssign(position:number) : HaltException {
  return new HaltException(0, 'Invalid reference assign to position, ' + String(position));
}

function CreateInvalidValueAssign(position:number) : HaltException {
  return new HaltException(0, 'Invalid value assign to position, ' + String(position));
}

function CreateNanInputError() {
  return new HaltException(0, 'Input must be a number.');
}

function CreateInvalidParameterMode(modeIndex:number, op:number, position:number) {
  return new HaltException(0, 'Invalid parameter mode at index:' + String(modeIndex) + ', op:' + String(op) + 'at pos:' + String(position));
}

class HaltException {
  public code:number;
  public message:string;

  constructor(code:number, message:string) {
    this.code = code;
    this.message = message;
  }
}

export class MemoryBlock {
  private memory:number[]

  constructor(memory:number[]) {
    this.memory = memory;
  }

  public valueAtPosition(position:number) {
    if (position >= this.memory.length) {
      throw CreateInvalidPosition(position);
    }

    return this.memory[position];
  }

  public assignValueAtPosition(position:number, value:number) {
    if (position >= this.memory.length) {
      throw CreateInvalidValueAssign(position);
    }

    this.memory[position] = value;
  }

  public dereference(position:number) {
    const referencePosition = this.valueAtPosition(position);
    if (referencePosition >= this.memory.length) {
      throw CreateInvalidDereferencePosition(referencePosition);
    }
    return this.memory[referencePosition];
  }

  public assignReference(position:number, value:number) {
    const referencePosition = this.valueAtPosition(position);
    if (referencePosition >= this.memory.length) {
      throw CreateInvalidReferenceAssign(referencePosition);
    }

    this.memory[referencePosition] = value;
  }
}

class OpCode {
  public position:number;
  private opCode:number;
  private memory:MemoryBlock;

  constructor(opCode:number, position:number, memory:MemoryBlock) {
    this.opCode = opCode;
    this.position = position;
    this.memory = memory;
  }

  code() : number {
    return this.opCode - Math.floor(this.opCode / 100) * 100;
  }

  /** Zero indexed. **/
  parameter(index:number) : number {
    const numberOfDigits = this.numberOfCodeDigits();
    if (index >= numberOfDigits - 2) {
      return 0;
    }

    let shift = Math.pow(10, 3 + index);
    const leftAdjustedCode = this.opCode - Math.floor(this.opCode / shift) * shift;
    shift /= 10;
    return Math.floor(leftAdjustedCode / shift);
  }

  valueForParameter(index:number) {
    const mode = this.parameter(index);
    const parameterPosition = this.indexToPosition(index)
    if (mode === 0) {
      return this.memory.dereference(parameterPosition);
    } else if (mode === 1) {
      return this.memory.valueAtPosition(parameterPosition);
    }

    throw CreateInvalidParameterMode(index, this.opCode, this.position);
  }

  writeToParameter(index:number, value:number) {
    const position = this.indexToPosition(index);
    this.memory.assignReference(position, value);
  }

  nextPositionAfterIndex(index:number) {
    return this.indexToPosition(index) + 1;
  }

  private numberOfCodeDigits() : number {
    return Math.max(1, Math.ceil(Math.log10(this.opCode)));
  }

  private indexToPosition(index:number) : number {
    return this.position + index + 1;
  }
}

export interface InputCompletionBlock {
  (input:number) : void;
}

export interface Input {
  getNextInput(completion:InputCompletionBlock);
}

class ConsoleInput {
  getNextInput(completion:InputCompletionBlock) {
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

export interface OutputCompletionBlock {
  () : void;
}

export interface Output {
  outputValue(value:number, completion:OutputCompletionBlock);
}

class ConsoleOutput {
  outputValue(value:number, completion:OutputCompletionBlock) {
    console.log(value);
    completion();
  }
}

interface InstructionCompletionBlock {
  (position:number) : void;
}

interface Instruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock);
}

class AddInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val1 = opCode.valueForParameter(0);
    const val2 = opCode.valueForParameter(1);
    opCode.writeToParameter(2, val1 + val2);

    completion(opCode.nextPositionAfterIndex(2));
  }
}

class MultiplyInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val1 = opCode.valueForParameter(0);
    const val2 = opCode.valueForParameter(1);
    opCode.writeToParameter(2, val1 * val2);

    completion(opCode.nextPositionAfterIndex(2));
  }
}

class ExitInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    throw new HaltException(1, '');
  }
}

class OutputInstruction {
  private output:Output;

  constructor() {
    this.output = new ConsoleOutput();
  }

  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val = opCode.valueForParameter(0);
    this.output.outputValue(val, () => {
      completion(opCode.nextPositionAfterIndex(0));
    });
  }

  public setOutput(output:Output) {
    this.output = output;
  }
}

class InputInstruction {
  private input:Input;

  constructor() {
    this.input = new ConsoleInput();
  }

  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    this.input.getNextInput((input:number) => {
      opCode.writeToParameter(0, input);
      completion(opCode.nextPositionAfterIndex(0))
    });
  }

  setInput(input:Input) {
    if (input === null) {
      throw new Error('Cannout set InputInstruction\' input to null.');
    }
    this.input = input;
  }
}

class JumpIfTrueInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val = opCode.valueForParameter(0);
    if (val !== 0) {
      const jumpPosition = opCode.valueForParameter(1);
      completion(jumpPosition);
    } else {
      completion(opCode.nextPositionAfterIndex(1));
    }
  }
}

class JumpIfFalseInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val = opCode.valueForParameter(0);
    if (val === 0) {
      const jumpPosition = opCode.valueForParameter(1);
      completion(jumpPosition);
    } else {
      completion(opCode.nextPositionAfterIndex(1));
    }
  }
}

class LessThanInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val1 = opCode.valueForParameter(0);
    const val2 = opCode.valueForParameter(1);
    const out = (val1 < val2) ? 1 : 0;
    opCode.writeToParameter(2, out);
    completion(opCode.nextPositionAfterIndex(2));
  }
}

class EqualsInstruction {
  perform(opCode:OpCode, completion:InstructionCompletionBlock) {
    const val1 = opCode.valueForParameter(0);
    const val2 = opCode.valueForParameter(1);
    const out = (val1 === val2) ? 1 : 0;
    opCode.writeToParameter(2, out);
    completion(opCode.nextPositionAfterIndex(2));
  }
}

export interface IntComputerExecuteCompletion {
  () : void;
}

export class IntComputer {
  public memory:MemoryBlock;
  private instructionMap:{ [id:number] : Instruction };
  private inputInstruction:InputInstruction;
  private outputInstruction:OutputInstruction;

  constructor() {
    this.memory = new MemoryBlock([]);
    this.inputInstruction = new InputInstruction();
    this.outputInstruction = new OutputInstruction();
    this.instructionMap = {
      1 : new AddInstruction(),
      2 : new MultiplyInstruction(),
      3 : this.inputInstruction,
      4 : this.outputInstruction,
      5 : new JumpIfTrueInstruction(),
      6 : new JumpIfFalseInstruction(),
      7 : new LessThanInstruction(),
      8 : new EqualsInstruction(),
      99 : new ExitInstruction(),
    };
  }

  public loadMemory(memory:number[]) {
    this.memory = new MemoryBlock(memory);
  }

  public loadProgram(program:number[]) {
    this.loadMemory(program);
    this.executeAtPosition(0, ()=>{});
  }

  public setInput(input:Input) {
    this.inputInstruction.setInput(input);
  }

  public setOutput(output:Output) {
    this.outputInstruction.setOutput(output);
  }

  public executeAtPosition(position:number, completion:IntComputerExecuteCompletion) {
    const op = this.memory.valueAtPosition(position);
    const opCode = new OpCode(op, position, this.memory);
    const instruction:Instruction = this.instructionMap[opCode.code()];
    try {
      instruction.perform(opCode, (position:number) => {
        this.executeAtPosition(position, completion);
      });
    } catch(e) {
      if (e instanceof HaltException) {
        if (e.code === 0) {
          console.log(e.message);
        } else if (e.code === 1) {
          completion();
        }
      } else {
        console.log(e.stack);
        throw e;
      }
    }
  }
}
