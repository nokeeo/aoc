declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/8/assembly.txt';

interface Result {
  value : number;
  loops : boolean;
}

function Run(program:string[]) : Result {
  let index = 0;
  let acc = 0;
  const seenIndicies:Set<number> = new Set();
  let looped = false;
  while (true) {
    if (index >= program.length) {
      break;
    }

    if (seenIndicies.has(index)) {
      looped = true;
      break;
    }
    seenIndicies.add(index);

    const parts:string[] = program[index].split(' ');
    const op = parts[0];
    const val = Number(parts[1]);
    if (op === 'acc') {
      acc += val;
      index++;
    } else if (op === 'jmp') {
      index += val;
    } else if (op === 'nop') {
      index++;
    }
  }

  return {
    value : acc,
    loops : looped,
  };
}

function SwapInstruction(line:string) : string {
  if (line.indexOf('nop') > -1) {
    return line.replace('nop', 'jmp');
  } else {
    return line.replace('jmp', 'nop');
  }
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line !== '';
  });

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.indexOf('acc') > -1) {
      continue;
    }

    const newLines = [...lines];
    newLines[i] = SwapInstruction(line);

    const result = Run(newLines);
    if (!result.loops) {
      console.log(result.value);
      break;
    }
  }
});
