/**
 * @file NexUpload.h
 *
 * The definition of class NexUpload. 
 *
 * @author Chen Zengpeng (email:<zengpeng.chen@itead.cc>)
 * @date 2016/3/29
 *
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#ifndef __NEXUPLOAD_H__
#define __NEXUPLOAD_H__
#include <Arduino.h>
#include <SPI.h>
#include "SPIFFS.h"
#include "NexHardware.h"

/**
 * @addtogroup CoreAPI 
 * @{ 
 */

/**
 *
 * Provides the API for nextion to download the ftf file.
 */
class NexUploadWIFI
{
public: /* methods */

    /**
     * Constructor. 
     * 
     * @param file_name - tft file name. 
     * @param SD_chip_select - sd chip select pin.
     * @param download_baudrate - set download baudrate.
     */
    NexUploadWIFI(uint32_t download_baudrate);

    /**
     * destructor. 
     * 
     */
    ~NexUploadWIFI(){}
    
    /*
     * start download.
     *
     * @return none.
     */
    void check(uint32_t size);

    /**
     * start dowload tft file to nextion. 
     * 
     * @return none. 
     */
    bool uploadTftFile(uint8_t * toDecode, size_t len);
    
private: /* methods */

    /*
     * get communicate baudrate. 
     * 
     * @return communicate baudrate.
     *
     */
    uint16_t _getBaudrate(void);
    
    /*
     * check tft file.
     *
     * @return true if success, false for failure. 
     */
    bool _checkFile(void);

    /*
     * search communicate baudrate.
     *
     * @param baudrate - communicate baudrate.
     *   
     * @return true if success, false for failure. 
     */
    bool _searchBaudrate(uint32_t baudrate);

    /*
     * set download baudrate.
     *
     * @param baudrate - set download baudrate.
     *   
     * @return true if success, false for failure. 
     */
    bool _setDownloadBaudrate(uint32_t baudrate);
    
    /*
     * Send command to Nextion.
     *
     * @param cmd - the string of command.
     *
     * @return none.
     */
    void sendCommand(const char* cmd);

    /*
     * Receive string data. 
     * 
     * @param buffer - save string data.  
     * @param timeout - set timeout time. 
     * @param recv_flag - if recv_flag is true,will braak when receive 0x05.
     *
     * @return the length of string buffer.
     *
     */   
    uint16_t recvRetString(String &string, uint32_t timeout = 100,bool recv_flag = false);
    
private: /* data */ 
    uint32_t _baudrate; /*nextion serail baudrate*/
    uint32_t _undownloadByte; /*undownload byte of tft file*/
    uint32_t _download_baudrate; /*download baudrate*/
    uint32_t _uploaded_bytes = 0; /*counter of uploaded bytes*/
};
/**
 * @}
 */

#endif /* #ifndef __NEXDOWNLOAD_H__ */
