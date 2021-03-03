
#ifndef MATH_C
#define MATH_C

#include <time.h>

const float m_f_pi = 3.14;
const float m_f_tau = 6.28;
const float m_f_piover2 = 1.5707963267948966;

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
  return m_sin_with_precision(radians, 10);
}

float lerp (float from, float to, float by) {
  return from*(1-by)+to*by;
}

float inverseLerp (float from, float to, float value) {
  return (value - from) / (to - from);
}

float m_arcsin (float x) {
  float x2 = x * x;
  float x3 = x2 * x;
  const float a = 1.5707288;
  const float b = -0.2121144;
  const float c = 0.0742610;
  const float d = -0.0187293;
  //TODO - implement sqrt
  return m_f_piover2 - sqrt(1 - x) * (a + b * x + c * x2 + d * x3);
}

/**I don't know why people think this is so complicated
 * Leave it to C programmers to be picky over an if statement for "speed bonuses"
 * 
 * Like, bro. If its not fast enough, its the computer, not the code.
 */
float m_fabs (float f) {
  if (f < 0) return -f;
  return f;
}

#endif