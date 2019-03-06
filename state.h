#include "transition.h";
#ifndef STATE_HEADER_GUARD
#define STATE_HEADER_GUARD

typedef struct State state;

struct State
{
    char name[20];
    Transition *transitions;
};

#endif