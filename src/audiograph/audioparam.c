

#ifndef AUDIOPARAM_C
#define AUDIOPARAM_C

#include "../linkedlist.c"

struct AudioParamTimeData {
  float time;
  float value;
};

struct AudioParam {
  float value;
  llnp valueAnimationTrack;
};

void AudioParam_setValueAtTime (float time, float value) {
  
}

#endif
