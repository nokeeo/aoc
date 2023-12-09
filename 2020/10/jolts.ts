declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/10/jolts.txt';

function GetJoltDifferences(jolts:number[]) {
  let oneDiff= 0;
  let threeDiff = 1;  // Your device rating.
  for (let i = 0; i < jolts.length; i++) {
    let diff = 0;
    if (i === 0) {
      diff = jolts[i];
    } else {
      diff = jolts[i] - jolts[i - 1];
    }

    if (diff === 1) {
      oneDiff++;
    } else if (diff === 3) {
      threeDiff++;
    }
  }

  return oneDiff * threeDiff;
}

function GetTotalCombinations(adapters:number[], index:number, cache:{ [id:number] : number }) {
  let sum = 0;
  const currentValue = adapters[index];
  if (cache[currentValue]) {
    return cache[currentValue];
  }

  for (let i = index +  1; i <= adapters.length; i++) {
    if (i === adapters.length) {
      sum += 1;
      continue;
    }

    let nextValue = adapters[i];
    const diff = nextValue - currentValue;
    if (diff > 0 && diff < 4) {
      sum += GetTotalCombinations(adapters, i, cache);
    } else {
      break;
    }
  }
  cache[currentValue] = sum;

  return sum;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const numbers:number[] = data.split('\n').filter((line:string) => {
    return line !== '';
  }).map((line:string) => {
    return Number(line);
  }).sort((a:number, b:number) => {
    return a - b;
  });

  console.log(GetTotalCombinations(numbers, 0, {}));
});
