var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/6/fish.txt';
var Fish = /** @class */ (function () {
    function Fish(days) {
        this.days = days;
    }
    Fish.prototype.moveTime = function () {
        this.days--;
        if (this.days < 0) {
            this.days = 6;
            return new Fish(8);
        }
        return null;
    };
    return Fish;
}());
var FishCollective = /** @class */ (function () {
    function FishCollective(days, fish) {
        if (fish === void 0) { fish = 1; }
        this.days = days;
        this.fish = fish;
    }
    FishCollective.prototype.getDays = function () {
        return this.days;
    };
    FishCollective.prototype.size = function () {
        return this.fish;
    };
    FishCollective.prototype.moveTime = function () {
        this.days--;
        if (this.days < 0) {
            this.days = 6;
            return new FishCollective(8, this.fish);
        }
        return null;
    };
    FishCollective.prototype.merge = function (b) {
        this.fish += b.size();
    };
    return FishCollective;
}());
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var fishes = data.split(',').map(function (item) {
        return Number(item.trim());
    });
    var fishCounts = {};
    for (var _i = 0, fishes_1 = fishes; _i < fishes_1.length; _i++) {
        var fish = fishes_1[_i];
        var currentCount = fishCounts[fish];
        if (currentCount === undefined) {
            currentCount = 0;
        }
        fishCounts[fish] = ++currentCount;
    }
    var collectives = [];
    for (var _a = 0, _b = Object.keys(fishCounts); _a < _b.length; _a++) {
        var key = _b[_a];
        var count = fishCounts[key];
        collectives.push(new FishCollective(Number(key), count));
    }
    var gameLoops = 256;
    while (gameLoops > 0) {
        var newCollectives = [];
        for (var _c = 0, collectives_1 = collectives; _c < collectives_1.length; _c++) {
            var collective = collectives_1[_c];
            var newCollective = collective.moveTime();
            if (newCollective !== null) {
                newCollectives.push(newCollective);
            }
        }
        var _loop_1 = function (newCollective) {
            var collectiveToMerge = collectives.find(function (c) {
                return c.getDays() === newCollective.getDays();
            });
            if (collectiveToMerge !== undefined) {
                collectiveToMerge.merge(newCollective);
            }
            else {
                collectives.push(newCollective);
            }
        };
        for (var _d = 0, newCollectives_1 = newCollectives; _d < newCollectives_1.length; _d++) {
            var newCollective = newCollectives_1[_d];
            _loop_1(newCollective);
        }
        gameLoops--;
    }
    console.log(collectives.reduce(function (acc, val) {
        return acc + val.size();
    }, 0));
});
