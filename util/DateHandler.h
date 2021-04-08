//
// Created by Quang Thanh Ta on 08.04.21.
//

#ifndef PICO_CLOCK_DATEHANDLER_H
#define PICO_CLOCK_DATEHANDLER_H

datetime_t getInitialDate();

int getIndividualDateInfo(const char *fieldName, bool(*pred)(int));

bool isValidYear(int year);
bool isValidMonth(int month);
bool isValidDay(int day);
bool isValidDOTW(int dotw);
bool isValidHour(int hour);
bool isValidMinute(int min);
bool isValidSecond(int sec);


#endif //PICO_CLOCK_DATEHANDLER_H
