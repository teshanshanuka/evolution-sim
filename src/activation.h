#include <math.h>

// tanh already in math.h

static inline double identity(double x) { return x; }

static inline double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
