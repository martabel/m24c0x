#ifndef M24C0x_H
#define M24C0x_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

class M24C0x {
    public:
    M24C0x(i2c_inst_t *i2c, uint8_t i2c_address, uint8_t i2c_sda, uint8_t i2c_scl, uint8_t wc_pin);
    void init();
    void deinit();
    bool write_bytes(uint8_t start_address, uint8_t *data, uint8_t length);
    bool read_bytes(uint8_t start_address, uint8_t *data, uint8_t length);

    private:
    i2c_inst_t* i2c;
    uint8_t i2c_address;
    uint8_t wc_pin;
    bool isInitialized;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t buffer[256] = {0};
};

#endif