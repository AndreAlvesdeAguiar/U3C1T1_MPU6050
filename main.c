#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "display.h"
#include "mpu6050.h"

#define SERVO_PIN 2
#define ANGULO_LIMITE_ALERTA 30

// PWM para servo
uint16_t angle_to_duty(float angle) {
    float pulse_ms = 0.5f + (angle / 180.0f) * 2.0f;
    return (uint16_t)((pulse_ms / 20.0f) * 20000.0f);
}

void servo_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 20000);
    pwm_init(slice, &config, true);
}

void servo_set_angle(uint pin, float angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    pwm_set_gpio_level(pin, angle_to_duty(angle));
}

int main() {
    stdio_init_all();
    mpu6050_init();
    display_init();
    servo_init(SERVO_PIN);

    int16_t ax, ay, az;
    char buffer[64];
    float pitch;

    while (true) {
        mpu6050_read_raw(&ax, &ay, &az);

        // cálculo do ângulo de inclinação em graus (eixo Y: pitch)
        pitch = atan2((float)ay, sqrt(ax * ax + az * az)) * 180.0f / M_PI;

        // mapeia pitch para servo (exemplo: -45 a +45 vira 0 a 180 graus)
        float angle = pitch + 90;  // ajusta centro
        if (angle < 0) angle = 0;
        if (angle > 180) angle = 180;

        servo_set_angle(SERVO_PIN, angle);

        // mostra leitura no display
        sprintf(buffer, "Pitch: %.2f graus", pitch);
        display_status_msg(buffer);

        // alerta visual
        if (fabs(pitch) > ANGULO_LIMITE_ALERTA) {
            display_status_msg("!! ALERTA DE INCLINACAO !!");
        }

        sleep_ms(500);
    }

    return 0;
}
