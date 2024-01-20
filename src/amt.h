#pragma once

#include <stdint.h>

#include "utils.h"
#include "world.h"

typedef struct Amt { // not exactly an ant
  uint16_t x;
  uint16_t y;
} Amt;

Amt create_random_amt(const World *const world);

void step_rand(const World *const world, Amt *const amt);