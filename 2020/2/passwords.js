var fileSystem = require('fs');
var inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode2020/2/passwords.txt';
var Password = /** @class */ (function () {
    function Password() {
    }
    Password.prototype.toString = function () {
        return String(this.minCount) + '-' + String(this.maxCount) + ' ' + this.letter + ': ' + this.text;
    };
    return Password;
}());
var CharacterCounter = /** @class */ (function () {
    function CharacterCounter() {
        this.counts = {};
    }
    CharacterCounter.prototype.addCount = function (char) {
        var currentCount = this.counts[char];
        if (typeof (currentCount) == 'undefined') {
            currentCount = 1;
        }
        else {
            currentCount++;
        }
        this.counts[char] = currentCount;
    };
    CharacterCounter.prototype.countForChar = function (char) {
        var count = this.counts[char];
        if (typeof (count) == 'undefined') {
            return 0;
        }
        return count;
    };
    return CharacterCounter;
}());
function ParseTextFile(data) {
    var lines = data.split('\n').filter(function (line) {
        return line != '';
    });
    var passwords = [];
    for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
        var line = lines_1[_i];
        var parts = line.split(' ');
        if (parts.length != 3) {
            continue;
        }
        var password = new Password;
        var range = parts[0].split('-');
        password.minCount = Number(range[0]);
        password.maxCount = Number(range[1]);
        password.letter = parts[1].slice(0, 1);
        password.text = parts[2];
        passwords.push(password);
    }
    return passwords;
}
function IsValidPassword(password) {
    var counter = new CharacterCounter();
    for (var i = 0; i < password.text.length; i++) {
        counter.addCount(password.text[i]);
    }
    var count = counter.countForChar(password.letter);
    return count >= password.minCount && count <= password.maxCount;
}
function IsValidPasswordPart2(password) {
    var index1 = password.minCount - 1;
    var index2 = password.maxCount - 1;
    var length = password.text.length;
    if (index1 > length) {
        return false;
    }
    if (index2 > length) {
        return password.text[index1] == password.letter;
    }
    var char1 = password.text[index1];
    var char2 = password.text[index2];
    return (char1 == password.letter || char2 == password.letter) && char1 != char2;
}
fileSystem.readFile(inputFileName, 'utf8', function (error, data) {
    var passwords = ParseTextFile(data);
    var matches = 0;
    for (var _i = 0, passwords_1 = passwords; _i < passwords_1.length; _i++) {
        var password = passwords_1[_i];
        if (IsValidPasswordPart2(password)) {
            matches++;
        }
    }
    console.log(matches);
});
