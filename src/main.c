
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <math.h>

#include "./audiograph/audiograph.c"

#include "./generators.c"

#include "./audiograph/cat.c"

#define SAMPLERATE 44100

void stdout_write_float (char * samplePtr) {
  printf("%c%c%c%c", samplePtr[0], samplePtr[1], samplePtr[2], samplePtr[3]);
}

void graphtest_0 () {
  bool debug = false;

  //Create an audio context graph
  struct AudioGraph * ctx = AudioGraph_create(SAMPLERATE);
  
  if (debug) printf("Graph at %p\n", ctx);

  //Create a buffer to render the graph to
  int outputBufferSize = 1024;
  float outputBuffer[outputBufferSize];

  //Create an oscillator
  struct AudioNode * osc = ctx->createNode(ctx, OscillatorNode);
  if (debug) printf("Osc %p\n", osc);

  //connect it to the graph output aka "destination"
  osc->connectToNode(osc, ctx->destination);
  if (debug) printf("Connected osc to destination\n");

  // //Render
  // ctx->renderSamples(ctx, outputBuffer, outputBufferSize);
  // if (debug) printf("Rendered %i samples to buffer\n", outputBufferSize);

  bool doProcess = true;
  int counter = 0;
  int max = 2048;

  ((struct OscillatorNodeParams *)osc->params)->frequency.value = 440/2;

  while (doProcess) {
    ((struct OscillatorNodeParams *)osc->params)->frequency.value += 1;

    //Render
    ctx->renderSamples(ctx, outputBuffer, outputBufferSize);
    if (debug) printf("Rendered %i samples to buffer\n", outputBufferSize);

    for (int i=0; i<outputBufferSize; i++) {
      if (!debug) stdout_write_float( (void *) &outputBuffer[i]);
      // if (debug) printf("%.4f\n", outputBuffer[i]);
    }
    counter ++;
    if (counter > max) {
      doProcess = false;
      // exit(0);
      break;
    }
  }
}

int main(int argc, char **argv) {

  // graphtest_0();

  TrackP track = Track_create();
  
  printf("Created track %p\n", track);

  track->addKeyFrame(track, 0.0, 0.0, false);
  track->addKeyFrame(track, 10.0, 2.0, false);
  track->addKeyFrame(track, 20.0, 1.0, true);

  printf("Track duration %f\n", track->duration);

  for (float i=0.0; i < track->duration; i+=0.1) {
    float value = Track_getValueAtTime(track, i);
    // float value = i;
    printf("Value at time %g is %g\n", i, value);
  }

  return 0;
}
