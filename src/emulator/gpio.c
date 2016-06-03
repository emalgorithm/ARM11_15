#include "gpio.h"
#include <stdio.h>

#define GPIO_0 0x20200000
#define GPIO_1 0x20200004
#define GPIO_2 0x20200008

#define GPIO_CLR 0x20200028
#define GPIO_SET 0x2020001c

#define GPIO_0_MSG "One GPIO pin from 0 to 9 has been accessed"
#define GPIO_1_MSG "One GPIO pin from 10 to 19 has been accessed"
#define GPIO_2_MSG "One GPIO pin from 20 to 29 has been accessed"

#define GPIO_CLR_MSG "PIN OFF"
#define GPIO_SET_MSG "PIN ON"

bool is_gpio(uint32_t address) {
    return address == GPIO_0
    || address == GPIO_1
    || address == GPIO_2
    || address == GPIO_CLR
    || address == GPIO_SET;
}

void handle_gpio(uint32_t address) {
    switch(address) {
      case GPIO_0: printf("%s\n", GPIO_0_MSG); return;
      case GPIO_1: printf("%s\n", GPIO_1_MSG); return;
      case GPIO_2: printf("%s\n", GPIO_2_MSG); return;
      case GPIO_CLR: printf("%s\n", GPIO_CLR_MSG); return;
      case GPIO_SET: printf("%s\n", GPIO_SET_MSG); return;
    }
}
