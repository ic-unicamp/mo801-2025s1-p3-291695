#include "litex_time.h"

inline uint32_t start_stopwatch(void) {
    timer0_en_write(0);
    timer0_reload_write(0);
    timer0_load_write(0xffffffff);
    timer0_en_write(1);
    timer0_update_value_write(1);
    return timer0_value_read();
}

inline uint32_t stop_stopwatch(void) {
    timer0_update_value_write(1);
    return timer0_value_read();
}

inline void print_elapsed_time(uint32_t Kticks) {
    uint32_t milliseconds = (uint32_t)(Kticks / (CONFIG_CLOCK_FREQUENCY / 1000000));
    uint32_t seconds = milliseconds / 1000;
    uint32_t minutes = seconds / 60;

    printf(
        "Elapsed time=%02d:%02d (%d milliseconds)\n",
        (int)minutes, (int)seconds, (int)milliseconds
    );
}
