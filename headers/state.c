#include <stdlib.h>
#include <string.h>
#include "state.h"

struct State *newState(char name[20])
{
    struct State *state = (struct State *)malloc(sizeof(struct State));
    if (state == NULL)
        return NULL;
    strcpy(state->name, name);
    state->nTrans = 0;
    return state;
}

struct Transition newTransition(struct State *start, struct State *end, int nPorts, struct Condition *conditions)
{
    struct Transition transition;
    transition.start = start;
    transition.end = end;
    transition.nPorts = nPorts;
    // memcpy(&transition.ports, &ports, sizeof(ports));
    transition.conditions = conditions;
    return transition;
}

struct Condition newCondition(char port[20], char operation, char value[20])
{
    struct Condition condition;
    strcpy(condition.port, port);
    condition.operation = operation;
    strcpy(condition.value, value);
    return condition;
}

void addTransition(struct Transition *transition)
{
    struct State *stateStart = transition->start;
    if (stateStart->nTrans == 0)
    {
        stateStart->transitions = malloc(sizeof(struct Transition *));
        stateStart->transitions[0] = transition;
        stateStart->nTrans = 1;
    }
    else
    {
        stateStart->transitions = realloc(stateStart->transitions, (stateStart->nTrans + 1) * sizeof(struct Transition));
        stateStart->transitions[stateStart->nTrans] = transition;
        stateStart->nTrans++;
    }
}

void delState(struct State *state)
{
    if (state != NULL)
    {
        free(state->transitions);
        free(state);
    }
}
