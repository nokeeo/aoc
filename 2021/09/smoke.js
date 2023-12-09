const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2021/09/smoke.txt';
class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
    toString() {
        return `${this.x},${this.y}`;
    }
}
class Grid {
    constructor() {
        this.rows = [];
    }
    addRow(line) {
        const row = [].concat(line.split('').map((num) => {
            return Number(num);
        }));
        this.rows.push(row);
    }
    width() {
        if (this.rows.length === 0) {
            return 0;
        }
        return this.rows[0].length;
    }
    height() {
        return this.rows.length;
    }
    get(x, y) {
        return this.rows[y][x];
    }
    getNeighbors(x, y) {
        const neighbors = [];
        // Left
        if (x - 1 >= 0) {
            neighbors.push(new Point(x - 1, y));
        }
        // Top
        if (y - 1 >= 0) {
            neighbors.push(new Point(x, y - 1));
        }
        // Right
        if (x + 1 < this.width()) {
            neighbors.push(new Point(x + 1, y));
        }
        // Bottom
        if (y + 1 < this.height()) {
            neighbors.push(new Point(x, y + 1));
        }
        return neighbors;
    }
    getSinks() {
        const sinks = [];
        for (let y = 0; y < this.height(); y++) {
            for (let x = 0; x < this.width(); x++) {
                const val = this.get(x, y);
                const neighbors = this.getNeighbors(x, y);
                let isSink = true;
                for (let neighbor of neighbors) {
                    const neighborVal = this.get(neighbor.x, neighbor.y);
                    if (neighborVal <= val) {
                        isSink = false;
                        break;
                    }
                }
                if (isSink) {
                    sinks.push(new Point(x, y));
                }
            }
        }
        return sinks;
    }
}
function ProcessSink(origin, grid) {
    let size = 0;
    const queue = [origin];
    const visited = new Set();
    while (queue.length > 0) {
        const point = queue.shift();
        if (visited.has(point.toString())) {
            continue;
        }
        const val = grid.get(point.x, point.y);
        if (val < 9) {
            size++;
            const neighbors = grid.getNeighbors(point.x, point.y);
            for (let neighbor of neighbors) {
                queue.push(neighbor);
            }
        }
        visited.add(point.toString());
    }
    return size;
}
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const lines = data.split('\n').filter((line) => {
        return line !== '';
    });
    const grid = new Grid();
    for (let line of lines) {
        grid.addRow(line);
    }
    const sizes = grid.getSinks().map((sink) => {
        return ProcessSink(sink, grid);
    });
    console.log(sizes.sort((a, b) => {
        return b - a;
    }).slice(0, 3).reduce((acc, val) => {
        return acc * val;
    }));
});
