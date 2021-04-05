#include "pxt.h"
using namespace pxt;

//% color=#008B00 weight=11
//% icon="\uf1eb"
//% block="IR remote"
namespace IR
{
    int ir_code = 0x00;
    int ir_addr = 0x00;
    int data;
    
    int logic_value(int pin_no)
    {
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while (!uBit.io.pin[pin_no].getDigitalValue());
        nowtime = system_timer_current_time_us();
        if ((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700)
        {
            while (uBit.io.pin[pin_no].getDigitalValue());
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

    void pulse_deal(int pin_no)
    {
        int i;
        ir_addr = 0x00;
        for (i = 0; i < 16; i++)
        {
            if (logic_value(pin_no) == 1)
            {
                ir_addr |= (1 << i);
            }
        }

        ir_code = 0x00;
        for (i = 0; i < 16; i++)
        {
            if (logic_value(pin_no) == 1)
            {
                ir_code |= (1 << i);
            }
        }
    }

    void remote_decode(int pin_no)
    {
        data = 0x00;
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
        while (uBit.io.pin[pin_no].getDigitalValue())
        {
            nowtime = system_timer_current_time_us();
            if ((nowtime - lasttime) > 100000)
            {
                ir_code = 0xff00;
                return;
            }
        }

        lasttime = system_timer_current_time_us();
        while (!uBit.io.pin[pin_no].getDigitalValue());
        nowtime = system_timer_current_time_us();
        if ((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000)
        {
            while (uBit.io.pin[pin_no].getDigitalValue());
            lasttime = system_timer_current_time_us();
            if ((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000)
            {
                pulse_deal(pin_no);
                //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                data = ir_code;
                return; //ir_code;
            }
            else if ((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500)
            {
                while (!uBit.io.pin[pin_no].getDigitalValue());
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
    int irCode(int pin_no)
    {
        remote_decode(pin_no);
        return data;
    }

}
