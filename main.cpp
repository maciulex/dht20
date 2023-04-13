#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

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


#include "dht20.cpp"

#define ALLWAYS_HIGHT 16
#define INPUT_1 17
#define INPUT_2 18



void initMainInputs() {
    gpio_init(ALLWAYS_HIGHT);
    gpio_set_dir(ALLWAYS_HIGHT, GPIO_OUT);
    gpio_put(ALLWAYS_HIGHT, 1);

    gpio_init(INPUT_1);
    gpio_set_dir(INPUT_1, GPIO_IN);

    gpio_init(INPUT_2);
    gpio_set_dir(INPUT_2, GPIO_IN);

    gpio_pull_down(INPUT_1);
    gpio_pull_down(INPUT_2);
}

int main() {
    stdio_init_all();
    initMainInputs();
    
    DHT20::fullINIT();
    bool allInputsLow = true;
    while(true) {
        if (!gpio_get(INPUT_1) && !gpio_get(INPUT_2)) allInputsLow = true;
        
        if (gpio_get(INPUT_1) && allInputsLow) {
            allInputsLow = false;
            uint8_t anwser = DHT20::getStatusWord();
            printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(anwser));
            printf("\n");
        }

        if (gpio_get(INPUT_2) && allInputsLow) {
            allInputsLow = false;
            DHT20::getMeasurement();
        }
    }

    return 0;
}