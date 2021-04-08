//#include <cstdio>
#include <pico/printf.h>
//#include <stdio.h>
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "util/DateHandler.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

static const char *DATETIME_DOWS[7] = {
        "Sonntag",
        "Montag",
        "Dienstag",
        "Mittwoch",
        "Donnerstag",
        "Freitag",
        "Samstag",
};

int main() {
    stdio_init_all();
    printf("Ready when you are!\n");

    datetime_t t = getInitialDate();

    rtc_init();
    rtc_set_datetime(&t);

    while (true) {
        rtc_get_datetime(&t);
        printf("\r%s, %02d.%02d.%d %d:%02d:%02d      ",
               DATETIME_DOWS[t.dotw],
               t.day,
               t.month,
               t.year,
               t.hour,
               t.min,
               t.sec
        );
        sleep_ms(1000);
    }

    return 0;
}


#pragma clang diagnostic pop