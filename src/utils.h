#ifndef UTILS_H
#define UTILS_H

static inline int max_i(int a, int b) {
    if (a > b) return a;
    return b;
}


static inline int min_i(int a, int b) {
    if (a < b) return a;
    return b;
}

static inline float clamp_f(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static inline float clamp_i(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


#endif