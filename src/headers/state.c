#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "state.h"

struct State *newState(char name[20])
{
    struct State *state = (struct State *)malloc(sizeof(struct State));
    if (state == NULL)
        return NULL;
    strcpy(state->name, name);
    state->nTrans = 0;
    state->transitions = NULL;
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
    // if (stateStart->nTrans == 0)
    // {
    //     stateStart->transitions = malloc(sizeof(struct Transition *));
    //     stateStart->transitions[0] = transition;
    //     stateStart->nTrans = 1;
    // }
    // else
    // {
    //     stateStart->transitions = realloc(stateStart->transitions, (stateStart->nTrans + 1) * sizeof(struct Transition));
    //     stateStart->transitions[stateStart->nTrans] = transition;
    //     stateStart->nTrans++;
    // }
    if (stateStart->transitions == NULL)
    {
        stateStart->transitions = (struct TransitionList *)malloc(sizeof(struct TransitionList));
        stateStart->transitions->transition = transition;
        stateStart->transitions->nextTransition = NULL;
        return;
    }
    struct TransitionList *tempTransition = stateStart->transitions;
    while (tempTransition->nextTransition != NULL)
        tempTransition = tempTransition->nextTransition;
    tempTransition->nextTransition = (struct TransitionList *)malloc(sizeof(struct TransitionList));
    tempTransition->nextTransition->transition = transition;
    tempTransition->nextTransition->nextTransition = NULL;
}

void delTransitionList(struct TransitionList *transitions)
{
    if (transitions == NULL)
        return;
    delTransitionList(transitions->nextTransition);
    free(transitions);
}

void delState(struct State *state)
{
    if (state != NULL)
    {
        if (state->transitions != NULL)
            delTransitionList(state->transitions);
        free(state);
    }
}

struct State *findState(struct StateList *states, char name[20])
{
    while (states != NULL)
    {
        if (strcmp(states->state->name, name) == 0)
            return states->state;
        states = states->nextState;
    }
    return NULL;
}

struct Automato *newAutomato(char name[20])
{
    struct Automato *automato = (struct Automato *)malloc(sizeof(struct Automato));
    if (automato == NULL)
        return NULL;
    strcpy(automato->name, name);
    automato->nStates = 0;
    automato->states = NULL;
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

void addCondition(struct ConditionList *conditions, int nPorts, struct Automato *automato)
{
    while (conditions)
    {
        addPort(conditions->condition->port, automato);
        conditions = conditions->nextCondition;
    }
}

void addPorts(struct TransitionList *transitions, int nTrans, struct Automato *automato)
{
    while (transitions != NULL)
    {
        addCondition(transitions->transition->conditions, transitions->transition->nPorts, automato);
        transitions = transitions->nextTransition;
    }
}

void addState(struct State *state, struct Automato *automato)
{
    addPorts(state->transitions, state->nTrans, automato);
    // if (automato->nStates == 0)
    // {
    //     automato->states = malloc(sizeof(struct State *));
    //     automato->states[0] = state;
    //     automato->nStates = 1;
    // }
    // else
    // {
    //     automato->states = realloc(automato->states, (automato->nStates + 1) * sizeof(struct State));
    //     automato->states[automato->nStates] = state;
    //     automato->nStates++;
    // }

    if (automato->states == NULL)
    {
        automato->states = (struct StateList *)malloc(sizeof(struct StateList));
        automato->states->state = state;
        automato->states->nextState = NULL;
        return;
    }
    struct StateList *tempState = automato->states;
    while (tempState->nextState != NULL)
        tempState = tempState->nextState;
    tempState->nextState = (struct StateList *)malloc(sizeof(struct StateList));
    tempState->nextState->state = state;
    tempState->nextState->nextState = NULL;
}

// void delStates(struct Automato *automato)
// {
//     if (automato != NULL)
//     {
//         while ()
//         {
//             delState(automato->states[i]);
//         }
//     }
// }

void delStatesList(struct StateList *states)
{
    if (states == NULL)
        return;
    delStatesList(states->nextState);
    delState(states->state);
    free(states);
}

void delAutomato(struct Automato *automato)
{
    if (automato != NULL)
    {
        if (automato->states)
        {
            delStatesList(automato->states);
        }
        free(automato->ports);
        free(automato);
    }
}

struct StringList *addString(struct StringList *stringlist, char *string)
{
    if (stringlist == NULL)
    {
        stringlist = (struct StringList *)malloc(sizeof(struct StringList));
        stringlist->string = (char *)malloc(600 * sizeof(char));
        strcpy(stringlist->string, string);
        stringlist->nextString = NULL;
        return stringlist;
    }
    struct StringList *tempString = stringlist;
    while (tempString->nextString != NULL)
        tempString = tempString->nextString;
    tempString->nextString = (struct StringList *)malloc(sizeof(struct StringList));
    tempString->nextString->string = (char *)malloc(600 * sizeof(char));
    strcpy(tempString->nextString->string, string);
    tempString->nextString->nextString = NULL;
    return stringlist;
}

void delStringList(struct StringList *stringList)
{
    if (stringList == NULL)
        return;
    delStringList(stringList->nextString);
    free(stringList->string);
    free(stringList);
}

struct StringList *delString(struct StringList *stringList, char *string)
{
    if (stringList == NULL || string == NULL)
    {
        return stringList;
    }
    struct StringList *temp = stringList;
    struct StringList *first = stringList;
    if (strcmp(stringList->string, string) == 0)
    {
        temp = stringList->nextString;
        free(stringList);
        return temp;
    }
    stringList = stringList->nextString;
    while (stringList != NULL)
    {
        if (strcmp(stringList->string, string) == 0)
        {
            temp->nextString = stringList->nextString;
            free(stringList);
            return first;
        }
        temp = stringList;
        stringList = stringList->nextString;
    }
    return first;
}

struct StringList *cpyStringList(struct StringList *newList, struct StringList *stringList)
{
    if (stringList == NULL)
        return NULL;
    newList = (struct StringList *)malloc(sizeof(struct StringList));
    newList->string = (char *)malloc(600 * sizeof(char));
    strcpy(newList->string, stringList->string);
    newList->nextString = cpyStringList(newList->nextString, stringList->nextString);
    return newList;
}

struct AutomatoList *addAutomato(struct AutomatoList *automatoList, struct Automato *automato)
{
    if (automatoList == NULL)
    {
        automatoList = (struct AutomatoList *)malloc(sizeof(struct AutomatoList));
        automatoList->automato = automato;
        automatoList->nextAutomato = NULL;
        return automatoList;
    }
    struct AutomatoList *tempAutomato = automatoList;
    while (tempAutomato->nextAutomato != NULL)
        tempAutomato = tempAutomato->nextAutomato;
    tempAutomato->nextAutomato = (struct AutomatoList *)malloc(sizeof(struct AutomatoList));
    tempAutomato->nextAutomato->automato = automato;
    tempAutomato->nextAutomato->nextAutomato = NULL;
    return automatoList;
}

struct ConditionList *addConditionToList(struct ConditionList *conditionList, struct Condition *condition)
{
    if (conditionList == NULL)
    {
        conditionList = (struct ConditionList *)malloc(sizeof(struct ConditionList));
        conditionList->condition = condition;
        conditionList->nextCondition = NULL;
        return conditionList;
    }
    struct ConditionList *tempCondition = conditionList;
    while (tempCondition->nextCondition != NULL)
        tempCondition = tempCondition->nextCondition;
    tempCondition->nextCondition = (struct ConditionList *)malloc(sizeof(struct ConditionList));
    tempCondition->nextCondition->condition = condition;
    tempCondition->nextCondition->nextCondition = NULL;
    return conditionList;
}
