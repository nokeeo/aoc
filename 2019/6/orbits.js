var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2019/6/orbits.txt';
var Plannet = /** @class */ (function () {
    function Plannet(name) {
        this.name = name;
        this.childPlannets = [];
        this.parent = null;
    }
    Plannet.prototype.findPlannet = function (name) {
        if (this.name === name) {
            return this;
        }
        for (var _i = 0, _a = this.childPlannets; _i < _a.length; _i++) {
            var childPlanet = _a[_i];
            var foundPlanet = childPlanet.findPlannet(name);
            if (foundPlanet !== null) {
                return foundPlanet;
            }
        }
        return null;
    };
    Plannet.prototype.orbitCount = function (depth) {
        var sum = depth;
        for (var _i = 0, _a = this.childPlannets; _i < _a.length; _i++) {
            var childPlanet = _a[_i];
            sum += childPlanet.orbitCount(depth + 1);
        }
        return sum;
    };
    Plannet.prototype.depthOfChild = function (child, depth) {
        if (child === this) {
            return depth;
        }
        for (var _i = 0, _a = this.childPlannets; _i < _a.length; _i++) {
            var next = _a[_i];
            var childDepth = next.depthOfChild(child, depth + 1);
            if (childDepth !== -1) {
                return childDepth;
            }
        }
        return -1;
    };
    return Plannet;
}());
function ParsePlannets(line) {
    return line.split(')');
}
function FindPlanet(roots, ID) {
    for (var _i = 0, roots_1 = roots; _i < roots_1.length; _i++) {
        var root = roots_1[_i];
        var foundPlanet = root.findPlannet(ID);
        if (foundPlanet != null) {
            var match = {
                planet: foundPlanet,
                root: root
            };
            return match;
        }
    }
    return null;
}
function FindCommonParent(p1, p2) {
    var p1Seen = [];
    var p2Seen = [];
    var p1Parent = p1.parent;
    var p2Parent = p2.parent;
    while (p1Parent !== null || p2Parent !== null) {
        if (p1Parent !== null) {
            if (p1Parent === p2Parent) {
                return p1Parent;
            }
            else if (p2Seen.indexOf(p1Parent) !== -1) {
                return p1Parent;
            }
            p1Seen.push(p1Parent);
            p1Parent = p1Parent.parent;
        }
        if (p2Parent !== null) {
            if (p1Seen.indexOf(p2Parent) !== -1) {
                return p2Parent;
            }
            p2Seen.push(p2Parent);
            p2Parent = p2Parent.parent;
        }
    }
    return null;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var lines = data.split('\n').filter(function (line) {
        return line !== '';
    });
    var IDs = ParsePlannets(lines[0]);
    var rootPlannet = new Plannet(IDs[0]);
    rootPlannet.childPlannets.push(new Plannet(IDs[1]));
    rootPlannet.childPlannets[0].parent = rootPlannet;
    var rootPlanets = [rootPlannet];
    for (var i = 1; i < lines.length; i++) {
        IDs = ParsePlannets(lines[i]);
        var orbitedMatch = FindPlanet(rootPlanets, IDs[0]);
        var orbitorMatch = FindPlanet(rootPlanets, IDs[1]);
        var orbitor = null;
        var orbited = null;
        if (orbitedMatch === null && orbitorMatch === null) {
            orbited = new Plannet(IDs[0]);
            orbitor = new Plannet(IDs[1]);
            orbited.childPlannets.push(orbitor);
            orbitor.parent = orbited;
            rootPlanets.push(orbited);
        }
        else if (orbitedMatch === null || orbitorMatch === null) {
            orbited = orbitedMatch !== null ? orbitedMatch.planet : new Plannet(IDs[0]);
            orbitor = orbitorMatch !== null ? orbitorMatch.planet : new Plannet(IDs[1]);
            orbitor.parent = orbited;
            orbited.childPlannets.push(orbitor);
            var indexOfOrbitor = rootPlanets.indexOf(orbitor);
            if (indexOfOrbitor !== -1) {
                rootPlanets[indexOfOrbitor] = orbited;
            }
        }
        else {
            orbitor = orbitorMatch.planet;
            orbited = orbitedMatch.planet;
            orbited.childPlannets.push(orbitor);
            orbitor.parent = orbited;
            var indexOfOrbitor = rootPlanets.indexOf(orbitor);
            if (indexOfOrbitor !== -1) {
                rootPlanets.splice(indexOfOrbitor, 1);
            }
            // rootPlanets.splice(rootPlanets.indexOf(orbitor.root), 1);
        }
    }
    var root = rootPlanets[0];
    var me = root.findPlannet('YOU');
    var santa = root.findPlannet('SAN');
    var commonParent = FindCommonParent(me, santa);
    console.log(commonParent);
    console.log(commonParent.depthOfChild(me, 0) + commonParent.depthOfChild(santa, 0));
    // while(rootPlanets.length > 1) {
    //   const p1 = rootPlanets.shift();
    //   const index = rootPlanets.findIndex((p2) => {
    //     return p1.findPlannet(p2.name) !== null;
    //   });
    //
    //   if (index === -1) {
    //     rootPlanets.push(p1);
    //     continue;
    //   }
    //
    //   const mergePlanet = rootPlanets.splice(index, 1);
    //   const matchingPlanet
    // }
});
