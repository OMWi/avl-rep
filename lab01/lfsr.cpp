#include <iostream>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    uint16_t n = (((*reg & 1) ^ ((*reg >> 2) & 1)) ^ ((*reg >> 3) & 1)) ^ ((*reg >> 5) & 1);
    *reg >>= 1;
    n <<= 15;
    *reg |= n;
}
