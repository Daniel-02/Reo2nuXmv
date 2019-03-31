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

void addPort(char port[20], struct Automato *automato)
{
    int exist = 0;
    for (size_t i = 0; i < automato->nPorts; i++)
    {
        if (strcmp(port, automato->ports[i]) == 0)
        {
            exist = 1;
        }
    }
    if (!exist)
    {
        if (automato->nPorts == 0)
        {
            automato->ports = malloc(sizeof(*automato->ports));
            strcpy(automato->ports[0], port);
            automato->nPorts = 1;
        }
        else
        {
            automato->ports = realloc(automato->ports, (automato->nPorts + 1) * sizeof(*automato->ports));
            strcpy(automato->ports[automato->nPorts], port);
            automato->nPorts++;
        }
    }
}

void addCondition(struct Condition *conditions, int nPorts, struct Automato *automato)
{
    for (size_t i = 0; i < nPorts; i++)
    {
        addPort(conditions[i].port, automato);
    }
}

void addPorts(struct Transition **transitions, int nTrans, struct Automato *automato)
{
    for (size_t i = 0; i < nTrans; i++)
    {
        addCondition(transitions[i]->conditions, transitions[i]->nPorts, automato);
    }
}

void addState(struct State *state, struct Automato *automato)
{
    addPorts(state->transitions, state->nTrans, automato);
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
        free(automato->ports);
        free(automato);
    }
}
