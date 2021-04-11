//
// Created by Quang Thanh Ta on 11.04.21.
//

#include "ModeHandler.h"

gpio_irq_callback_t handleDate(datetime_t t) {
    return [](uint pin, uint32_t event) {
    };
}
