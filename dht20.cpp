#ifndef DHT20
#define DHT20

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#ifndef INT_TO_BIN
#define INT_TO_BIN
    #define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
    #define BYTE_TO_BINARY(byte)  \
    (byte & 0x80 ? '1' : '0'), \
    (byte & 0x40 ? '1' : '0'), \
    (byte & 0x20 ? '1' : '0'), \
    (byte & 0x10 ? '1' : '0'), \
    (byte & 0x08 ? '1' : '0'), \
    (byte & 0x04 ? '1' : '0'), \
    (byte & 0x02 ? '1' : '0'), \
    (byte & 0x01 ? '1' : '0') 
#endif

#ifndef I2C_INTERFACE
    #define I2C_INTERFACE 
    #define I2C_PORT i2c0
    #define I2C_PIN_SDA 4
    #define I2C_PIN_SCL 5
    #define I2C_CLOCK 50*1000
#endif


namespace DHT20 {
    uint8_t i2c_address = 0b0011'1000;
    uint8_t read_bit    = 0b1000'0000;
    
    double result[2] = {0x00, 0x00};
    bool print = false;

    void I2Cinit() {
        i2c_init(I2C_PORT, I2C_CLOCK);
        gpio_set_function(I2C_PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(I2C_PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(I2C_PIN_SDA);
        gpio_pull_up(I2C_PIN_SCL);

    }

    uint8_t getStatusWord() {
        uint8_t command[1] {0x71};
        uint8_t anwser[1] {0x00};
        i2c_write_blocking(I2C_PORT, i2c_address, command, 1, true);
        i2c_read_blocking(I2C_PORT, i2c_address, anwser, 1, false);
        return anwser[0];
    }

    void getMeasurement() {
        uint8_t command[3] {0xAC, 0x33, 0x00};
        uint8_t status;
        i2c_write_blocking(I2C_PORT, i2c_address, command, 3, false);

        sleep_ms(80);
        while (true) {
            status = getStatusWord();
            sleep_ms(50);
            if (!(status & 0x80)) break;
        }

        uint8_t data[7];
        i2c_read_blocking(I2C_PORT, i2c_address | read_bit, data, 7, false);
        for (int i = 0; i < 7; i++) {
            if (!print) break;
            printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(data[i]));
            printf("\n");
        }
        double srh = (uint32_t)data[1] << 12 | (uint32_t)data[2] << 4 | (uint32_t)data[3] >> 4;
        srh = srh/1048576;
        srh *= 100;
        double tmp = (((uint32_t)data[3]) << 16 | ((uint32_t)data[4]) << 8 | ((uint32_t)data[5])) & 0xfffff;
        tmp = tmp/1048576;
        tmp *= 200;
        tmp -= 50;
        result[0] = tmp;
        result[1] = srh;
    }

    void fullINIT() {
        uint8_t status;
        I2Cinit();  
        status = getStatusWord();
        if (print){
            printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(status));
            printf("\n");
        }
        sleep_ms(40);
        while (true) {
            status = getStatusWord();
            if (print){
                printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(status));
                printf("\n");
            }
            if (status & 0x08) {
                break;
            }
            uint8_t command[1] {0xBE};
            i2c_write_blocking(I2C_PORT, i2c_address, command, 1, true);
            sleep_ms(10);

        }      
    }

}

#endif