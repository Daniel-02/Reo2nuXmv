#ifndef STATE_HEADER_GUARD
#define STATE_HEADER_GUARD

struct State
{
    char name[20];
    struct Transition *transitions;
};

struct Transition
{
    struct State *start;
    struct State *end;
    char *ports[20];
};

struct State *newState(char name[20], int nTrans);

void delState(struct State *state);

#endif