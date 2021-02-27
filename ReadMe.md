# c-audio-graph

An audio graph library written in C

## Why
In js/ts I'm used to having the WebAudioAPI audio graph</br>

I didn't feel like using some bloated OS dep. libs, so here we are.

## State
Currently the code can render a (slightly buggy) scene graph where an
Oscillator node connects to the destination node

## How it works
On the top level, this is a node graph where nodes connect to other nodes, and/or node params (like frequency, gain, etc)

Intermediately, it is a recursive reverse lookup tree structure of nodes
The first node to evaluate is the context destination, which triggers
all other nodes connected to it

All nodes are stored in a linked list in the graph context,
which can be modified during runtime w/ dynamic heap memory allocation.

I *might* be a huge fan of linked lists

Nodes can be source, destination, or both.

Oscillators are sources, with no inputs
DestinationNode is specifically meant for graph output, there should only be one of these

Other nodes are planned:
- gain (amplifier)
- fft (frequency analysis)
- ifft (frequencies to sample)
- adsr (attack-decay-sustain-release) controller
- note (adsr, gain, frequency) controller
- biquad filter
- equalizer (something like 16 bands?)
- delay (buffer)
- reverb
- AM pitch shift
- PCM buffer (for easily playing of sounds that loop, or microphone)

## Intentions
I intend to try and compile this for arduino, rpi, and arm, but for now
I'm doing all testing on Ubuntu x64, so the included built binary is x64

This library does not depend on anything other that libc at the moment, and
hopefully I'll remove that as well soon.

This lib only renders the audio to a float[]
I am writing a stdin audio player that plays floats directly from terminal/stdin
as a pipe solution that won't require you to bind dumb libraries, but you could
just stream the data to a web browser through a web socket.

TODO - write lots of readme
