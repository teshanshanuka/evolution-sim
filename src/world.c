#include <stdint.h>
#include <stdlib.h>
#include "world.h"

World create_world(uint16_t w, uint16_t h) {
  World world = {.w = w, .h = h};
  world.grid = malloc(h * sizeof(uint16_t *));
  for (uint16_t y = 0; y < h; y++)
    world.grid[y] = calloc(w, sizeof(uint16_t));
  return world;
}
