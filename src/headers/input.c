#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "state.h"

struct Automato *createFifo(char *ports, int nAuto)
{
    char port1[20];
    char port2[20];
    memset(port1, 0, sizeof(port1));
    memset(port2, 0, sizeof(port2));
    int i = 0, j = 0;
    while (ports[i] != ',')
    {
        port1[i] = ports[i];
        i++;
    }
    i++;
    while (ports[i] != '\0')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0");
    struct State *state2 = newState("p0");
    struct State *state3 = newState("p1");
    struct ConditionList *conditions1 = NULL;
    struct ConditionList *conditions2 = NULL;
    struct ConditionList *conditions3 = NULL;
    struct ConditionList *conditions4 = NULL;
    struct Condition *condition = (struct Condition *)malloc(sizeof(struct Condition));
    strcpy(condition->port, port1);
    condition->operation = '=';
    strcpy(condition->value, "0\0");
    conditions1 = addConditionToList(conditions1, condition);
    condition = (struct Condition *)malloc(sizeof(struct Condition));
    strcpy(condition->port, port2);
    condition->operation = '=';
    strcpy(condition->value, "0\0");
    conditions2 = addConditionToList(conditions2, condition);
    condition = (struct Condition *)malloc(sizeof(struct Condition));
    strcpy(condition->port, port1);
    condition->operation = '=';
    strcpy(condition->value, "1\0");
    conditions3 = addConditionToList(conditions3, condition);
    condition = (struct Condition *)malloc(sizeof(struct Condition));
    strcpy(condition->port, port2);
    condition->operation = '=';
    strcpy(condition->value, "1\0");
    conditions4 = addConditionToList(conditions4, condition);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state2;
    transition->nPorts = 1;
    transition->conditions = conditions1;
    addTransition(transition);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state2;
    transition->end = state1;
    transition->nPorts = 1;
    transition->conditions = conditions2;
    addTransition(transition);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state3;
    transition->nPorts = 1;
    transition->conditions = conditions3;
    addTransition(transition);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state3;
    transition->end = state1;
    transition->nPorts = 1;
    transition->conditions = conditions4;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "fifo%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    addState(state2, automato);
    addState(state3, automato);
    return automato;
}

struct AutomatoList *
readInput(FILE *f)
{
    struct AutomatoList *automatoList = NULL;
    struct Automato *temp = NULL;
    char line[128];
    char command[128];
    char ports[128];
    int i = 0;
    int j = 0;
    int nAuto = 0;
    while (fgets(line, sizeof line, f) != NULL)
    {
        i = 0;
        j = 0;
        nAuto++;
        memset(command, 0, sizeof(command));
        memset(ports, 0, sizeof(ports));
        while (line[i] != '(')
        {
            command[i] = line[i];
            i++;
        }
        i++;
        while (line[i] != ')')
        {
            ports[j] = line[i];
            i++;
            j++;
        }
        if (strcmp(command, "fifo") == 0)
        {
            temp = createFifo(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
    }
    return automatoList;
}