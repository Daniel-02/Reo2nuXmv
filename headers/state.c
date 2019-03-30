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

// struct Transition newTransition(struct State *start, struct State *end, int nPorts, struct Condition *conditions)
// {
//     struct Transition transition;
//     transition.start = start;
//     transition.end = end;
//     transition.nPorts = nPorts;
//     // memcpy(&transition.ports, &ports, sizeof(ports));
//     transition.conditions = conditions;
//     return transition;
// }

// struct Condition newCondition(char port[20], char operation, char value[20])
// {
//     struct Condition condition;
//     strcpy(condition.port, port);
//     condition.operation = operation;
//     strcpy(condition.value, value);
//     return condition;
// }

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

struct Automato *newAutomato(char name[20])
{
    struct Automato *automato = (struct Automato *)malloc(sizeof(struct Automato));
    if (automato == NULL)
        return NULL;
    strcpy(automato->name, name);
    automato->nStates = 0;
    return automato;
}

void addState(struct State *state, struct Automato *automato)
{
    if (automato->nStates == 0)
    {
        automato->states = malloc(sizeof(struct State *));
        automato->states[0] = state;
        automato->nStates = 1;
    }
    else
    {
        automato->states = realloc(automato->states, (automato->nStates + 1) * sizeof(struct State));
        automato->states[automato->nStates] = state;
        automato->nStates++;
    }
}

void delAutomato(struct Automato *automato)
{
    if (automato != NULL)
    {
        for (size_t i = 0; i < automato->nStates; i++)
        {
            delState(automato->states[i]);
        }
        free(automato->states);
        free(automato);
    }
}
