import { InputCompletionBlock } from './IntComputer.js';

/** Implements IntComputer.Input */
export class InputAdapter {
  private values:number[];
  private pendingCompletion:InputCompletionBlock;

  constructor() {
    this.values = [];
    this.pendingCompletion = null;
  }

  getNextInput(completion:InputCompletionBlock) {
    if (this.values.length > 0) {
      const value = this.values.splice(0, 1)[0];
      completion(value);
    } else {
      this.pendingCompletion = completion;
    }
  }

  public setValue(value:number) {
    if (this.pendingCompletion !== null) {
      const completion = this.pendingCompletion;
      this.pendingCompletion = null;
      completion(value);
    } else {
      this.values.push(value);
    }
  }
}
