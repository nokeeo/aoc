declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2019/6/orbits.txt';

class Plannet {
  public name:string;
  public childPlannets:Plannet[];
  public parent:Plannet;

  constructor(name:string) {
    this.name = name;
    this.childPlannets = [];
    this.parent = null;
  }

  findPlannet(name:string) {
    if (this.name === name) {
      return this;
    }

    for (let childPlanet of this.childPlannets) {
      const foundPlanet = childPlanet.findPlannet(name);
      if (foundPlanet !== null) {
        return foundPlanet;
      }
    }

    return null;
  }

  orbitCount(depth:number) : number {
    let sum = depth;
    for (let childPlanet of this.childPlannets) {
      sum += childPlanet.orbitCount(depth + 1);
    }
    return sum;
  }

  depthOfChild(child:Plannet, depth:number) : number {
    if (child === this) {
      return depth;
    }

    for (let next of this.childPlannets) {
      const childDepth = next.depthOfChild(child, depth + 1);
      if (childDepth !== -1) {
        return childDepth;
      }
    }

    return -1;
  }
}

function ParsePlannets(line:string) : string[] {
  return line.split(')');
}

interface PlanetMatch {
  planet:Plannet;
  root:Plannet
}

function FindPlanet(roots:Plannet[], ID:string) : PlanetMatch {
  for (let root of roots) {
    const foundPlanet = root.findPlannet(ID);
    if (foundPlanet != null) {
      const match:PlanetMatch = {
        planet: foundPlanet,
        root: root,
      };
      return match;
    }
  }

  return null;
}

function FindCommonParent(p1:Plannet, p2:Plannet) : Plannet {
  const p1Seen:Plannet[] = [];
  const p2Seen:Plannet[] = [];

  let p1Parent = p1.parent;
  let p2Parent = p2.parent;
  while (p1Parent !== null || p2Parent !== null) {
    if (p1Parent !== null) {
      if (p1Parent === p2Parent) {
        return p1Parent;
      } else if (p2Seen.indexOf(p1Parent) !== -1) {
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

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line !== '';
  });

  let IDs = ParsePlannets(lines[0]);
  let rootPlannet = new Plannet(IDs[0]);
  rootPlannet.childPlannets.push(new Plannet(IDs[1]));
  rootPlannet.childPlannets[0].parent = rootPlannet;

  let rootPlanets:Plannet[] = [rootPlannet];
  for(let i = 1; i < lines.length; i++) {

    IDs = ParsePlannets(lines[i]);
    let orbitedMatch = FindPlanet(rootPlanets, IDs[0]);
    let orbitorMatch = FindPlanet(rootPlanets, IDs[1]);
    let orbitor = null;
    let orbited = null;
    if (orbitedMatch === null && orbitorMatch === null) {
      orbited = new Plannet(IDs[0]);
      orbitor = new Plannet(IDs[1]);
      orbited.childPlannets.push(orbitor);
      orbitor.parent = orbited;
      rootPlanets.push(orbited);
    } else if (orbitedMatch === null || orbitorMatch === null) {
      orbited = orbitedMatch !== null ? orbitedMatch.planet : new Plannet(IDs[0]);
      orbitor = orbitorMatch !== null ? orbitorMatch.planet : new Plannet(IDs[1]);
      orbitor.parent = orbited;
      orbited.childPlannets.push(orbitor);

      const indexOfOrbitor = rootPlanets.indexOf(orbitor);
      if (indexOfOrbitor !== -1) {
        rootPlanets[indexOfOrbitor] = orbited;
      }
    } else {
      orbitor = orbitorMatch.planet;
      orbited = orbitedMatch.planet;
      orbited.childPlannets.push(orbitor);
      orbitor.parent = orbited;

      const indexOfOrbitor = rootPlanets.indexOf(orbitor);
      if (indexOfOrbitor !== -1) {
        rootPlanets.splice(indexOfOrbitor, 1);
      }
      // rootPlanets.splice(rootPlanets.indexOf(orbitor.root), 1);
    }
  }

  const root = rootPlanets[0];
  const me = root.findPlannet('YOU');
  const santa = root.findPlannet('SAN');

  const commonParent = FindCommonParent(me, santa);
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
