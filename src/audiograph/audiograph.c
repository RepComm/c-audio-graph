
#ifndef AUDIOGRAPH_C
#define AUDIOGRAPH_C

#include "../linkedlist.c"
#include "./audionode.c"

struct AudioGraph {
  struct lln * nodes;
  int sampleRate;
  struct AudioNode * destination;
  double seconds;

  struct AudioNode * (*createNode)(struct AudioGraph * thiz, enum AudioNodeType type);
  void (*renderSamples)(struct AudioGraph * thiz, float out[], int outSize);
};

bool AudioGraph_getNodeCount (struct AudioGraph * thiz) {
  return lln_get_size(thiz->nodes);
}

/**Create a node from the graph object
 * 
 */
struct AudioNode * AudioGraph_create_node (struct AudioGraph * thiz, enum AudioNodeType type) {
  if (thiz == 0) return 0;
  struct AudioNode * result = AudioNode_create(type);
  
  if (AudioGraph_getNodeCount(thiz) == 0) {
    //create first link in the chain
    struct lln* link = lln_create();
    link->value = result;

    thiz->nodes = link;
  } else {
    lln_add_value(thiz->nodes, result);
  }
  return result;
}

void AudioGraph_renderSample (struct AudioGraph * thiz, double seconds, float * out) {
  thiz->destination->renderSample(thiz->destination, seconds, out);
}

void AudioGraph_renderSamples (struct AudioGraph * thiz, float out[], int outSize) {
  //TODO calculate seconds
  for (int i=0; i<outSize; i++) {
    thiz->seconds += 0.001;
    // out[i] = 0.0;
    AudioGraph_renderSample(thiz, thiz->seconds, &out[i]);
  }
}

/**Create an audio graph context
 * If sampleRate is 0, will return null, no context is created
 */
struct AudioGraph * AudioGraph_create (int sampleRate) {
  if (sampleRate == 0) return 0;
  struct AudioGraph * result = malloc(sizeof(struct AudioGraph));
  result->nodes = 0;
  result->sampleRate = sampleRate;
  result->createNode = AudioGraph_create_node;
  result->renderSamples = AudioGraph_renderSamples;
  result->destination = result->createNode(result, DestinationNode);

  return result;
}

#endif
