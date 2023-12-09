declare function require(name:string);

interface Set<T> {
  difference(a:Set<T>) : Set<T>;
  union(a:Set<T>) : Set<T>;
}

Set.prototype.difference = function(a) {
  let difference = new Set(this.values());
  a.forEach((value, value1, set) => {
      if (difference.has(value)) {
          difference.delete(value)
      }
  });
  return difference;
}

Set.prototype.union = function(a) {
  let union = new Set();
  a.forEach((value, value1, set) => {
    if (this.has(value)) {
      union.add(value);
    }
  });
  return union;
}

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode2020/4/passports.txt';

const requiredPassportFields:Set<string> = new Set([
  'byr',
  'iyr',
  'eyr',
  'hgt',
  'hcl',
  'ecl',
  'pid',
]);

const optionalPassportFields:Set<string> = new Set([
  'cid',
]);

const validEyeColors:Set<string> = new Set([
  'amb',
  'blu',
  'brn',
  'gry',
  'grn',
  'hzl',
  'oth',
]);

class Passport {
  public fields:{ [id:string] : string };

  constructor() {
    this.fields = {};
  }

  addField(name:string, value:string) {
    this.fields[name] = value;
  }

  hasField(name:string) : boolean {
    return typeof(this.fields[name]) != 'undefined';
  }

  isValid() : boolean {
    const fieldSet:Set<string> = new Set(Object.keys(this.fields));
    const hasRequiredKeys = fieldSet.union(requiredPassportFields).size == requiredPassportFields.size;
    const hasOnlyDefinedKeys = fieldSet.difference(requiredPassportFields).difference(optionalPassportFields).size == 0;
    return hasRequiredKeys && hasOnlyDefinedKeys;
  }

  isValidPart2() : boolean {
    return this.isValidBirthYear() && this.isValidIssueYear() && this.isValidExpirationYear() && this.isValidHeight() &&
      this.isValidHairColor() && this.isValidEyeColor() && this.isValidPassportID();
  }

  isValidBirthYear() : boolean {
    const value:string = this.fields['byr'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    const numberVal:number = Number(value);
    return numberVal >= 1920 && numberVal <= 2002;
  }

  isValidIssueYear() : boolean {
    const value:string = this.fields['iyr'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    const numberVal:number = Number(value);
    return numberVal >= 2010 && numberVal <= 2020;
  }

  isValidExpirationYear() : boolean {
    const value:string = this.fields['eyr'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    const numberVal:number = Number(value);
    return numberVal >= 2020 && numberVal <= 2030;
  }

  isValidHeight() : boolean {
    const value:string = this.fields['hgt'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    let indexOfUnits = value.indexOf('in');
    if (indexOfUnits != -1) {
      const numberVal:number = Number(value.slice(0, indexOfUnits));
      return numberVal >= 59 && numberVal <= 76;
    }

    indexOfUnits = value.indexOf('cm');
    if (indexOfUnits != -1) {
      const numberVal:number = Number(value.slice(0, indexOfUnits));
      return numberVal >= 150 && numberVal <= 193;
    }

    return false;
  }

  isValidHairColor() : boolean {
    const value:string = this.fields['hcl'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    if (value.length != 7) {
      return false;
    }

    let regex = /#[0-9a-f]{6}/;
    return regex.exec(value) != null;
  }

  isValidEyeColor() : boolean {
    const value:string = this.fields['ecl'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    return validEyeColors.has(value);
  }

  isValidPassportID() : boolean {
    const value:string = this.fields['pid'];
    if (typeof(value) == 'undefined') {
      return false;
    }

    if (value.length != 9) {
      return false;
    }

    let regex = /[0-9]{9}/;
    return regex.exec(value) != null;
  }
}

function ParseKeyValuePairs(keyValuePair:string) : string[] {
  const parts = keyValuePair.split(':');
  if (parts.length != 2) {
    return null;
  }

  return parts;
}

function ParsePassports(data:string) : Passport[] {
  const passports:Passport[] = [];
  const lines:string[] = data.split('\n');

  let currentPassport = null;
  for(let line of lines) {
    if (line == '' && currentPassport != null) {
      passports.push(currentPassport);
      currentPassport = null;
    }

    const fields:string[] = line.split(' ');
    for (let field of fields) {
      const keyValuePair:string[] = ParseKeyValuePairs(field);
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

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const passports:Passport[] = ParsePassports(data);
  let validPassportCount = 0;
  for (let passport of passports) {
    if (passport.isValidPart2()) {
      validPassportCount++;
    }
  }
  console.log(validPassportCount);
});
