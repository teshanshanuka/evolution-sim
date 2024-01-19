#pragma once

#include <stdint.h>
#include <stdlib.h>

static inline uint8_t rand_bool() { return rand() > RAND_MAX / 2; }

static inline uint8_t rand_uint8() { return rand() % (UINT8_MAX + 1); }

static inline uint16_t rand_uint16() { return rand() % (UINT16_MAX + 1); }

/** Returns a random number between 0 and 1 */
static inline double rand01() { return (double)rand() / RAND_MAX; }

static inline int8_t rand_choice(const int8_t *choices, uint8_t len) {
  return choices[rand() % len];
}
