
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <math.h>

#include "./audiograph/audiograph.c"

#include "./generators.c"

#include "./audiograph/cat.c"

#include "./utils.c"

#define SAMPLERATE 44100

char stdout_write_hexfloat_data[16] = "00000000";
void stdout_write_hexfloat (char * samplePtr) {
  _32bit_to_hex(samplePtr, stdout_write_hexfloat_data, 0);
  printf(
    "%c%c%c%c%c%c%c%c",
    stdout_write_hexfloat_data[0],
    stdout_write_hexfloat_data[1],
    stdout_write_hexfloat_data[2],
    stdout_write_hexfloat_data[3],
    stdout_write_hexfloat_data[4],
    stdout_write_hexfloat_data[5],
    stdout_write_hexfloat_data[6],
    stdout_write_hexfloat_data[7]
  );
}

void graphtest_0 () {
  //Create an audio context graph
  struct AudioGraph * ctx = AudioGraph_create(SAMPLERATE);
  
  //Create a buffer to render the graph to
  int outputBufferSize = 1024;
  float outputBuffer[outputBufferSize];

  //Create an oscillator
  struct AudioNode * osc = ctx->createNode(ctx, OscillatorNode);

  //connect it to the graph output aka "destination"
  osc->connectToNode(osc, ctx->destination);

  bool doProcess = true;
  int counter = 0;
  int max = 2048;

  struct OscillatorNodeParams * oscParams = osc->params;

  oscParams->frequency->value = 440/2;
  
  oscParams->frequency->setValueAtTime(
    oscParams->frequency,
    0.0,
    440
  );

  oscParams->frequency->setValueAtTime(
    oscParams->frequency,
    20.0,
    440/16
  );

  while (doProcess) {
    //Render
    ctx->renderSamples(ctx, outputBuffer, outputBufferSize);

    for (int i=0; i<outputBufferSize; i++) {
      stdout_write_hexfloat( (void *) &outputBuffer[i]);
    }
    counter ++;
    if (counter > max) {
      doProcess = false;
      // exit(0);
      break;
    }
  }
}

void tracktest_0 () {
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
}

int main(int argc, char **argv) {

  graphtest_0();

  return 0;
}
