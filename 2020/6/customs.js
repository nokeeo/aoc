const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/6/customs.txt';
Set.prototype.union = function (a) {
    let union = new Set();
    a.forEach((value, value1, set) => {
        if (this.has(value)) {
            union.add(value);
        }
    });
    return union;
};
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const lines = data.split('\n');
    let sum = 0;
    let groupAnswers = [];
    for (let line of lines) {
        const person = new Set();
        if (line == '') {
            if (groupAnswers.length === 0) {
                continue;
            }
            let unionSet = groupAnswers[0];
            for (let i = 1; i < groupAnswers.length; i++) {
                unionSet = unionSet.union(groupAnswers[i]);
            }
            sum += unionSet.size;
            groupAnswers = [];
            continue;
        }
        for (let i = 0; i < line.length; i++) {
            const char = line[i];
            person.add(char);
        }
        groupAnswers.push(person);
    }
    console.log(sum);
});
