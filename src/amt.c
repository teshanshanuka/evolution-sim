#include "amt.h"
#include "utils.h"
#include "world.h"

Amt create_random_amt(const World *const world) {
  return (Amt){.x = rand_uint16() % world->w, .y = rand_uint16() % world->h};
}

static inline void step(const World *const world, Amt *const amt, int8_t dx, int8_t dy) {
  if (amt->x + dx >= 0 && amt->x + dx < world->w) amt->x += dx;
  if (amt->y + dy >= 0 && amt->y + dy < world->h) amt->y += dy;
}

void step_rand(const World *const world, Amt *const amt) {
  const int8_t dir[] = {-1, 0, 1};
  step(world, amt, rand_choice(dir, 3), rand_choice(dir, 3));
}
