#ifndef STATE_HEADER_GUARD
#define STATE_HEADER_GUARD

struct State
{
    char name[20];
    int nTrans;
    struct Transition *transitions;
};

struct Transition
{
    struct State *start;
    struct State *end;
    int nPorts;
    char *ports[20];
    struct Condition *conditions;
};

struct Condition
{
    char port[20];
    char operation;
    char value[20];
};

struct State *newState(char name[20]);

struct Transition newTransition(struct State *start, struct State *end, int nPorts, char *ports[20], struct Condition *conditions);

struct Condition newCondition(char port[20], char operation, char value[20]);

void addTransition(struct Transition transition);

void delState(struct State *state);

#endif