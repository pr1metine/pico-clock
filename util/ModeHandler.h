//
// Created by Quang Thanh Ta on 11.04.21.
//

#ifndef PICO_CLOCK_MODEHANDLER_H
#define PICO_CLOCK_MODEHANDLER_H

#include <hardware/gpio.h>

gpio_irq_callback_t handleDate(datetime_t t);

#endif //PICO_CLOCK_MODEHANDLER_H
