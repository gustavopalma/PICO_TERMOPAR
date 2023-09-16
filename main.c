#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define TOTAL_LEITURAS 10

int main() {

    stdio_init_all();

    spi_init(spi_default, 5000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, true);

    uint8_t dados[4];
    
    bool circuito_aberto = false;
    bool curto_vcc = false;
    bool curto_gnd = false;

    float media_temp = 0;
    int contador_media = 0;

    while(true) {
        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, false);
        sleep_us(10);
        spi_read_blocking(spi_default, 0, &dados, 4);
        sleep_us(10);
        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, true);

        if ( dados[1] & 0b00000001 ) {
            printf("Falha no Sensor!\n");

            if ( dados[3] & 0b00000001 ) {
                printf("Circuito Aberto, Termopar desconectado!\n");
            }

            if (dados[3] & 0b00000010) {
                printf("Termopar em Curto com o GND\n");
            }

            if (dados[3] & 0b00000100) {
                printf("Termopar em Curto com a Alimentação\n");
            }
            sleep_ms(100);
            continue;
        }

        float parte_decimal = ((dados[1] & 0b00001100) >> 2) * 0.25;
        int parte_inteira = dados[1] >> 4;
        parte_inteira = parte_inteira + (dados[0] << 4);

        float temperatura_sensor = parte_inteira + parte_decimal;
        if (contador_media < TOTAL_LEITURAS) {
            media_temp += temperatura_sensor;
            contador_media++;
        } else {
            media_temp = media_temp / TOTAL_LEITURAS;
            printf("Temperatura do Sensor: %.2f\n", media_temp);
            contador_media = 0;
            media_temp = 0;
        } 
        sleep_ms(100);
    }
    return 0;
}