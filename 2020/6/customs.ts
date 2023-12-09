declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/6/customs.txt';

interface Set<T> {
  union(a:Set<T>) : Set<T>;
}

Set.prototype.union = function(a) {
  let union = new Set();
  a.forEach((value, value1, set) => {
    if (this.has(value)) {
      union.add(value);
    }
  });
  return union;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n');

  let sum:number = 0;
  let groupAnswers:Set<string>[] = [];
  for(let line of lines) {
    const person:Set<string> = new Set();
    if (line == '') {
      if (groupAnswers.length === 0) {
        continue;
      }
      let unionSet:Set<string> = groupAnswers[0];
      for (let i = 1; i < groupAnswers.length; i++) {
        unionSet = unionSet.union(groupAnswers[i]);
      }
      sum += unionSet.size;
      groupAnswers = [];
      continue;
    }

    for (let i = 0; i < line.length; i++) {
      const char:string = line[i];
      person.add(char);
    }
    groupAnswers.push(person);
  }

  console.log(sum);
});
