

#ifndef AUDIOPARAM_C
#define AUDIOPARAM_C

#include "./cat.c"
#include "../linkedlist.c"

#define AudioParamP struct AudioParam *

struct AudioParam {
  float value;
  TrackP track;
  void (*setValueAtTime)(AudioParamP thiz, float time, float value);
  float (*getValueAtTime)(AudioParamP thiz, float time);
};

void AudioParam_setValueAtTime (AudioParamP param, float time, float value) {
  param->track->setValueAtTime(param->track, time, value);
}

float AudioParam_getValueAtTime (AudioParamP param, float time) {
  return param->track->getValueAtTime(param->track, time);
}

AudioParamP AudioParam_create (float value) {
  AudioParamP result = malloc(sizeof(struct AudioParam));
  result->setValueAtTime = &AudioParam_setValueAtTime;
  result->getValueAtTime = &AudioParam_getValueAtTime;
  result->track = Track_create();
  return result;
}

#endif
