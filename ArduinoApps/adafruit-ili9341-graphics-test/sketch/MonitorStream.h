/*
    This file is part of the Arduino_RouterBridge library.

    Copyright (c) 2025 Arduino SA

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/


#ifndef MONITOR_STREAM_H
#define MONITOR_STREAM_H

#include <Arduino_RouterBridge.h>

class MonitorStream: public Stream {
//protected:
public:
    enum {TX_BUFFER_SIZE = 128};
  // tx Buffer
    struct k_mutex monitor_mutex{};
    char txBuffer[TX_BUFFER_SIZE+1];
    uint16_t txBufferCnt = 0;
  
public:
    explicit MonitorStream() {}

    using Print::write;

    bool begin(unsigned long _legacy_baud=0, uint16_t _legacy_config=0) {
	(void)_legacy_baud;
	(void)_legacy_config;
        printk("MS::begin()\n");
        k_mutex_init(&monitor_mutex);
        //Bridge.begin();
        Monitor.begin();
        printk("\tAfter Monitor begin\n");
        txBufferCnt = 0;
        return true;
    }

#if 0
    bool is_connected() {
        k_mutex_lock(&monitor_mutex, K_FOREVER);
        bool out = _connected;
        k_mutex_unlock(&monitor_mutex);
        return out;
    }
#endif
//    explicit operator bool() {
//        return is_connected();
//    }

    int read() override {
        return Monitor.read();
    }


    int available() override {
      return Monitor.available();
      //return 0;
    }

    int peek() override {
        return Monitor.peek();
    }

    size_t write(uint8_t c) override {
        k_mutex_lock(&monitor_mutex, K_FOREVER);
        txBuffer[txBufferCnt++] = c;
        if (txBufferCnt == TX_BUFFER_SIZE) _send_buffer();
        k_mutex_unlock(&monitor_mutex);
        return 1;
    }

    void flush() {
        k_mutex_lock(&monitor_mutex, K_FOREVER);
        if (txBufferCnt) _send_buffer();
        k_mutex_unlock(&monitor_mutex);
    }

  
    void _send_buffer() {
      // assumes we have the buffer locked
      txBuffer[txBufferCnt] = '\0';
      printk("SB: %s\n", txBuffer);
      String str = txBuffer;
      //Bridge.notify("bridge_print", str);
      //Monitor.print(str);
      Monitor.print(txBuffer);
      txBufferCnt = 0;
    }

  
};


MonitorStream MSerial;


#endif // BRIDGE_MONITOR_H
