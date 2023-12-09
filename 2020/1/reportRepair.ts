declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode2020/1/reportRepair.txt';


fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const numbers:number[] = data.split('\n').map((line:string) => {
    return Number(line);
  }).sort((a:number, b:number) => {
    return a - b;
  }).filter((a:number) => {
    return a != 0;
  });

  //Part 1.
  // for (let i = 0; i < numbers.length; i++) {
  //   for (let j = numbers.length - 1; j > i; j--) {
  //     const sum = numbers[i] + numbers[j];
  //     if (sum == 2020) {
  //       console.log(numbers[i] * numbers[j]);
  //       return;
  //     } else if (sum < 2020) {
  //       break;
  //     }
  //   }
  // }

  // Part 2.
  for (let i = 0; i < numbers.length; i++) {
    for (let j = 0; j < numbers.length; j++) {
      for (let k = numbers.length - 1; (k > j && k > i); k--) {
        const sum = numbers[i] + numbers[j] + numbers[k];
        if (sum == 2020) {
          console.log(numbers[i] * numbers[j] * numbers[k]);
          return;
        } else if (sum < 2020) {
          break;
        }
      }
    }
  }
});
