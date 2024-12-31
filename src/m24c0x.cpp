#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "m24c0x.hpp"

M24C0x::M24C0x(i2c_inst_t *i2c, uint8_t i2c_address, uint8_t i2c_sda, uint8_t i2c_scl, uint8_t wc_pin){
    this->i2c = i2c;
    this->i2c_address = i2c_address;
    this->isInitialized = false;
    this->i2c_sda = i2c_sda;
    this->i2c_scl = i2c_scl;
}

void M24C0x::init(){
    stdio_init_all();

    // init ic2 with 100khz
    i2c_init(this->i2c, 100*1000);
    
    gpio_set_function(this->i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(this->i2c_scl, GPIO_FUNC_I2C);
    gpio_set_pulls(this->i2c_sda, true, false);
    gpio_set_pulls(this->i2c_scl, true, false);
    gpio_init(this->wc_pin);
    gpio_set_dir(this->wc_pin, true);

    i2c_set_slave_mode(this->i2c, false, 0x00);

    isInitialized = true;

    return;
}

void M24C0x::deinit(){
    i2c_deinit(this->i2c);
}

bool M24C0x::write_bytes(uint8_t start_address, uint8_t *data, uint8_t length){
    int ret;
    uint8_t dataIdx = 0;
    uint8_t addressCnt = start_address;
    if(!isInitialized) {
        this->init();
    }

    // todo write bytes in pages instead of single bytes  
    // fill buffer with eeprom address [0] + data[x]
    //this->buffer[0] = start_address;
    // write given data after the address byte in the buffer
    // memcpy(this->buffer+1, data, length);

    // set inverted write control for write access
    gpio_put(this->wc_pin, false);
    for(; dataIdx < length; dataIdx++)
    {
        this->buffer[0] = addressCnt;
        this->buffer[1] = data[dataIdx];
        addressCnt++;
        // write address and data byte with stop
        ret = i2c_write_blocking(this->i2c, this->i2c_address, this->buffer, 2, false);
        // sleep max write time for each byte
        sleep_ms(5);
    }
    // reset write control
    gpio_put(this->wc_pin, true);

    if (ret != PICO_ERROR_GENERIC) {
        return true;    
    }else {
        return false;
    }
}

bool M24C0x::read_bytes(uint8_t start_address, uint8_t *data, uint8_t length){
    if(!isInitialized) {
        this->init();
    }
    int ret;
    // set inverted write control for read access
    gpio_put(this->wc_pin, true);
    sleep_ms(1);
    // write address without stop
    ret = i2c_write_blocking(this->i2c, this->i2c_address, &start_address, 1, true);
    // read values with stop
    ret = i2c_read_blocking(this->i2c, this->i2c_address, this->buffer, length, false);
    sleep_ms(1);
    if (ret != PICO_ERROR_GENERIC) {
        // copy values to output buffer
        memcpy(data, this->buffer, length);
        return true;    
    }else {
        return false;
    }
}
