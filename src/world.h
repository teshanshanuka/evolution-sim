#pragma once

#include <stdint.h>

typedef struct World {
    uint16_t w;
    uint16_t h;
    uint16_t** grid;
} World;

World create_world(uint16_t w, uint16_t h);