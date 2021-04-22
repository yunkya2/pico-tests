#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

#include "i2s.pio.h"

static inline void i2s_program_init(PIO pio, uint sm, pio_sm_config sm_config,
                                    uint offset, bool autopull, uint hz,
                                    uint data_pin, uint clock_pin_base)
{
    sm_config_set_out_pins(&sm_config, data_pin, 1);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, autopull, 32);

    sm_config_set_set_pins(&sm_config, data_pin, 1);

    float div = (float)clock_get_hz(clk_sys) / hz;
    sm_config_set_clkdiv(&sm_config, div);

    pio_sm_init(pio, sm, offset, &sm_config);

    uint pin_mask = (1u << data_pin) | (3u << clock_pin_base);
    pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
    pio_sm_set_pins(pio, sm, 1); // clear pins

    pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    pio_sm_set_enabled(pio, sm, true);
}

int main()
{
    PIO pio = pio0;
    uint sm = 0;
    uint data_pin = 9;
    uint clock_pin_base = 10;

    uint offset;
    pio_sm_config sm_config;

    uint func = GPIO_FUNC_PIO0;
    gpio_set_function(data_pin, func);
    gpio_set_function(clock_pin_base, func);
    gpio_set_function(clock_pin_base + 1, func);

    /* 16bit stereo */

    offset = pio_add_program(pio, &i2s_16s_program);
    sm_config = i2s_16s_program_get_default_config(offset);
    i2s_program_init(pio, sm, sm_config, offset, true, 44100 * 32 * 2,
                     data_pin, clock_pin_base);

    pio_sm_put_blocking(pio, sm, 0x1234);
    pio_sm_put_blocking(pio, sm, 0x5678);
    pio_sm_put_blocking(pio, sm, 0xff00);
    pio_sm_put_blocking(pio, sm, 0x00ff);
    pio_sm_put_blocking(pio, sm, 0x0000);
    pio_sm_put_blocking(pio, sm, 0x0000);

    pio_remove_program(pio, &i2s_16s_program, offset);

    sleep_ms(1);

    /* 16bit mono */

    offset = pio_add_program(pio, &i2s_16m_program);
    sm_config = i2s_16m_program_get_default_config(offset);
    i2s_program_init(pio, sm, sm_config, offset, false, 44100 * 32 * 4,
                     data_pin, clock_pin_base);

    pio_sm_put_blocking(pio, sm, 0x1234);
    pio_sm_put_blocking(pio, sm, 0x5678);
    pio_sm_put_blocking(pio, sm, 0x0000);

    pio_remove_program(pio, &i2s_16m_program, offset);

    sleep_ms(1);

    /* 8bit stereo */

    offset = pio_add_program(pio, &i2s_8s_program);
    sm_config = i2s_8s_program_get_default_config(offset);
    i2s_program_init(pio, sm, sm_config, offset, true, 44100 * 32 * 2,
                     data_pin, clock_pin_base);

    pio_sm_put_blocking(pio, sm, 0x12);
    pio_sm_put_blocking(pio, sm, 0x34);
    pio_sm_put_blocking(pio, sm, 0xab);
    pio_sm_put_blocking(pio, sm, 0xcd);
    pio_sm_put_blocking(pio, sm, 0x80);
    pio_sm_put_blocking(pio, sm, 0x80);

    pio_remove_program(pio, &i2s_8s_program, offset);

    sleep_ms(1);

    /* 8bit mono */

    offset = pio_add_program(pio, &i2s_8m_program);
    sm_config = i2s_8m_program_get_default_config(offset);
    i2s_program_init(pio, sm, sm_config, offset, false, 44100 * 32 * 4,
                     data_pin, clock_pin_base);

    pio_sm_put_blocking(pio, sm, 0x44);
    pio_sm_put_blocking(pio, sm, 0xbb);
    pio_sm_put_blocking(pio, sm, 0x80);

    pio_remove_program(pio, &i2s_8m_program, offset);

    while (1) ;
}
