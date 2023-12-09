declare function require(name:string);

const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode2020/2/passwords.txt';

class Password {
  minCount:number;
  maxCount:number;
  letter:string;
  text:string;

  public toString() {
    return String(this.minCount) + '-' + String(this.maxCount) + ' ' + this.letter + ': ' + this.text;
  }
}

class CharacterCounter {
  private counts:{ [id:string] : number};

  constructor() {
    this.counts = {};
  }

  addCount(char:string) {
    let currentCount:number = this.counts[char];
    if (typeof(currentCount) == 'undefined') {
      currentCount = 1;
    } else {
      currentCount++;
    }
    this.counts[char] = currentCount;
  }

  countForChar(char:string) {
    const count:number = this.counts[char];
    if (typeof(count) == 'undefined') {
      return 0;
    }

    return count;
  }
}

function ParseTextFile(data:string) : Password[] {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  const passwords = [];
  for (let line of lines) {
    const parts:string[] = line.split(' ');
    if (parts.length != 3) {
      continue;
    }

    const password:Password = new Password;
    const range:string[] = parts[0].split('-');
    password.minCount = Number(range[0]);
    password.maxCount = Number(range[1]);

    password.letter = parts[1].slice(0, 1);
    password.text = parts[2];

    passwords.push(password);
  }

  return passwords;
}

function IsValidPassword(password:Password) : boolean {
  const counter:CharacterCounter = new CharacterCounter();
  for (let i = 0; i < password.text.length; i++) {
    counter.addCount(password.text[i]);
  }

  const count = counter.countForChar(password.letter);
  return count >= password.minCount && count <= password.maxCount;
}

function IsValidPasswordPart2(password:Password) : boolean {
  const index1 = password.minCount - 1;
  const index2 = password.maxCount - 1;
  const length = password.text.length;
  if (index1 > length) {
    return false;
  }

  if (index2 > length) {
    return password.text[index1] == password.letter;
  }

  const char1 = password.text[index1];
  const char2 = password.text[index2];

  return (char1 == password.letter || char2 == password.letter) && char1 != char2;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const passwords:Password[] = ParseTextFile(data);
  let matches:number = 0;
  for (let password of passwords) {
    if (IsValidPasswordPart2(password)) {
      matches++;
    }
  }
  console.log(matches);
});
