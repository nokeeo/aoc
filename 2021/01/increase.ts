declare function require(name:string);
const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/1/increase.txt';

function increaseCount(numbers:number[]): number {
  let increased = 0;
  for (let i = 1; i < numbers.length; i++) {
    const before = numbers[i - 1];
    const current = numbers[i];
    if (current > before) {
      increased++;
    }
  }

  return increased;
}

function slidingIncreaseCount(numbers:number[]): number {
  let increased = 0;
  const windowSize = 3;
  let lastSum = 0;
  for (let i = 0; i < windowSize && numbers.length; i++) {
    lastSum += numbers[i];
  }

  for (let i = windowSize; i < numbers.length; i++) {
    const currentSum = lastSum - numbers[i - windowSize] + numbers[i];
    if (currentSum > lastSum) {
      increased++;
    }

    lastSum = currentSum;
  }

  return increased;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const numbers:number[] = data.split('\n').map((line:string) => {
    return Number(line);
  });

  console.log(slidingIncreaseCount(numbers));
});
