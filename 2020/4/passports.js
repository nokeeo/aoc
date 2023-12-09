Set.prototype.difference = function (a) {
    let difference = new Set(this.values());
    a.forEach((value, value1, set) => {
        if (difference.has(value)) {
            difference.delete(value);
        }
    });
    return difference;
};
Set.prototype.union = function (a) {
    let union = new Set();
    a.forEach((value, value1, set) => {
        if (this.has(value)) {
            union.add(value);
        }
    });
    return union;
};
const fileSystem = require('fs');
const inputFileName = '/Users/ericlee/Documents/Projects/adventOfCode2020/4/passports.txt';
const requiredPassportFields = new Set([
    'byr',
    'iyr',
    'eyr',
    'hgt',
    'hcl',
    'ecl',
    'pid',
]);
const optionalPassportFields = new Set([
    'cid',
]);
const validEyeColors = new Set([
    'amb',
    'blu',
    'brn',
    'gry',
    'grn',
    'hzl',
    'oth',
]);
class Passport {
    constructor() {
        this.fields = {};
    }
    addField(name, value) {
        this.fields[name] = value;
    }
    hasField(name) {
        return typeof (this.fields[name]) != 'undefined';
    }
    isValid() {
        const fieldSet = new Set(Object.keys(this.fields));
        const hasRequiredKeys = fieldSet.union(requiredPassportFields).size == requiredPassportFields.size;
        const hasOnlyDefinedKeys = fieldSet.difference(requiredPassportFields).difference(optionalPassportFields).size == 0;
        return hasRequiredKeys && hasOnlyDefinedKeys;
    }
    isValidPart2() {
        return this.isValidBirthYear() && this.isValidIssueYear() && this.isValidExpirationYear() && this.isValidHeight() &&
            this.isValidHairColor() && this.isValidEyeColor() && this.isValidPassportID();
    }
    isValidBirthYear() {
        const value = this.fields['byr'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        const numberVal = Number(value);
        return numberVal >= 1920 && numberVal <= 2002;
    }
    isValidIssueYear() {
        const value = this.fields['iyr'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        const numberVal = Number(value);
        return numberVal >= 2010 && numberVal <= 2020;
    }
    isValidExpirationYear() {
        const value = this.fields['eyr'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        const numberVal = Number(value);
        return numberVal >= 2020 && numberVal <= 2030;
    }
    isValidHeight() {
        const value = this.fields['hgt'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        let indexOfUnits = value.indexOf('in');
        if (indexOfUnits != -1) {
            const numberVal = Number(value.slice(0, indexOfUnits));
            return numberVal >= 59 && numberVal <= 76;
        }
        indexOfUnits = value.indexOf('cm');
        if (indexOfUnits != -1) {
            const numberVal = Number(value.slice(0, indexOfUnits));
            return numberVal >= 150 && numberVal <= 193;
        }
        return false;
    }
    isValidHairColor() {
        const value = this.fields['hcl'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        if (value.length != 7) {
            return false;
        }
        let regex = /#[0-9a-f]{6}/;
        return regex.exec(value) != null;
    }
    isValidEyeColor() {
        const value = this.fields['ecl'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        return validEyeColors.has(value);
    }
    isValidPassportID() {
        const value = this.fields['pid'];
        if (typeof (value) == 'undefined') {
            return false;
        }
        if (value.length != 9) {
            return false;
        }
        let regex = /[0-9]{9}/;
        return regex.exec(value) != null;
    }
}
function ParseKeyValuePairs(keyValuePair) {
    const parts = keyValuePair.split(':');
    if (parts.length != 2) {
        return null;
    }
    return parts;
}
function ParsePassports(data) {
    const passports = [];
    const lines = data.split('\n');
    let currentPassport = null;
    for (let line of lines) {
        if (line == '' && currentPassport != null) {
            passports.push(currentPassport);
            currentPassport = null;
        }
        const fields = line.split(' ');
        for (let field of fields) {
            const keyValuePair = ParseKeyValuePairs(field);
            if (keyValuePair) {
                if (currentPassport == null) {
                    currentPassport = new Passport();
                }
                currentPassport.addField(keyValuePair[0], keyValuePair[1]);
            }
        }
    }
    if (currentPassport != null) {
        passports.push(currentPassport);
    }
    return passports;
}
fileSystem.readFile(inputFileName, 'utf8', (error, data) => {
    const passports = ParsePassports(data);
    let validPassportCount = 0;
    for (let passport of passports) {
        if (passport.isValidPart2()) {
            validPassportCount++;
        }
    }
    console.log(validPassportCount);
});
