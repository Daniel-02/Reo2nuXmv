#include "state.h";
#ifndef TRANSITION_HEADER_GUARD
#define TRANSITION_HEADER_GUARD

typedef struct Transition transition;

struct Transition
{
    State start;
    State end;
    char *ports[20];
};

#endif