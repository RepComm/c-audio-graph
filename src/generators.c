
#ifndef GENERATORS_C
#define GENERATORS_C

#include "./math.c"

float sine_wave_with_precision (float seconds, float frequency, int precision) {
  return m_sin_with_precision(seconds * frequency, precision);
}

float sine_wave (float seconds, float frequency) {
  return m_sin(seconds * frequency);
}

#endif
