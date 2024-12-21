#include "src/m24c0x.hpp"

#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define WC_PIN 13   
#define I2C_ADDR 0x50


int main()
{
    M24C0x eeprom(I2C_PORT, I2C_ADDR, I2C_SDA, I2C_SCL, WC_PIN);

    eeprom.init();


    uint8_t wBytes[10] ={11, 22, 33, 44, 55, 66, 77, 88, 99, 100};
    if(!eeprom.write_bytes(0, wBytes, 10)) {
        printf("\nError while write to the eeprom\n");
    }

    while (true) {
        printf("\nRead bytes\n");
        uint8_t rBytes[5] = {0};

        if(!eeprom.read_bytes(0, rBytes, 5)) {
            printf("\nError while read from the eeprom\n");
        }

        for(uint8_t i=0; i<5; i++) {
            printf("Address: %d Value: %d\n", i, rBytes[i]);
        }

        sleep_ms(2000);
    }
}