declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/10/syntax.txt';

const closingMap = {
  '(': ')',
  '[': ']',
  '{': '}',
  '<': '>'
};

const pointMap = {
  ')': 3,
  ']': 57,
  '}': 1197,
  '>': 25137,
}

const completionPointMap = {
  ')': 1,
  ']': 2,
  '}': 3,
  '>': 4,
}

const openingSet = new Set(Object.keys(closingMap));

function lineScore(line:string):number {
  const stack = [];
  const elements = line.split('');
  while (elements.length > 0) {
    const el = elements.shift();

    let isError = false;
    if (openingSet.has(el)) {
      stack.push(el);
    } else {
      const top = stack.pop();
      isError = top === undefined || el !== closingMap[top];
    }

    if (isError) {
      return pointMap[el];
    }
  }

  return 0;
}

function completeScore(line:string):number {
  const stack = [];
  const elements = line.split('');
  while (elements.length > 0) {
    const el = elements.shift();
    if (openingSet.has(el)) {
      stack.push(el);
    } else {
      const top = stack.pop();
      if(top === undefined || el !== closingMap[top]) {
        return 0;
      }
    }
  }

  const completions = stack.reverse().map((el) => {
    return closingMap[el];
  });

  let sum = 0;
  for (let el of completions) {
    sum *= 5;
    sum += completionPointMap[el];
  }

  return sum;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines = data.split('\n').filter((line) => {
    return line !== '';
  });

  const scores = []
  for (let line of lines) {
    const score = completeScore(line);
    if (score > 0) {
      scores.push(score);
    }
  }

  scores.sort((a, b) => {
    return b - a;
  });

  console.log(scores[Math.floor(scores.length / 2)]);
});
