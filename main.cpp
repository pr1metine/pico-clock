#include <pico/printf.h>
#include <sstream>
#include <iomanip>
#include "RPi-Pico-SSD1306-library/GFX.hpp"
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

const std::string MODES[]{
        "JAHR",
        "MONAT",
        "TAG",
        "WOCHENTAG",
        "STUNDE",
        "MINUTE",
        "SEKUNDE",
};

static const char *DATETIME_DOWS[7] = {
        "Sonntag",
        "Montag",
        "Dienstag",
        "Mittwoch",
        "Donnerstag",
        "Freitag",
        "Samstag",
};

enum Mode {
    YEAR, MONTH, DAY, DOTW, HOUR, MINUTE, SECOND
};

constexpr int MODE_SEL = 14;
constexpr int UP = 15;
constexpr int DOWN = 16;
Mode curMode = YEAR;
datetime_t t;

void handleModeSelection(uint, uint32_t) {
    curMode = (Mode) (((int) curMode + 1) % 7);
}

void handleIncrement(uint, uint32_t) {
    switch (curMode) {
        case YEAR:
            t.year = t.year < 4095 ? t.year + 1 : 0;
            break;
        case MONTH:
            t.month = t.month < 12 ? t.month + 1 : 1;
            break;
        case DAY:
            t.day = t.day < 31 ? t.day + 1 : 1;
            break;
        case DOTW:
            t.dotw = t.dotw < 6 ? t.dotw + 1 : 0;
            break;
        case HOUR:
            t.hour = t.hour < 23 ? t.hour + 1 : 0;
            break;
        case MINUTE:
            t.min = t.min < 59 ? t.min + 1 : 0;
            break;
        default:
        case SECOND:
            t.sec = t.sec < 59 ? t.sec + 1 : 0;
    }

    rtc_set_datetime(&t);
}

void handleDecrement(uint, uint32_t) {
    switch (curMode) {
        case YEAR:
            t.year = t.year > 0 ? t.year - 1 : 4095;
            break;
        case MONTH:
            t.month = t.month > 1 ? t.month - 1 : 12;
            break;
        case DAY:
            t.day = t.day > 1 ? t.day - 1 : 31;
            break;
        case DOTW:
            t.dotw = t.dotw > 0 ? t.dotw - 1 : 6;
            break;
        case HOUR:
            t.hour = t.hour > 0 ? t.hour - 1 : 23;
            break;
        case MINUTE:
            t.min = t.min > 0 ? t.min - 1 : 59;
            break;
        default:
        case SECOND:
            t.sec = t.sec > 0 ? t.sec - 1 : 59;
    }

    rtc_set_datetime(&t);

}

void handleIRQ(uint gpio, uint32_t event) {
    switch (gpio) {
        default:
        case MODE_SEL:
            handleModeSelection(gpio, event);
            return;
        case UP:
            handleIncrement(gpio, event);
            return;
        case DOWN:
            handleDecrement(gpio, event);
            return;
    }
}

int main() {
    stdio_init_all();

    i2c_init(i2c_default, 400'000);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_init(MODE_SEL);
    gpio_init(UP);
    gpio_init(DOWN);
    gpio_set_irq_enabled(UP, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(DOWN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled_with_callback(MODE_SEL, GPIO_IRQ_EDGE_RISE, true, &handleIRQ);

    GFX gfx{0x3C, 128, 64, i2c_default};

    t = {
            .year = 2020,
            .month = 4,
            .day = 11,
            .dotw = 0,
            .hour = 20,
            .min = 24,
            .sec = 30,
    };

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
        time_str << " " << "(" << MODES[curMode] << ")";

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

