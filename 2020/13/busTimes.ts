declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/13/busTimes.txt';

class Bus {
  ID:number;
  offset:number;

  constructor(ID:number, offset:number) {
    this.ID = ID;
    this.offset = offset;
  }
}

function ParseBuses(times:string[]) : Bus[] {
  let offset = 0;
  const buses:Bus[] = [];
  for (let time of times) {
    if (time !== 'x') {
      buses.push(new Bus(Number(time), offset));
    }
    offset++;
  }
  return buses;
}

function GetTimeToBusID(startTime:number, value:number) : number {
  return value - (startTime % value);
}

function IsValidT(t:number, bus:Bus) : boolean {
  return (t + bus.offset) % bus.ID === 0;
}

function GetSyncTime(t:number, offset:number, increment:number, buses:Bus[]) : number {
  let i = 1;
  while (true) {
    const currentT = t + increment * i;
    let isValid = true;
    for (let bus of buses) {
      if (!IsValidT(currentT, bus)) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      break;
    }
    i++;
  }
  return  t + (increment * i);
}

function GetEarliestTimeAlignment(times:string[]) : number {
  const buses = ParseBuses(times);
  let t = 0;
  let increment = buses[0].ID;
  console.log(buses);
  for (let i = 1; i < buses.length; i++) {
    let bus = buses[i];
    increment = Math.max(increment, bus.ID);
    t = GetSyncTime(t, bus.offset, increment, buses.slice(0, i + 1));
    increment = t;
  }
  return t;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line !== '';
  });

  // const startTime = Number(lines[0]);
  const busIDs = lines[1].split(',');
  //
  // let busID:number = 0;
  // let timeToBus:number = Number.MAX_VALUE;
  // for (let ID of busIDs) {
  //   if (ID === 'x') {
  //     continue;
  //   }
  //
  //   const value = Number(ID);
  //   const timeRemaining = GetTimeToBusID(startTime, value);
  //   if (timeToBus > timeRemaining) {
  //     busID = value;
  //     timeToBus = timeRemaining;
  //   }
  // }
  //
  // console.log(busID * timeToBus);

  console.log(GetEarliestTimeAlignment(busIDs));
});
