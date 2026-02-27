#pragma once

#include <stdint.h>

extern volatile uint64_t pitInteruptsTriggered;

void PIT_sleep_ms(unsigned int ms);

void PIT_init(uint32_t hz);
