/*
Copyright (C): 2020, Shenzhen Yahboom Tech
modified by chengengyue
modified by TomasKazda
*/

//% color=#008B00 weight=11
//% icon="\uf1eb"
//% block="IR remote"
namespace IR {
  let irstate:number;
  let state:number;


  //% blockId=IR_remote_read block="read IR key value"
  export function IR_read(): number {
      if (state == 1) return irstate
      return valuotokeyConversion()
  }

  //% blockId=IR_remote_readHEX block="read IR hex value"
  export function IR_readHex(): string {
      if (state == 1) return to16BitHex(irstate)
      return to16BitHex(valuotokeyConversion());
  }

  //% blockId=IR_remote_callback block="on IR received"
  //% draggableParameters
  export function onIR_received(cb: (message: number) => void) {
      state = 1;
      control.onEvent(11, 22, function() {
          cb(irstate);
      }) 
  }

  //% advanced=true shim=IR::irCode
  function irCode(pinNo: number): number {
      return -2;
  }

  function valuotokeyConversion():number{
      let irdata:number = -1;
      let irCd: number = irCode(1)
      if (irCd != 0) irdata = irCd
      return irdata;
  }

  basic.forever(() => {
      if(state == 1){
          irstate = valuotokeyConversion();
          if(irstate != -1){
              control.raiseEvent(11, 22);
          }
      }
      
      basic.pause(20);
  })

  function to16BitHex(value: number): string {
    let hex = "";
    for (let pos = 0; pos < 4; pos++) {
      let remainder = value % 16;
      if (remainder < 10) {
        hex = remainder.toString() + hex;
      } else {
        hex = String.fromCharCode(55 + remainder) + hex;
      }
      value = Math.idiv(value, 16);
    }
    return hex;
  }
}