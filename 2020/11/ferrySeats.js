var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode/2020/11/ferrySeats.txt';
function GetValue(x, y, rows) {
    return rows[y].charAt(x);
}
function GetSeatInDirection(x, y, deltaX, deltaY, rows) {
    while (true) {
        var newX = x + deltaX;
        var newY = y + deltaY;
        if (newY >= 0 && newY < rows.length && newX >= 0 && newX < rows[y].length) {
            var value = GetValue(newX, newY, rows);
            if (value !== '.') {
                return value;
            }
        }
        else {
            break;
        }
        x = newX;
        y = newY;
    }
    return null;
}
function GetAdjacentSeats(x, y, rows) {
    var neighbors = [];
    var upperCheck = y > 0;
    var leftCheck = x > 0;
    var lowerCheck = y < rows.length - 1;
    var rightCheck = x < rows[y].length - 1;
    // Up.
    if (upperCheck) {
        neighbors.push(GetValue(x, y - 1, rows));
        // Up left.
        if (leftCheck) {
            neighbors.push(GetValue(x - 1, y - 1, rows));
        }
        // Up right.
        if (rightCheck) {
            neighbors.push(GetValue(x + 1, y - 1, rows));
        }
    }
    // Down.
    if (lowerCheck) {
        neighbors.push(GetValue(x, y + 1, rows));
        // Down left.
        if (leftCheck) {
            neighbors.push(GetValue(x - 1, y + 1, rows));
        }
        // Down right.
        if (rightCheck) {
            neighbors.push(GetValue(x + 1, y + 1, rows));
        }
    }
    // Left.
    if (leftCheck) {
        neighbors.push(GetValue(x - 1, y, rows));
    }
    // Right.
    if (rightCheck) {
        neighbors.push(GetValue(x + 1, y, rows));
    }
    return neighbors;
}
function GetAdjacentVisibleSeats(x, y, rows) {
    var neighbors = [];
    // Up.
    var up = GetSeatInDirection(x, y, 0, -1, rows);
    if (up !== null) {
        neighbors.push(up);
    }
    // Up left.
    var upLeft = GetSeatInDirection(x, y, -1, -1, rows);
    if (upLeft !== null) {
        neighbors.push(upLeft);
    }
    // Up right.
    var upRight = GetSeatInDirection(x, y, 1, -1, rows);
    if (upRight !== null) {
        neighbors.push(upRight);
    }
    // Down.
    var down = GetSeatInDirection(x, y, 0, 1, rows);
    if (down !== null) {
        neighbors.push(down);
    }
    // Down left.
    var downLeft = GetSeatInDirection(x, y, -1, 1, rows);
    if (downLeft !== null) {
        neighbors.push(downLeft);
    }
    // Down right.
    var downRight = GetSeatInDirection(x, y, 1, 1, rows);
    if (downRight !== null) {
        neighbors.push(downRight);
    }
    // Left.
    var left = GetSeatInDirection(x, y, -1, 0, rows);
    if (left !== null) {
        neighbors.push(left);
    }
    // Right.
    var right = GetSeatInDirection(x, y, 1, 0, rows);
    if (right !== null) {
        neighbors.push(right);
    }
    return neighbors;
}
function AreAnySeatsOccupied(seats) {
    return seats.reduce(function (acc, currentValue) {
        return acc || (currentValue === '#');
    }, false);
}
function HandleEmptySeat(adjacentSeats) {
    if (!AreAnySeatsOccupied(adjacentSeats)) {
        return '#';
    }
    return 'L';
}
function HandleOccupiedSeat(adjacentSeats) {
    var occupiedSeatCount = 0;
    for (var _i = 0, adjacentSeats_1 = adjacentSeats; _i < adjacentSeats_1.length; _i++) {
        var adjacentSeat = adjacentSeats_1[_i];
        if (adjacentSeat === '#') {
            occupiedSeatCount++;
        }
    }
    return (occupiedSeatCount >= 5) ? 'L' : '#';
}
function CountOccupiedSeatsOnFerry(rows) {
    var count = 0;
    for (var _i = 0, rows_1 = rows; _i < rows_1.length; _i++) {
        var row = rows_1[_i];
        for (var i = 0; i < row.length; i++) {
            if (row.charAt(i) === '#') {
                count++;
            }
        }
    }
    return count;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var rows = data.split('\n').filter(function (line) {
        return line !== '';
    });
    var seatsChanged = false;
    var newSeats = [];
    while (true) {
        for (var y = 0; y < rows.length; y++) {
            var row = rows[y];
            var newRow = '';
            for (var x = 0; x < row.length; x++) {
                var currentValue = GetValue(x, y, rows);
                var adjacentSeats = GetAdjacentVisibleSeats(x, y, rows);
                var newValue = currentValue;
                if (currentValue === 'L') {
                    newValue = HandleEmptySeat(adjacentSeats);
                }
                else if (currentValue === '#') {
                    newValue = HandleOccupiedSeat(adjacentSeats);
                }
                newRow += newValue;
                seatsChanged = seatsChanged || (newValue !== currentValue);
            }
            newSeats.push(newRow);
        }
        rows = newSeats;
        newSeats = [];
        if (!seatsChanged) {
            break;
        }
        seatsChanged = false;
    }
    console.log(CountOccupiedSeatsOnFerry(rows));
});
