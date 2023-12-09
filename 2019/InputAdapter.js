"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/** Implements IntComputer.Input */
class InputAdapter {
    constructor() {
        this.values = [];
        this.pendingCompletion = null;
    }
    getNextInput(completion) {
        if (this.values.length > 0) {
            const value = this.values.splice(0, 1)[0];
            completion(value);
        }
        else {
            this.pendingCompletion = completion;
        }
    }
    setValue(value) {
        if (this.pendingCompletion !== null) {
            const completion = this.pendingCompletion;
            this.pendingCompletion = null;
            completion(value);
        }
        else {
            this.values.push(value);
        }
    }
}
exports.InputAdapter = InputAdapter;
