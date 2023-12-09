"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class OutputAdapter {
    constructor() {
        this.callback = null;
        this.value = null;
        this.outputCompletion = null;
    }
    outputValue(value, completion) {
        if (this.callback === null) {
            this.value = value;
            this.outputCompletion = completion;
        }
        else {
            completion();
            this.callback(value);
        }
    }
    setCallback(callback) {
        if (this.value !== null) {
            const value = this.value;
            const completion = this.outputCompletion;
            this.value = null;
            this.outputCompletion = null;
            completion();
            callback(value);
        }
        this.callback = callback;
    }
}
exports.OutputAdapter = OutputAdapter;
