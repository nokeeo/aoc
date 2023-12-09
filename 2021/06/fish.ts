declare function require(name:string);
declare namespace console {
  function log(para:any);
}
const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/6/fish.txt';

class Fish {
  private days:number;
  constructor(days:number) {
    this.days = days;
  }

  moveTime():Fish {
    this.days--;
    if (this.days < 0) {
      this.days = 6;
      return new Fish(8);
    }

    return null;
  }
}

class FishCollective {
  private days:number;
  private fish:number;

  getDays():number {
    return this.days;
  }

  size():number {
    return this.fish;
  }

  constructor(days:number, fish:number = 1) {
    this.days = days;
    this.fish = fish;
  }

  moveTime():FishCollective {
    this.days--;
    if (this.days < 0) {
      this.days = 6;
      return new FishCollective(8, this.fish);
    }

    return null;
  }

  merge(b:FishCollective) {
    this.fish += b.size();
  }
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const fishes:number[] = data.split(',').map((item) => {
    return Number(item.trim());
  });
  const fishCounts:{[id:number]: number} = {};
  for (let fish of fishes) {
    let currentCount = fishCounts[fish];
    if (currentCount === undefined) {
      currentCount = 0;
    }
    fishCounts[fish] = ++currentCount;
  }

  const collectives = [];
  for (let key of Object.keys(fishCounts)) {
    const count = fishCounts[key];
    collectives.push(new FishCollective(Number(key), count));
  }

  let gameLoops = 256;
  while (gameLoops > 0) {
    const newCollectives:FishCollective[] = [];
    for(let collective of collectives) {
      const newCollective = collective.moveTime();
      if (newCollective !== null) {
        newCollectives.push(newCollective);
      }
    }

    for (let newCollective of newCollectives) {
      let collectiveToMerge = collectives.find((c) => {
        return c.getDays() === newCollective.getDays()
      });

      if (collectiveToMerge !== undefined) {
        collectiveToMerge.merge(newCollective);
      } else {
        collectives.push(newCollective);
      }
    }
    gameLoops--;
  }

  console.log(collectives.reduce((acc:number, val) => {
    return acc + val.size();
  }, 0));
});
