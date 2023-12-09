import { OutputCompletionBlock } from './IntComputer.js';

export interface OutputAdapterCallback {
  (value:number) : void;
}

export class OutputAdapter {
  private callback:OutputAdapterCallback;
  private outputCompletion:OutputCompletionBlock;
  private value:number;

  constructor() {
    this.callback = null;
    this.value = null;
    this.outputCompletion = null;
  }

  outputValue(value:number, completion:OutputCompletionBlock) {
    if (this.callback === null) {
      this.value = value;
      this.outputCompletion = completion;
    } else {
      completion();
      this.callback(value);
    }
  }

  public setCallback(callback:OutputAdapterCallback) {
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
