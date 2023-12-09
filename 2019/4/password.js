function IsValidLength(num) {
    return Math.ceil(Math.log10(num)) === 6;
}
function IsAdjacentDigitEqual(num) {
    const str = String(num);
    if (str.length < 2) {
        return false;
    }
    let lastChar = str[0];
    let charEqualCount = 0;
    for (let i = 1; i < str.length; i++) {
        const char = str[i];
        if (char === lastChar) {
            charEqualCount++;
        }
        else if (charEqualCount == 1) {
            return true;
        }
        else {
            charEqualCount = 0;
        }
        lastChar = char;
    }
    if (charEqualCount === 1) {
        return true;
    }
    return false;
}
function AreDigitsIncreasing(num) {
    const str = String(num);
    if (str.length < 2) {
        return true;
    }
    let lastChar = str[0];
    for (let i = 1; i < str.length; i++) {
        const char = str[i];
        if (char < lastChar) {
            return false;
        }
        lastChar = char;
    }
    return true;
}
let count = 0;
for (let i = 245182; i <= 790572; i++) {
    if (IsValidLength(i) && IsAdjacentDigitEqual(i) && AreDigitsIncreasing(i)) {
        count++;
    }
}
console.log(count);
