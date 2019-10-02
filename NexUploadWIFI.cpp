/**
 * @file NexUploadWIFI.cpp
 *
 * The implementation of download tft file for nextion. 
 *
 * @author  Chen Zengpeng (email:<zengpeng.chen@itead.cc>), Bogdan Symchych (email:<bogdan.symchych@gmail.com>)
 * @date    2019/10/1
 *
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "NexUploadWIFI.h"
//#include <SoftwareSerial.h>

//#define USE_SOFTWARE_SERIAL
#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial dbSerial(3, 2); /* RX:D3, TX:D2 */
#define DEBUG_SERIAL_ENABLE
#endif

#ifdef DEBUG_SERIAL_ENABLE
#define dbSerialPrint(a)    dbSerial.print(a)
#define dbSerialPrintln(a)  dbSerial.println(a)
#define dbSerialBegin(a)    dbSerial.begin(a)
#else
#define dbSerialPrint(a)    do{}while(0)
#define dbSerialPrintln(a)  do{}while(0)
#define dbSerialBegin(a)    do{}while(0)
#endif

NexUploadWIFI::NexUploadWIFI(uint32_t download_baudrate)
{
    _download_baudrate = download_baudrate;
}

void NexUploadWIFI::check(uint32_t size)
{
    _undownloadByte = size;
    dbSerialPrintln("tft file size is:");
    dbSerialPrintln(_undownloadByte);
    dbSerialPrintln("check file ok");

    dbSerialBegin(115200);
    if(_getBaudrate() == 0)
    {
        dbSerialPrintln("get baudrate error");
        return;
    }
    if(!_setDownloadBaudrate(_download_baudrate))
    {
        dbSerialPrintln("modify baudrate error");
        return;
    }
}

uint16_t NexUploadWIFI::_getBaudrate(void)
{
    uint32_t baudrate_array[7] = {115200,19200,9600,57600,38400,4800,2400};
    for(uint8_t i = 0; i < 7; i++)
    {
        if(_searchBaudrate(baudrate_array[i]))
        {
            _baudrate = baudrate_array[i];
            dbSerialPrintln("get baudrate");
            break;
        }
    }
    return _baudrate;
}

bool NexUploadWIFI::_searchBaudrate(uint32_t baudrate)
{
    String string = String("");  
    nexSerial.begin(baudrate);
    this->sendCommand("");
    this->sendCommand("connect");
    this->recvRetString(string);  
    if(string.indexOf("comok") != -1)
    {
        return 1;
    } 
    return 0;
}

void NexUploadWIFI::sendCommand(const char* cmd)
{

    while (nexSerial.available())
    {
        nexSerial.read();
    }

    nexSerial.print(cmd);
    nexSerial.write(0xFF);
    nexSerial.write(0xFF);
    nexSerial.write(0xFF);
}

uint16_t NexUploadWIFI::recvRetString(String &string, uint32_t timeout,bool recv_flag)
{
    uint16_t ret = 0;
    uint8_t c = 0;
    long start;
    bool exit_flag = false;
    start = millis();
    while (millis() - start <= timeout)
    {
        while (nexSerial.available())
        {
            c = nexSerial.read(); 
            if(c == 0)
            {
                continue;
            }
            string += (char)c;
            if(recv_flag)
            {
                if(string.indexOf(0x05) != -1)
                { 
                    exit_flag = true;
                } 
            }
        }
        if(exit_flag)
        {
            break;
        }
    }
    ret = string.length();
    return ret;
}

bool NexUploadWIFI::_setDownloadBaudrate(uint32_t baudrate)
{
    String string = String(""); 
    String cmd = String("");
    
    String filesize_str = String(_undownloadByte,10);
    String baudrate_str = String(baudrate,10);
    cmd = "whmi-wri " + filesize_str + "," + baudrate_str + ",0";
    
    dbSerialPrintln(cmd);
    this->sendCommand("");
    this->sendCommand(cmd.c_str());
    delay(50);
    nexSerial.begin(baudrate);
    this->recvRetString(string,500);  
    if(string.indexOf(0x05) != -1)
    { 
        return 1;
    } 
    return 0;
}

bool NexUploadWIFI::uploadTftFile(uint8_t * data, size_t len)
{
    String string = String("");

    for(uint16_t i = 0; i < len; i++)
    {
      nexSerial.write(data[i]);
      _uploaded_bytes++;

      if (_uploaded_bytes == 4096) {
        this->recvRetString(string,500,true);  
        if(string.indexOf(0x05) != -1)
        { 
            string = "";
        } 
        else
        {
            dbSerialPrintln("Upload error");
            return 0;
        }
        _uploaded_bytes = 0;
      }
      
    }
}


