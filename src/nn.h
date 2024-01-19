#pragma once

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "utils.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef union Gene {
  struct {
    uint8_t ip; // <input type> | <node id>[7]
    uint8_t op; // <output type> | <node id>[7]
    uint16_t w; // weight
  };
  uint32_t raw;
} Gene;

typedef struct Cfg {
  double (*act_fn)(double); // activation function
  uint8_t n_ip;             // number of input nodes
  uint8_t n_hn;             // number of hidden nodes
  uint8_t n_op;             // number of output nodes
  double w_max; // weight is normalized to be between -w_max and +w_max
} NNCfg;

/** Normalize `uint16 weight` to be between -w_max and +w_max */
static inline double w_norm(uint16_t w, double w_max) {
  return (double)w / UINT16_MAX * w_max * 2 - w_max;
}

void create_genome(Gene *genome, uint16_t size);

/** Mutate one random bit in n random genes */
void mutate_genome_1bit(Gene *genome, uint16_t len, Gene *new_genome, uint8_t n);

void calculate(NNCfg *nn_cfg, Gene *genome, uint16_t len, double *ip,
               double *op);

void dump_nn(NNCfg *nn_cfg, Gene *genome, uint16_t len, char *file);
