declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/12/caves.txt';

namespace Graphing {
  export class Node {
    readonly key:string;
    children: Node[];

    constructor(key:string) {
      this.key = key;
      this.children = [];
    }
  }

  export class Graph {
    private nodes:{ [id:string]:Node };

    constructor() {
      this.nodes = {};
    }

    get(key:string):Node {
      const node = this.nodes[key];
      if (node === undefined) {
        return null;
      }
      return node;
    }

    set(node:Node) {
      this.nodes[node.key] = node;
    }
  }

  export function GetOrAdd(key:string, graph:Graph):Node {
    let node = graph.get(key);
    if (node === null) {
      node = new Node(key);
      graph.set(node);
    }
    return node;
  }
}  // namespace Gaphing

function isUpperCase(str:string):boolean {
  return str === str.toUpperCase();
}

class VisitedSet {
  private visited:Set<string>;
  private twiceVisited:boolean;

  constructor() {
    this.visited = new Set();
    this.twiceVisited = false;
  }

  copy():VisitedSet {
    const copy = new VisitedSet();
    copy.visited = new Set(this.visited);
    copy.twiceVisited = this.twiceVisited;
    return copy;
  }

  add(key:string) {
    if (this.visited.has(key)) {
      this.twiceVisited = true;
    }
    this.visited.add(key);
  }

  canVisit(key:string):boolean {
    if (key === 'start') {
      // Ensure we can only visit the start node once.
      return !this.visited.has(key);
    }

    const hasKey = this.visited.has(key);
    return !hasKey || (hasKey && !this.twiceVisited);
  }
}

class PathState {
  visited:VisitedSet;
  node:Graphing.Node;

  constructor(node:Graphing.Node, visited:VisitedSet) {
    this.node = node;
    this.visited = visited.copy();
  }
}

function NumberOfPaths(graph:Graphing.Graph):number {
  let pathCount = 0;
  let stack = [new PathState(graph.get('start'), new VisitedSet())];
  while (stack.length > 0) {
    const state = stack.pop();
    const n = state.node;
    const visited = state.visited.copy();
    if (!visited.canVisit(n.key)) {
      continue;
    }

    if (state.node.key === 'end') {
      pathCount++;
      continue;
    }

    if (!isUpperCase(n.key) || n.key === 'start') {
      visited.add(n.key);
    }

    for (let child of n.children) {
      stack.push(new PathState(child, visited));
    }
  }

  return pathCount;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines = data.split('\n').filter((line) => {
    return line !== '';
  });

  const graph = new Graphing.Graph();
  for (let line of lines) {
    const parts = line.split('-');
    const n1 = Graphing.GetOrAdd(parts[0], graph);
    const n2 = Graphing.GetOrAdd(parts[1], graph);
    n1.children.push(n2);
    n2.children.push(n1);
  }

  console.log(NumberOfPaths(graph));
});
