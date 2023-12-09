var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/13/busTimes.txt';
var Bus = /** @class */ (function () {
    function Bus(ID, offset) {
        this.ID = ID;
        this.offset = offset;
    }
    return Bus;
}());
function ParseBuses(times) {
    var offset = 0;
    var buses = [];
    for (var _i = 0, times_1 = times; _i < times_1.length; _i++) {
        var time = times_1[_i];
        if (time !== 'x') {
            buses.push(new Bus(Number(time), offset));
        }
        offset++;
    }
    return buses;
}
function GetTimeToBusID(startTime, value) {
    return value - (startTime % value);
}
function IsValidT(t, bus) {
    return (t + bus.offset) % bus.ID === 0;
}
function GetSyncTime(t, offset, increment, buses) {
    var i = 1;
    while (true) {
        var currentT = t + increment * i;
        var isValid = true;
        for (var _i = 0, buses_1 = buses; _i < buses_1.length; _i++) {
            var bus = buses_1[_i];
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
    return t + (increment * i);
}
function GetEarliestTimeAlignment(times) {
    var buses = ParseBuses(times);
    var t = 0;
    var increment = buses[0].ID;
    console.log(buses);
    for (var i = 1; i < buses.length; i++) {
        var bus = buses[i];
        increment = Math.max(increment, bus.ID);
        t = GetSyncTime(t, bus.offset, increment, buses.slice(0, i + 1));
        increment = t;
    }
    return t;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line !== '';
    });
    // const startTime = Number(lines[0]);
    var busIDs = lines[1].split(',');
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
