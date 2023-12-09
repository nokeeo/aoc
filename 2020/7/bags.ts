declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2020/7/bags.txt';

const SubConditionRegex = /([0-9]+) (.*)/;
const BagIDRegex = /(.+) bags?/;

class BagCondition {
  private ID:string;
  private subConditions:BagCondition[];
  private subConditionCountMap : {[id:string] : number};
  public parentConditions:BagCondition[];

  constructor(ID:string) {
    this.ID = ID;
    this.subConditions = [];
    this.subConditionCountMap = {};
    this.parentConditions = [];
  }

  addSubCondition(condition:BagCondition, count:number) {
    this.subConditions.push(condition);
    this.subConditionCountMap[condition.ID] = count;
    condition.addParentCondition(this);
  }

  getSubBagCount() : number {
    return this._getSubBagCount(new Set());
  }

  private _getSubBagCount(seenSet:Set<BagCondition>) : number {
    let count = 1;  // 1 for self.
    for (let condition of this.subConditions) {
      if (seenSet.has(condition)) {
        continue;
      }
      seenSet.add(condition);

      const conditionCount = this.subConditionCountMap[condition.ID];
      count += conditionCount * condition.getSubBagCount();
    }

    return count;
  }

  private addParentCondition(condition:BagCondition) {
    this.parentConditions.push(condition);
  }
}

class ConditionGraph {
  private conditions:{ [id:string] : BagCondition };

  constructor() {
    this.conditions = {};
  }

  getCondition(ID:string) {
    let condition = this.conditions[ID];
    if (typeof(condition) === 'undefined') {
      condition = new BagCondition(ID);
      this.conditions[ID] = condition;
    }
    return condition;
  }
}

function GetBagID(input:string) {
  const result = BagIDRegex.exec(input);
  if (result.length < 2) {
    return '';
  }

  return result[1].trim();
}

function GetParentCount(ID:string, graph:ConditionGraph) : number {
  const condition = graph.getCondition(ID);
  let parentConditions = [...condition.parentConditions];
  const seenSet:Set<BagCondition> = new Set();
  let count = 0;
  while (parentConditions.length > 0) {
    const parentCondition = parentConditions.pop();
    if (seenSet.has(parentCondition)) {
      continue;
    }

    parentConditions = parentConditions.concat(parentCondition.parentConditions);
    count++;
    seenSet.add(parentCondition);
  }

  return count;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  const bagGraph = new ConditionGraph();
  for (let line of lines) {
    const parts:string[] = line.split('contain');
    const containerCondition = bagGraph.getCondition(GetBagID(parts[0]));
    if (parts[1].indexOf('no other bags') !== -1) {
      continue;
    }

    const subConditions:string[] = parts[1].split(',');
    for (let subConditionStr of subConditions) {
      const subConditionParts = SubConditionRegex.exec(subConditionStr);
      const count = subConditionParts[1];
      const subID = GetBagID(subConditionParts[2]);
      const subCondition = bagGraph.getCondition(subID);
      containerCondition.addSubCondition(subCondition, Number(count));
    }
  }
  const goldBag = bagGraph.getCondition('shiny gold');
  console.log(goldBag.getSubBagCount());
});
