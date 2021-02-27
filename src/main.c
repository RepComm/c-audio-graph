
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <math.h>

#include "./audiograph/audiograph.c"

#include "./generators.c"

float randomFloat () {
  return rand() / (float) RAND_MAX;
}

#define SAMPLERATE 44100

void stdout_write_float (char * samplePtr) {
  printf("%c", samplePtr[0]);
  printf("%c", samplePtr[1]);
  printf("%c", samplePtr[2]);
  printf("%c", samplePtr[3]);
}

int main(int argc, char **argv) {
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
  int max = 1024;

  while (doProcess) {
    ((struct OscillatorNodeParams *)osc->params)->frequency.value += 0.1;

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
      break;
    }
  }

  return 0;
}
