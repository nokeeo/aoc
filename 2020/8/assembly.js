const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/8/assembly.txt';
function Run(program) {
    let index = 0;
    let acc = 0;
    const seenIndicies = new Set();
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
        const parts = program[index].split(' ');
        const op = parts[0];
        const val = Number(parts[1]);
        if (op === 'acc') {
            acc += val;
            index++;
        }
        else if (op === 'jmp') {
            index += val;
        }
        else if (op === 'nop') {
            index++;
        }
    }
    return {
        value: acc,
        loops: looped,
    };
}
function SwapInstruction(line) {
    if (line.indexOf('nop') > -1) {
        return line.replace('nop', 'jmp');
    }
    else {
        return line.replace('jmp', 'nop');
    }
}
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const lines = data.split('\n').filter((line) => {
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
