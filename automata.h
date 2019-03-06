#include "state.h";
#ifndef AUTOMATA_HEADER_GUARD
#define AUTOMATA_HEADER_GUARD

typedef struct Automata automata;

struct Automata
{
    char name[20];
    State *states;
};

#endif