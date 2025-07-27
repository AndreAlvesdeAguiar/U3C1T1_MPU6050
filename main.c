#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"
#include "mpu6050.h"

int main()
{
    stdio_init_all();
    mpu6050_init();
    display_init();
    int16_t ax, ay, az;
    char buffer1[20], buffer2[20], buffer3[20];

   
   
    while (true) {
        mpu6050_read_raw(&ax, &ay, &az);
        sprintf(buffer1, "ax: %d ", ax);
        sprintf(buffer2, "ay: %d ", ay);
        sprintf(buffer3, "az: %d ", az);
        display_status_msg(buffer1);
        display_status_msg(buffer2);
        display_status_msg(buffer3);
        
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
