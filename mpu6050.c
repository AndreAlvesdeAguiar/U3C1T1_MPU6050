#include "mpu6050.h"

// Inicializa o sensor MPU6050 e o barramento I2C
void mpu6050_init(void) {
    // Inicializa o I2C0 com frequência de 400kHz
    i2c_init(i2c0, 400 * 1000);
    // Configura os pinos SDA e SCL para função I2C
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    // Ativa pull-up nos pinos SDA e SCL
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    // Envia comando para sair do modo de sleep (Power Management register)
    uint8_t reset[2] = {0x6B, 0x00}; // 0x6B = endereço do registrador de gerenciamento de energia
    i2c_write_blocking(i2c0, MPU6050_ADDR, reset, 2, false);
}

// Lê os valores brutos do acelerômetro (eixos X, Y, Z)
void mpu6050_read_raw(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];
    // Solicita leitura a partir do registrador ACCEL_XOUT_H (0x3B)
    i2c_write_blocking(i2c0, MPU6050_ADDR, (uint8_t[]){0x3B}, 1, true); // ACCEL_XOUT_H
    // Lê 6 bytes consecutivos (XH, XL, YH, YL, ZH, ZL)
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

    // Converte os bytes lidos para valores de 16 bits (big-endian)
    *ax = (buffer[0] << 8) | buffer[1];
    *ay = (buffer[2] << 8) | buffer[3];
    *az = (buffer[4] << 8) | buffer[5];
}
