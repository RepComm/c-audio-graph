
#ifndef MATH_C
#define MATH_C

#include <time.h>

const float m_f_pi = 3.14;
const float m_f_tau = 6.28;

float m_sin_with_precision (float radians, int steps) {
  //keep radians between -pi and pi
  radians = fmod(radians, m_f_tau)-m_f_pi;

  //https://stackoverflow.com/a/2284929
  float res=0, pow=radians, fact=1;

  for(int i=0; i<steps; ++i) {
    res+=pow/fact;
    pow*=-1*radians*radians;
    fact*=(2*(i+1))*(2*(i+1)+1);
  }
  return res;
}

float m_get_seconds () {
  double res = 0;

  res = ((double) clock()) / CLOCKS_PER_SEC;

  return res;
}

float m_sin (float radians) {
  return m_sin_with_precision(radians, 5);
}

float lerp (float from, float to, float by) {
  return from*(1-by)+to*by;
}

#endif