#include <pico/printf.h>
#include "RPi-Pico-SSD1306-library/GFX.hpp"
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

const char *MODES[]{
        "JAHR",
        "MONAT",
        "TAG",
        "WOCHENTAG",
        "STUNDE",
        "MINUTE",
        "SEKUNDE",
};

const char *DATETIME_DOTW[7] = {
        "Sonntag",
        "Montag",
        "Dienstag",
        "Mittwoch",
        "Donnerstag",
        "Freitag",
        "Samstag",
};

const int8_t DAYS_PER_MONTH[12]{
        31, // Januar
        29, // Februar
        31, // März
        30, // April
        31, // Mai
        30, // Juni
        31, // Juli
        31, // August
        30, // September
        31, // Oktober
        30, // November
        31, // Dezember
};

enum Mode {
    YEAR, MONTH, DAY, DOTW, HOUR, MINUTE, SECOND
};

constexpr int MODE_SEL = 27;
constexpr int UP = 8;
constexpr int DOWN = 7;
constexpr int DEBOUNCE_DELAY = 200;
Mode curMode = YEAR;
datetime_t t{
        .year = 2021,
        .month = 4,
        .day = 20,
        .dotw = 0,
        .hour = 9,
        .min = 34,
        .sec = 30,
};
ulong lastInput = to_ms_since_boot(get_absolute_time());

int8_t getDaysPerMonth(int month) {
    if (month != 2) {
        return DAYS_PER_MONTH[month - 1];
    }

    bool isLeapYear = (t.year % 4 == 0) && ((t.year % 100 != 0) || (t.year % 400 == 0));

    return isLeapYear ? 29 : 28;
}

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
            t.day = t.day < getDaysPerMonth(t.month) ? t.day + 1 : 1;
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
            t.day = t.day > 1 ? t.day - 1 : getDaysPerMonth(t.month);
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
    ulong curInput = to_ms_since_boot(get_absolute_time());
    if (curInput - lastInput < DEBOUNCE_DELAY) {
        return;
    }
    lastInput = curInput;

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

    i2c_init(i2c1, 400'000);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);
    gpio_pull_up(18);
    gpio_pull_up(19);
    gpio_init(MODE_SEL);
    gpio_init(UP);
    gpio_init(DOWN);
    gpio_pull_up(MODE_SEL);
    gpio_pull_up(UP);
    gpio_pull_up(DOWN);
    gpio_set_irq_enabled(UP, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(DOWN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled_with_callback(MODE_SEL, GPIO_IRQ_EDGE_FALL, true, &handleIRQ);

    GFX gfx{0x3C, D128x32, i2c1};

    rtc_init();
    rtc_set_datetime(&t);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        gfx.clear();
        rtc_get_datetime(&t);

        char date_str[40], time_str[40];
        sprintf(date_str, "%s, %02i.%02i.%i", DATETIME_DOTW[t.dotw], t.day, t.month, t.year);
        sprintf(time_str, "%02i:%02i:%02i (%s)", t.hour, t.min, t.sec, MODES[curMode]);
        gfx.drawString(0, 0, date_str);
        gfx.drawString(0, 15, time_str);
        gfx.display();

        sleep_ms(100);
    }
#pragma clang diagnostic pop

    return 0;
}
