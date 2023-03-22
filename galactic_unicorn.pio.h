// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ---------------- //
// galactic_unicorn //
// ---------------- //

#define galactic_unicorn_wrap_target 0
#define galactic_unicorn_wrap 23

static const uint16_t galactic_unicorn_program_instructions[] = {
            //     .wrap_target
    0x6048, //  0: out    y, 8                       
    0x6008, //  1: out    pins, 8                    
    0x7121, //  2: out    x, 1            side 0 [1] 
    0xe004, //  3: set    pins, 4                    
    0x0026, //  4: jmp    !x, 6                      
    0xe005, //  5: set    pins, 5                    
    0xba42, //  6: nop                    side 1 [2] 
    0x7121, //  7: out    x, 1            side 0 [1] 
    0xe004, //  8: set    pins, 4                    
    0x002b, //  9: jmp    !x, 11                     
    0xe005, // 10: set    pins, 5                    
    0xba42, // 11: nop                    side 1 [2] 
    0x7121, // 12: out    x, 1            side 0 [1] 
    0xe004, // 13: set    pins, 4                    
    0x0030, // 14: jmp    !x, 16                     
    0xe005, // 15: set    pins, 5                    
    0x7a65, // 16: out    null, 5         side 1 [2] 
    0x0082, // 17: jmp    y--, 2                     
    0x6068, // 18: out    null, 8                    
    0xe506, // 19: set    pins, 6                [5] 
    0xe000, // 20: set    pins, 0                    
    0x6040, // 21: out    y, 32                      
    0x0096, // 22: jmp    y--, 22                    
    0xe004, // 23: set    pins, 4                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program galactic_unicorn_program = {
    .instructions = galactic_unicorn_program_instructions,
    .length = 24,
    .origin = -1,
};

static inline pio_sm_config galactic_unicorn_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + galactic_unicorn_wrap_target, offset + galactic_unicorn_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}
#endif
