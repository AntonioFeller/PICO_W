// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ---------------- //
// spi_gap0_sample1 //
// ---------------- //

#define spi_gap0_sample1_wrap_target 0
#define spi_gap0_sample1_wrap 4

#define spi_gap0_sample1_offset_lp1_end 2u
#define spi_gap0_sample1_offset_end 5u

static const uint16_t spi_gap0_sample1_program_instructions[] = {
            //     .wrap_target
    0x6001, //  0: out    pins, 1         side 0     
    0x1040, //  1: jmp    x--, 0          side 1     
    0xe080, //  2: set    pindirs, 0      side 0     
    0x5001, //  3: in     pins, 1         side 1     
    0x0083, //  4: jmp    y--, 3          side 0     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program spi_gap0_sample1_program = {
    .instructions = spi_gap0_sample1_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config spi_gap0_sample1_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + spi_gap0_sample1_wrap_target, offset + spi_gap0_sample1_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}
#endif

// ----------------- //
// spi_gap01_sample0 //
// ----------------- //

#define spi_gap01_sample0_wrap_target 0
#define spi_gap01_sample0_wrap 5

#define spi_gap01_sample0_offset_lp1_end 2u
#define spi_gap01_sample0_offset_end 6u

static const uint16_t spi_gap01_sample0_program_instructions[] = {
            //     .wrap_target
    0x6001, //  0: out    pins, 1         side 0     
    0x1040, //  1: jmp    x--, 0          side 1     
    0xe080, //  2: set    pindirs, 0      side 0     
    0xb042, //  3: nop                    side 1     
    0x4001, //  4: in     pins, 1         side 0     
    0x1084, //  5: jmp    y--, 4          side 1     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program spi_gap01_sample0_program = {
    .instructions = spi_gap01_sample0_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config spi_gap01_sample0_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + spi_gap01_sample0_wrap_target, offset + spi_gap01_sample0_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}
#endif

// ------------------ //
// spi_gap010_sample1 //
// ------------------ //

#define spi_gap010_sample1_wrap_target 0
#define spi_gap010_sample1_wrap 6

#define spi_gap010_sample1_offset_lp1_end 2u
#define spi_gap010_sample1_offset_end 7u

static const uint16_t spi_gap010_sample1_program_instructions[] = {
            //     .wrap_target
    0x6001, //  0: out    pins, 1         side 0     
    0x1040, //  1: jmp    x--, 0          side 1     
    0xe080, //  2: set    pindirs, 0      side 0     
    0xb042, //  3: nop                    side 1     
    0xa042, //  4: nop                    side 0     
    0x5001, //  5: in     pins, 1         side 1     
    0x0085, //  6: jmp    y--, 5          side 0     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program spi_gap010_sample1_program = {
    .instructions = spi_gap010_sample1_program_instructions,
    .length = 7,
    .origin = -1,
};

static inline pio_sm_config spi_gap010_sample1_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + spi_gap010_sample1_wrap_target, offset + spi_gap010_sample1_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}
#endif

// ------------------------ //
// spi_gap0_sample1_regular //
// ------------------------ //

#define spi_gap0_sample1_regular_wrap_target 0
#define spi_gap0_sample1_regular_wrap 4

#define spi_gap0_sample1_regular_offset_lp1_end 2u
#define spi_gap0_sample1_regular_offset_end 5u

static const uint16_t spi_gap0_sample1_regular_program_instructions[] = {
            //     .wrap_target
    0x6001, //  0: out    pins, 1         side 0     
    0x1040, //  1: jmp    x--, 0          side 1     
    0xe080, //  2: set    pindirs, 0      side 0     
    0x5001, //  3: in     pins, 1         side 1     
    0x0083, //  4: jmp    y--, 3          side 0     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program spi_gap0_sample1_regular_program = {
    .instructions = spi_gap0_sample1_regular_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config spi_gap0_sample1_regular_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + spi_gap0_sample1_regular_wrap_target, offset + spi_gap0_sample1_regular_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}
#endif

