#ifndef STATE_HEADER_GUARD
#define STATE_HEADER_GUARD

struct Automato
{
    char name[20];
    int nStates;
    struct State **states;
};

struct State
{
    char name[20];
    int nTrans;
    struct Transition **transitions;
};

struct Transition
{
    struct State *start;
    struct State *end;
    int nPorts;
    struct Condition *conditions;
};

struct Condition
{
    char port[20];
    char operation;
    char value[20];
};

struct State *newState(char name[20]);

void addTransition(struct Transition *transition);

void delState(struct State *state);

struct Automato *newAutomato(char name[20]);

void addState(struct State *state, struct Automato *automato);

void delAutomato(struct Automato *automato);

#endif