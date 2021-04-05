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
  let irstateLast:number;
  let irstateTime:number;
  let state:number;

    /**
    * call initialization before first use!
    */
    //% blockId=IR_remote_init
    //% blockGap=20 weight=90
    //% block="connect ir receiver to |pin %pin"
    //% pin.min=0 pin.max=20 pin.defl=1
    //% shim=IR::init
    export function init(pin: number): void {
        return
    }

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
  function irCode(): number {
      return -2;
  }

  function valuotokeyConversion():number{
      let irdata:number = -1;
      let irCd: number = irCode()
      if (irCd != 0) irdata = irCd
      return irdata;
  }

  basic.forever(() => {
      if(state == 1){
          irstate = valuotokeyConversion();
          if(irstate != -1){
              //serial.writeValue("irstate", irstate)
              //serial.writeValue("irstateLast", irstateLast)
              if (irstate != irstateLast)
              {
                  irstateTime = control.millis();
                  control.raiseEvent(11, 22);
              } else {
                  
                  //music.playTone(Note.C, 200)
                  if (control.millis() > irstateTime + 500)
                  {
                      control.raiseEvent(11, 22);
                  }
              }
              irstateLast = irstate;
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