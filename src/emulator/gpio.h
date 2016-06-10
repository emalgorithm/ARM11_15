#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

bool is_gpio(uint32_t address);
void handle_gpio(uint32_t address);

#endif
