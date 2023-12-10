#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nn.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void create_genome(Gene *genome, uint16_t size) {
  for (uint16_t i = 0; i < size; i++)
    genome[i] = (Gene){
        .ip = rand_uint8(),
        .op = rand_uint8(),
        .w = rand_uint16(),
    };
}

void mutate_genome_1bit(Gene *genome, uint16_t len, Gene *new_genome,
                        uint8_t n) {
  for (uint16_t i = 0; i < len; i++)
    new_genome[i] = genome[i];

  for (uint8_t i = 0; i < n; i++) {
    uint8_t mod_idx = MIN(rand01() * len, len - 1);
    uint8_t bit = MIN(rand01() * 32, 31);
    
    new_genome[mod_idx].raw ^= (1 << bit);
    DEBUG_PRINT("gene %u bit %u: old %u, new %u\n", mod_idx, bit,
                genome[mod_idx].raw, new_genome[mod_idx].raw);
  }
}

void calculate(NNCfg *nn_cfg, Gene *genome, uint16_t len, double *ip,
               double *op) {
  double hns[nn_cfg->n_hn];

  // TODO: optimize by not calculate unconnected hidden nodes

  // ip -> op/hn
  for (uint16_t i = 0; i < len; i++) {
    const Gene c = genome[i];
    if (c.ip >> 7)
      continue; // ip type is hn. TODO: hn->hn

    const double w = w_norm(c.w, nn_cfg->w_max);
    const uint16_t ip_idx = (c.ip & 0x7f) % nn_cfg->n_ip;

    if (c.op >> 7) { // op type is hn
      const uint16_t hn_idx = (c.op & 0x7f) % nn_cfg->n_hn;
      hns[hn_idx] += nn_cfg->act_fn(ip[ip_idx] * w);
      DEBUG_PRINT("ip %u -> hn %u w: %f, %f -> %f\n", ip_idx, hn_idx, w,
                  ip[ip_idx], hns[hn_idx]);
    } else { // op type is op
      const uint16_t op_idx = (c.op & 0x7f) % nn_cfg->n_op;
      op[op_idx] += nn_cfg->act_fn(ip[ip_idx] * w);
      DEBUG_PRINT("ip %u -> op %u w: %f, %f -> %f\n", ip_idx, op_idx, w,
                  ip[ip_idx], op[op_idx]);
    }
  }

  // hn -> op
  for (uint16_t i = 0; i < len; i++) {
    const Gene c = genome[i];
    if (!(c.ip >> 7)) // already calculated
      continue;

    if (c.op >> 7) // TODO: hn -> hn
      continue;

    const double w = w_norm(c.w, nn_cfg->w_max);
    const uint16_t hn_idx = (c.ip & 0x7f) % nn_cfg->n_hn;
    const uint16_t op_idx = (c.op & 0x7f) % nn_cfg->n_op;
    op[op_idx] += nn_cfg->act_fn(hns[hn_idx] * w);
    DEBUG_PRINT("hn %u -> op %u w: %f, %f -> %f\n", hn_idx, op_idx, w,
                hns[hn_idx], op[op_idx]);
  }
}

void dump_nn(NNCfg *nn_cfg, Gene *genome, uint16_t len, char *file) {
  FILE *fptr = fopen(file, "w");

  // can simplify but keeping the same format as the calculate function
  for (uint16_t i = 0; i < len; i++) {
    const Gene c = genome[i];
    if (c.ip >> 7)
      continue; // ip type is hn. TODO: hn->hn

    const uint16_t ip_idx = (c.ip & 0x7f) % nn_cfg->n_ip;

    if (c.op >> 7) { // op type is hn
      const uint16_t hn_idx = (c.op & 0x7f) % nn_cfg->n_hn;
      fprintf(fptr, "I%u H%u %u\n", ip_idx, hn_idx, c.w);
    } else { // op type is op
      const uint16_t op_idx = (c.op & 0x7f) % nn_cfg->n_op;
      fprintf(fptr, "I%u O%u %u\n", ip_idx, op_idx, c.w);
    }
  }

  for (uint16_t i = 0; i < len; i++) {
    const Gene c = genome[i];
    if (!(c.ip >> 7)) // already calculated
      continue;

    if (c.op >> 7) // TODO: hn -> hn
      continue;

    const uint16_t hn_idx = (c.ip & 0x7f) % nn_cfg->n_hn;
    const uint16_t op_idx = (c.op & 0x7f) % nn_cfg->n_op;
    fprintf(fptr, "H%u O%u %u\n", hn_idx, op_idx, c.w);
  }

  fclose(fptr);
}
