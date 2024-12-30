#include "src/m24c0x.hpp"

#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define WC_PIN 13   
#define I2C_ADDR 0x50

#define B_SIZE 50

int main()
{
    char input = 0;

    M24C0x eeprom(I2C_PORT, I2C_ADDR, I2C_SDA, I2C_SCL, WC_PIN);

    eeprom.init();

    uint8_t buffer[B_SIZE] = {0};

    while (true) 
    {
        //clear buffer
        for(int i=0;i<B_SIZE;i++)
        {
            buffer[i] = 0;
        }
        printf("\nType 'r' for read and 'w' for write test\n");
        input = getchar();
        if(input == 'r')
        {
            if(!eeprom.read_bytes(0, buffer, B_SIZE)) 
            {
                printf("\nError while read from the eeprom\n");
            }
            printf("Read: %s\n", buffer);
            /*
            for(int i=0;i<B_SIZE;i++)
            {
                printf("%#x ", buffer[i]);
            }
            */
            printf("\n");
        }else if(input == 'w')
        {
            printf("Enter string to write: ");
            scanf("%49s", buffer);
            printf("\nWrite: %s\n", buffer);
            if(!eeprom.write_bytes(0, buffer, B_SIZE))
            {
                printf("Error while write to the eeprom\n");
            }
            printf("Write done.\n");
        }else
        {
            printf("\nNo valid command entered!\n");
        }

        sleep_ms(1000);
    }
}