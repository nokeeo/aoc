declare function require(name:string);
const fileSystem = require('fs');
const inputFileName:string = '/Users/ericlee/Documents/Projects/adventOfCode/2021/3/diag.txt';

function FindOxNumber(lines:string[]):number {
  for (let i = 0; i < lines[0].length; i++) {
    let oneCount = 0;
    for (let line of lines) {
      if (line.charAt(i) === '1') {
        oneCount++;
      }
    }

    let filterValue = oneCount >= (lines.length / 2) ? '1' : '0';
    lines = lines.filter((line) => {
      return line.charAt(i) !== filterValue;
    });

    if (lines.length === 1) {
      return parseInt(lines[0], 2);
    }
  }

  return 0;
}

function FindCo2Number(lines:string[]):number {
  for (let i = 0; i < lines[0].length; i++) {
    let oneCount = 0;
    for (let line of lines) {
      if (line.charAt(i) === '1') {
        oneCount++;
      }
    }

    let filterValue = oneCount >= (lines.length / 2) ? '1' : '0';
    lines = lines.filter((line) => {
      return line.charAt(i) === filterValue;
    });

    if (lines.length === 1) {
      return parseInt(lines[0], 2);
    }
  }

  return 0;
}

fileSystem.readFile(inputFileName, 'utf8', (error:Error, data:string) => {
  const lines:string[] = data.split('\n').filter((line:string) => {
    return line != '';
  });

  // let gama = '';
  // let epsilon = '';
  //
  // const bitLength = lines[0].length;
  // for (let i = 0; i < bitLength; i++) {
  //   let oneCount = 0;
  //   for (let line of lines) {
  //     if (line.charAt(i) === '1') {
  //       oneCount++;
  //     }
  //   }
  //
  //   if (oneCount > (lines.length / 2)) {
  //     gama += '1';
  //     epsilon += '0';
  //   } else {
  //     gama += '0';
  //     epsilon += '1';
  //   }
  // }
  //
  // console.log(gama);
  // console.log(epsilon);
  // const gamaDec = parseInt(gama, 2);
  // const epsilonDec = parseInt(epsilon, 2);
  //
  // console.log(gamaDec * epsilonDec);

  console.log(FindOxNumber(lines) * FindCo2Number(lines));
});
