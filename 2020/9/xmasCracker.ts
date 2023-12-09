declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/9/xmas.txt';

interface MinMax {
  min:number;
  max:number;
}

function MinMax(numbers:number[]) : number {
  let min = Number.MAX_VALUE;
  let max = Number.MIN_VALUE;
  for (let number of numbers) {
    min = Math.min(min, number);
    max = Math.max(max, number);
  }

  return min + max;
}

function IsNumberSumOfTwoNumbers(num:number, numbers:number[]) : boolean {
  for (let i = 0; i < numbers.length; i++) {
    for (let j = i + 1; j < numbers.length; j++) {
      if (numbers[i] + numbers[j] === num) {
        return true;
      }
    }
  }

  return false;
}

function GetEncryptionWeaknes(num:number, numbers:number[]) : number {
  let index1 = 0;
  let index2 = 1;
  while (index1 < numbers.length - 1 && index2 < numbers.length - 1) {
    let sum = numbers[index1];
    while (sum <= num) {
      const slice = numbers.slice(index1, index2 + 1);
      sum = slice.reduce((acc:number, value:number) => {
        return acc + value;
      });
      if (sum === num) {
        return MinMax(slice);
      }
      index2++;
    }
    index1++;
    index2 = index1 + 1;
  }

  return 0;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const numbers:number[] = data.split('\n').filter((line:string) => {
    return line !== '';
  }).map((line:string) => {
    return Number(line);
  });

  const preambleLength = 25;
  for (let i = preambleLength; i < numbers.length; i++) {
    const num = numbers[i];
    const preamble = numbers.slice(i - preambleLength, i);
    if (!IsNumberSumOfTwoNumbers(num, preamble)) {
      console.log(GetEncryptionWeaknes(num, numbers));
      break;
    }
  }
});
