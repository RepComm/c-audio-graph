

#ifndef AUDIONODE_C
#define AUDIONODE_C

#include "../generators.c"
#include "./audioparam.c"
#include "../linkedlist.c"

enum AudioNodeType {
  OscillatorNode,
  DestinationNode
};

char * AudioNode_type_toString (enum AudioNodeType type) {
  if (type == OscillatorNode) return "oscillator";
  if (type == DestinationNode) return "destination";
  return "unknown";
}

struct OscillatorNodeParams {
  struct AudioParam frequency;
};

struct AudioNode {
  /**The type of node*/
  enum AudioNodeType type;
  /**Linked list of AudioNodes*/
  struct lln * inputs;
  /**Pointer to params struct, which is different for each type of node*/
  void * params;
  /**All the nodes feeding their output sample into this one
   * Only applies to nodes that can have input
   */
  struct lln * (*getInputs)(struct AudioNode * thiz);
  void (*connectToNode)(struct AudioNode * thiz, struct AudioNode * to);
  void (*connectToParam)(struct AudioNode * thiz, struct AudioParam * to);
  void (*renderSample)(struct AudioNode * thiz, double seconds, float * out);
  bool (*isSource)(struct AudioNode * thiz);
  bool (*isDestination)(struct AudioNode * thiz);
};

struct lln * AudioNode_getInputs (struct AudioNode * node) {
  return node->inputs;
}

void AudioNode_connectParam (struct AudioNode * src, struct AudioParam * dest) {

}

void AudioNode_connectNode (struct AudioNode * src, struct AudioNode * dest) {
  if (dest->inputs == 0) {
    dest->inputs = lln_create();
    dest->inputs->value = src;
  } else {
    lln_add_value(dest->inputs, src);
  }
}

void AudioNode_renderSample (struct AudioNode * node, double time, float * out) {
  if (node->type == OscillatorNode) {
    *out = sine_wave(
      time,
      //get the frequency value from node params
      ((struct OscillatorNodeParams *) node->params)->frequency.value
    ) / 2;
  } else if (node->type == DestinationNode) {
    struct lln * current = node->inputs;
    
    //If there are no connected nodes, render nothing
    if (current == 0) {
      *out = 0.0;
    } else {
      struct AudioNode * currentNode;

      int count = 0;
      while (current != 0) {
        count++;

        currentNode = current->value;

        currentNode->renderSample(currentNode, time, out);

        current = current->next;
      }
      *out /= count;
    }
  }
}

bool AudioNode_isDestination (struct AudioNode * node) {
  if (node->type == OscillatorNode) return false;

  return true;
}

bool AudioNode_isSource (struct AudioNode * node) {
  if (node->type == DestinationNode) return false;

  return true;
}

struct AudioNode * AudioNode_create (enum AudioNodeType type) {
  struct AudioNode * result = malloc(sizeof(struct AudioNode));

  //TODO - add other node params here
  if (type == OscillatorNode) {
    struct OscillatorNodeParams * params = malloc(sizeof(struct OscillatorNodeParams));
    params->frequency.value = 10.0;

    result->params = params;
  }

  result->type = type;
  result->isDestination = AudioNode_isDestination;
  result->isSource = AudioNode_isSource;
  result->getInputs = AudioNode_getInputs;
  result->connectToNode = AudioNode_connectNode;
  result->connectToParam = AudioNode_connectParam;
  result->renderSample = AudioNode_renderSample;

  return result;
}

#endif
