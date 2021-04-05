/**
 * OSOYOO_IR (Version 2.0.0)
 *
 * Copyright (C) 2020 Yahboom
 * 
 */
#include "pxt.h"
#include <map>
#include <vector>
#include "ReceiverIR.h"
using namespace pxt;
typedef vector<Action> vA;



enum class Pins{
    P0=  2,
    P1=  3,
    P2=  4,
    P3=  31,
    P4=  28,
    P5=  14,
    P6=  5,
    P7=  11,
    P8=  10,
    P9=  9,
    P10= 30,
    P11= 23,
    P12= 12,
    P13= 17,
    P14= 1,
    P15= 13,
    P16= 2,
    P19= 26,
    P20= 00
};

enum class RemoteButton {
      CH_MINUS = 0x45,
	  CH = 0x46,
	  CH_Add = 0x47,
	  PREV = 0x44,
	  PLAY = 0x43,
	  NUM_200 = 0xd,
	  NEXT = 0x40,
	  NUM_100 = 0x19,
	  Minus = 0x7,
	  Add = 0x15,
	  EQ = 0x9,
	  NUM0 = 0x16,
	  NUM1 = 0xc,
	  NUM2 = 0x18,
	  NUM3 = 0x5e,
	  NUM4 = 0x8,
	  NUM5 = 0x1c,
	  NUM6 = 0x5a,
	  NUM7 = 0x42,
	  NUM8 = 0x52,
	  NUM9 = 0x4A,
};

//% color=50 weight=11
//% icon="\uf1eb"
//% block="OSOYOO_IR_V1.5"
namespace Bit_IR { 

    map<RemoteButton, vA> actions;
    map<RemoteButton, uint32_t> lastact;
    Timer tsb; 
    uint8_t buf[32];
    uint32_t now;
    ReceiverIR *rx;
    RemoteIR::Format fmt = RemoteIR::UNKNOWN;
    Pins pin;

    /*
    V2 logic
    */
    int ir_code = 0x00;
    int ir_addr = 0x00;
    int data;
    MicroBitPin* pIRV2_pin = NULL;


    void cA(vA runner){for(int i=0;i<runner.size();i++){runAction0(runner[i]);} }

    void onReceivable(){
        int x = rx->getData(&fmt, buf, 32 * 8);
        if(actions.find((RemoteButton)buf[2]) == actions.end()) return;
        now = tsb.read_ms();
        if(now - lastact[(RemoteButton)buf[2]] < 100) return;
        lastact[(RemoteButton)buf[2]] = now;
        cA(actions[(RemoteButton)buf[2]]);
    }


    void monitorIR(){
        while(1){
        while(rx->getState() != ReceiverIR::Received){
            uBit.sleep(50);
        }
        onReceivable();
        }
    }

    //%
    void init(Pins pin){
        rx = new ReceiverIR((PinName)pin);
        tsb.start(); //interrupt timer for debounce
        create_fiber(monitorIR);
    }

    //%
    void onPressEvent(RemoteButton btn, Action body) {
        actions[btn].push_back(body);
    }

    
     
  

    /****************************************************************************************************
        OSOYOO_IRV2
    ****************************************************************************************************/
  
    int logic_value(){
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while(!uBit.io.P1.getDigitalValue());
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700){
            while(uBit.io.P1.getDigitalValue());
            lasttime = system_timer_current_time_us();
            if((lasttime - nowtime)>400 && (lasttime - nowtime) < 700){
                return 0;
            }else if((lasttime - nowtime)>1500 && (lasttime - nowtime) < 1800){
                return 1;
        }
        }
        uBit.serial.printf("error\r\n");
        return -1;
    }

    void pulse_deal(){
        int i;
        ir_addr=0x00;
        for(i=0; i<16;i++ )
        {
            if(logic_value() == 1)
            {
                ir_addr |=(1<<i);
            }
        }
        /
        ir_code=0x00;
        for(i=0; i<16;i++ )
        {
            if(logic_value() == 1)
            {
                ir_code |=(1<<i);
            }
        }

    }

    void remote_decode(void){
        data = 0x00;
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while(uBit.io.P1.getDigitalValue()){
            nowtime = system_timer_current_time_us();
            if((nowtime - lasttime) > 100000){/
                ir_code = 0xff00;
                return;
            }
        }
        
        lasttime = system_timer_current_time_us();
        while(!uBit.io.P1.getDigitalValue());
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000){
            while(uBit.io.P1.getDigitalValue());
            lasttime = system_timer_current_time_us();
            if((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000){
                pulse_deal();
                //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                data = ir_code;
                return;//ir_code;
            }else if((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500){
                while(!uBit.io.P1.getDigitalValue());
                nowtime = system_timer_current_time_us();
                if((nowtime - lasttime) > 500 && (nowtime - lasttime) < 700){
                    //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                    data = ir_code;
                    return;//ir_code;
                }
            }
        }
    }

    //% 
    int irCode(){
        remote_decode();
        return data;
    }
  
}
