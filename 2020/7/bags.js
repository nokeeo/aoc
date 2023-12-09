const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/7/bags.txt';
const SubConditionRegex = /([0-9]+) (.*)/;
const BagIDRegex = /(.+) bags?/;
class BagCondition {
    constructor(ID) {
        this.ID = ID;
        this.subConditions = [];
        this.subConditionCountMap = {};
        this.parentConditions = [];
    }
    addSubCondition(condition, count) {
        this.subConditions.push(condition);
        this.subConditionCountMap[condition.ID] = count;
        condition.addParentCondition(this);
    }
    getSubBagCount() {
        return this._getSubBagCount(new Set());
    }
    _getSubBagCount(seenSet) {
        let count = 1; // 1 for self.
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
    addParentCondition(condition) {
        this.parentConditions.push(condition);
    }
}
class ConditionGraph {
    constructor() {
        this.conditions = {};
    }
    getCondition(ID) {
        let condition = this.conditions[ID];
        if (typeof (condition) === 'undefined') {
            condition = new BagCondition(ID);
            this.conditions[ID] = condition;
        }
        return condition;
    }
}
function GetBagID(input) {
    const result = BagIDRegex.exec(input);
    if (result.length < 2) {
        return '';
    }
    return result[1].trim();
}
function GetParentCount(ID, graph) {
    const condition = graph.getCondition(ID);
    let parentConditions = [...condition.parentConditions];
    const seenSet = new Set();
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
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const lines = data.split('\n').filter((line) => {
        return line != '';
    });
    const bagGraph = new ConditionGraph();
    for (let line of lines) {
        const parts = line.split('contain');
        const containerCondition = bagGraph.getCondition(GetBagID(parts[0]));
        if (parts[1].indexOf('no other bags') !== -1) {
            continue;
        }
        const subConditions = parts[1].split(',');
        for (let subConditionStr of subConditions) {
            const subConditionParts = SubConditionRegex.exec(subConditionStr);
            const count = subConditionParts[1];
            const subID = GetBagID(subConditionParts[2]);
            const subCondition = bagGraph.getCondition(subID);
            containerCondition.addSubCondition(subCondition, Number(count));
        }
    }
    console.log(bagGraph);
    const goldBag = bagGraph.getCondition('shiny gold');
    console.log(goldBag.getSubBagCount());
});
