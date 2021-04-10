//#include <cstdio>
#include <pico/printf.h>
#include <sstream>
#include <iomanip>
//#include <stdio.h>
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "util/DateHandler.h"
#include "SSD1306.hpp"

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


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        rtc_get_datetime(&t);

        std::ostringstream os{};

        os << "\r"
           << DATETIME_DOWS[t.dotw] << ", "
           << std::setw(2) << std::setfill('0') << std::to_string(t.day) << "."
           << std::setw(2) << std::setfill('0') << std::to_string(t.month) << "."
           << std::to_string(t.year)
           << " "
           << std::setw(2) << std::setfill('0') << std::to_string(t.hour) << ":"
           << std::setw(2) << std::setfill('0') << std::to_string(t.min) << ":"
           << std::setw(2) << std::setfill('0') << std::to_string(t.sec)
           << " ";

        printf("%s", os.str().c_str());
        sleep_ms(1000);
    }
#pragma clang diagnostic pop

    return 0;
}
