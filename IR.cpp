/**
* OSOYOO_IR (Version 2.0.0)
*
* Copyright (C) 2020 Yahboom
* 
*/
#include "pxt.h"
using namespace pxt;
// enum class Pins
// {
//     P0 = 2,
//     P1 = 3,
//     P2 = 4,
//     P3 = 31,
//     P4 = 28,
//     P5 = 14,
//     P6 = 5,
//     P7 = 11,
//     P8 = 10,
//     P9 = 9,
//     P10 = 30,
//     P11 = 23,
//     P12 = 12,
//     P13 = 17,
//     P14 = 1,
//     P15 = 13,
//     P16 = 2,
//     P19 = 26,
//     P20 = 00
// };

//% color=#008B00 weight=11
//% icon="\uf1eb"
//% block="IR remote"
namespace IR
{
    //Pins pin;
    int ir_code = 0x00;
    int ir_addr = 0x00;
    int data;
    //MicroBitPin* pIRV2_pin = NULL;

    int logic_value()
    {
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while (!uBit.io.P1.getDigitalValue())
            ;
        nowtime = system_timer_current_time_us();
        if ((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700)
        {
            while (uBit.io.P1.getDigitalValue())
                ;
            lasttime = system_timer_current_time_us();
            if ((lasttime - nowtime) > 400 && (lasttime - nowtime) < 700)
            {
                return 0;
            }
            else if ((lasttime - nowtime) > 1500 && (lasttime - nowtime) < 1800)
            {
                return 1;
            }
        }
        //uBit.serial.printf("error\r\n");
        return -1;
    }

    void pulse_deal()
    {
        int i;
        ir_addr = 0x00;
        for (i = 0; i < 16; i++)
        {
            if (logic_value() == 1)
            {
                ir_addr |= (1 << i);
            }
        }

        ir_code = 0x00;
        for (i = 0; i < 16; i++)
        {
            if (logic_value() == 1)
            {
                ir_code |= (1 << i);
            }
        }
    }

    void remote_decode(void)
    {
        data = 0x00;
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while (uBit.io.P1.getDigitalValue())
        {
            nowtime = system_timer_current_time_us();
            if ((nowtime - lasttime) > 100000)
            {
                ir_code = 0xff00;
                return;
            }
        }

        lasttime = system_timer_current_time_us();
        while (!uBit.io.P1.getDigitalValue())
            ;
        nowtime = system_timer_current_time_us();
        if ((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000)
        {
            while (uBit.io.P1.getDigitalValue())
                ;
            lasttime = system_timer_current_time_us();
            if ((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000)
            {
                pulse_deal();
                //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                data = ir_code;
                return; //ir_code;
            }
            else if ((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500)
            {
                while (!uBit.io.P1.getDigitalValue())
                    ;
                nowtime = system_timer_current_time_us();
                if ((nowtime - lasttime) > 500 && (nowtime - lasttime) < 700)
                {
                    //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                    data = ir_code;
                    return; //ir_code;
                }
            }
        }
    }

    //%
    int irCode()
    {
        remote_decode();
        return data;
    }

}
