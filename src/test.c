#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "activation.h"
#include "nn.h"

const char *to_binary(char *buf, uint16_t n, uint8_t len) {
  for (int i = len - 1; i >= 0; i--)
    buf[len - 1 - i] = (n >> i & 0b1) ? '1' : '0';
  buf[len] = '\0';
  return buf;
}

void test_create_genome() {
  //   srand(time(NULL));

  uint16_t len = 10;
  Gene genome[len];
  create_genome(genome, len);
  char buf1[65], buf2[9];

  for (int i = 0; i < len; i++) {
    Gene c = genome[i];

    printf("ip %u, op %u, w %u\n", c.ip, c.op, c.w);
    printf("ip %s, op %s\n", to_binary(buf1, c.ip, 8),
           to_binary(buf2, c.op, 8));
    // printf("frm raw ip %u, op %u,w %u\n", c.raw & 0xff, (c.raw >> 8) & 0xff,
    // c.raw >> 16);
    printf("ip typ: %d, op typ: %d\n", c.ip >> 7, c.op >> 7);
    printf("\n");
  }
}

void test_mutation() {
  srand(time(NULL));

  uint16_t len = 5;
  Gene genome[len];
  create_genome(genome, len);
  Gene new_genome[len];

  mutate_genome_1bit(genome, len, new_genome, 3);

  for (int i = 0; i < len; i++) {
    Gene c = genome[i], nc = new_genome[i];
    printf("%u ip %u, op %u, w %u\n", i, c.ip, c.op, c.w);
    printf("%u ip %u, op %u, w %u\n", i, nc.ip, nc.op, nc.w);
    printf("\n");
  }
}

void test_calculate() {
  Gene genome[] = {
      {.ip = 0, .op = 1, .w = UINT16_MAX / 4 * 3},            // ip 0 -> op 1
      {.ip = 3, .op = 2, .w = UINT16_MAX / 4},                // ip 3 -> op 2
      {.ip = 2, .op = 1 | (1 << 7), .w = UINT16_MAX / 8 * 3}, // ip 2 -> hn 1
      {.ip = 1 | (1 << 7), .op = 0, .w = UINT16_MAX / 8 * 7}, // hn 1 -> op 0
  };
  uint8_t len = sizeof(genome) / sizeof(Gene);

  double ips[] = {1.0, 0.0, 2.0, 0.0, 3.0};
  double ops[] = {0.0, 0.0, 0.0};

  NNCfg nn_cfg = {
      .act_fn = tanh,
      .n_ip = sizeof(ips) / sizeof(double),
      .n_hn = 2,
      .n_op = sizeof(ops) / sizeof(double),
      .w_max = 5.0,
  };

  printf("inputs: %f, %f, %f, %f, %f\n", ips[0], ips[1], ips[2], ips[3],
         ips[4]);
  calculate(&nn_cfg, genome, len, ips, ops);
  printf("outputs: %f, %f, %f\n", ops[0], ops[1], ops[2]);
}

void test_nn_gen() {
  uint8_t n_genomes = 15;
  uint16_t len = 5;
  Gene genome[len];

  NNCfg nn_cfg = {
      .act_fn = tanh,
      .n_ip = 6,
      .n_hn = 2,
      .n_op = 3,
      .w_max = 5.0,
  };

  char file_name[] = "out/gXXX.nn";
  for (int i = 0; i < n_genomes; i++) {
    create_genome(genome, len);
    sprintf(file_name, "out/g%d.nn", i);
    dump_nn(&nn_cfg, genome, len, file_name);

    mutate_genome_1bit(genome, len, genome, 1);
    sprintf(file_name, "out/g%dm.nn", i);
    dump_nn(&nn_cfg, genome, len, file_name);
  }
}

int main() {
  // test_create_genome();
  // test_mutation();
  // test_w_norm();
  // test_calculate();
  test_nn_gen();
  return 0;
}