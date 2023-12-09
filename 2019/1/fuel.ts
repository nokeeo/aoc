declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/1/moduleMass.txt';

function CalculateFuelRequirement(mass:number) : number {
  let fuel:number = Math.max(0, Math.floor(mass / 3) - 2);
  if (fuel != 0) {
    fuel += CalculateFuelRequirement(fuel);
  }

  return fuel;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  let fuelSum:number = 0;
  for (let line of lines) {
    fuelSum += CalculateFuelRequirement(Number(line));
  }

  console.log(fuelSum);
});
