#include <pico/printf.h>
#include <sstream>
#include <iomanip>
#include "RPi-Pico-SSD1306-library/GFX.hpp"
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "util/DateHandler.h"

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

    i2c_init(i2c_default, 400'000);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    GFX gfx{0x3C, 128, 64, i2c_default};
    gfx.drawString(0, 10, "Waiting for input");
    gfx.display();

    // Wait for input before asking for date and time
    scanf("%s");
    printf("\nReady when you are!\n");

    datetime_t t = getInitialDate();
    rtc_init();
    rtc_set_datetime(&t);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        gfx.clear();
        rtc_get_datetime(&t);

        std::ostringstream date_str{}, time_str{};

        date_str
                << DATETIME_DOWS[t.dotw] << ", "
                << std::setw(2) << std::setfill('0') << std::to_string(t.day) << "."
                << std::setw(2) << std::setfill('0') << std::to_string(t.month) << "."
                << std::to_string(t.year);
        time_str
                << std::setw(2) << std::setfill('0') << std::to_string(t.hour) << ":"
                << std::setw(2) << std::setfill('0') << std::to_string(t.min) << ":"
                << std::setw(2) << std::setfill('0') << std::to_string(t.sec);

        printf("\r %s %s", date_str.str().c_str(), time_str.str().c_str());

        gfx.drawString(0, 0, date_str.str());
        gfx.display();
        gfx.drawString(0, 20, time_str.str());
        gfx.display();
        sleep_ms(1000);
    }
#pragma clang diagnostic pop

    return 0;
}
