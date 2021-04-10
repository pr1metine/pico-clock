//
// Created by Quang Thanh Ta on 08.04.21.
//

#include "pico/util/datetime.h"
#include "pico/stdlib.h" // NOLINT(modernize-deprecated-headers)
#include <pico/printf.h>
#include "DateHandler.h"


datetime_t getInitialDate() {
    printf("Please enter the following information...\n");
    int year = getIndividualDateInfo("Year", isValidYear);
    int month = getIndividualDateInfo("Month", isValidMonth);
    int day = getIndividualDateInfo("Day", isValidDay);
    int dotw = getIndividualDateInfo("Day Of The Week", isValidDOTW);
    int hour = getIndividualDateInfo("Hour", isValidHour);
    int min = getIndividualDateInfo("Minute", isValidMinute);
    int sec = getIndividualDateInfo("Second", isValidSecond);

    datetime_t t{
            .year = static_cast<int16_t>(year),
            .month = static_cast<int8_t>(month),
            .day = static_cast<int8_t>(day),
            .dotw = static_cast<int8_t>(dotw),
            .hour = static_cast<int8_t>(hour),
            .min = static_cast<int8_t>(min),
            .sec = static_cast<int8_t>(sec)
    };

    return t;
}

int getIndividualDateInfo(const char *fieldName, bool(*pred)(int)) {
    int res = -1;
    while (!pred(res)) {
        printf("\n%s: ", fieldName);
        //TODO: Handle conversion errors
        scanf("%i", &res);
        sleep_ms(100);
    }

    printf("%i\n", res);
    return res;
}

bool isValidYear(int year) { return year >= 0 && year <= 4095; }

bool isValidMonth(int month) { return month >= 1 && month <= 12; }

bool isValidDay(int day) { return day >= 1 && day <= 31; }

bool isValidDOTW(int dotw) { return dotw >= 0 && dotw <= 6; }

bool isValidHour(int hour) { return hour >= 0 && hour <= 23; }

bool isValidMinute(int min) { return min >= 0 && min <= 59; }

bool isValidSecond(int sec) { return sec >= 0 & sec <= 59; }
