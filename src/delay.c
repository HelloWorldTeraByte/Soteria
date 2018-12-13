#include <stdint.h>
#include "delay.h"

void wait_unprecise(int moment)
{
    volatile uint32_t i, j;
    for(i = 0; i < moment; i++) {
        j++;
    }
}
