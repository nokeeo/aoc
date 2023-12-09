declare function require(name:string);
const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/7/crab.txt';

function oldCost(crabs:number[], num:number):number {
  return crabs.reduce((acc:number, crab) => {
    return acc + Math.abs(num - crab);
  }, 0);
}

function newCost(crabs:number[], num:number):number {
  return crabs.reduce((acc:number, crab) => {
    const diff = Math.abs(num - crab);
    return acc + (diff / 2 * (1 + diff));
  }, 0);
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const crabs:number[] = data.split(',').map((item) => {
    return Number(item.trim());
  }).sort((a, b) => {
		return a - b;
	});

  let current = crabs[Math.floor(crabs.length / 2)];
  let currentCost = newCost(crabs, current);

  let above = current + 1;
  let aboveCost = newCost(crabs, above);

  let below = current - 1;
  let belowCost = newCost(crabs, below);

  const direction = currentCost > aboveCost ? 1 : -1;

  let next = current;
  let  nextCost = currentCost;
  while (current <= crabs[crabs.length - 1]) {
    current = current + direction;
    currentCost = newCost(crabs, current);

    if (currentCost <= nextCost) {
      nextCost = currentCost;
      next = current;
    }

  }
  console.log(`${next} ${nextCost}`);
});
