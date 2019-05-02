#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "state.h"

struct Automato *createSync(char *ports, int nAuto)
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
    while (ports[i] != ')')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] = ports.%s[time]", port1, port1, port2);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 2;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "sync%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    return automato;
}

struct Automato *createLossy(char *ports, int nAuto)
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
    while (ports[i] != ')')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] = ports.%s[time]", port1, port1, port2);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 2;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port1);
    snprintf(condition, 600, "ports.%s[time] != NULL", port1);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "lossySync%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    return automato;
}

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
    while (ports[i] != ')')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    struct State *state2 = newState("p0", 0);
    struct State *state3 = newState("p1", 0);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    snprintf(condition, 600, "ports.%s[time] = 0", port1);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state2;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port1);
    snprintf(condition, 600, "ports.%s[time] = 1", port1);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state3;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] = 1", port2);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state3;
    transition->end = state1;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] = 0", port2);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state2;
    transition->end = state1;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "fifo%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    addState(state2, automato);
    addState(state3, automato);
    return automato;
}

struct Automato *createSyncDrain(char *ports, int nAuto)
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
    while (ports[i] != ')')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] != NULL", port1, port2);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "syncDrain%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    return automato;
}

struct Automato *createAsync(char *ports, int nAuto)
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
    while (ports[i] != ')')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    snprintf(condition, 600, "ports.%s[time] != NULL", port1);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port2);
    snprintf(condition, 600, "ports.%s[time] != NULL", port2);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 1;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "asyncDrain%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    return automato;
}

struct Automato *createMerger(char *ports, int nAuto)
{
    char port1[20];
    char port2[20];
    char port3[20];
    memset(port1, 0, sizeof(port1));
    memset(port2, 0, sizeof(port2));
    memset(port3, 0, sizeof(port3));
    int i = 0, j = 0;
    while (ports[i] != ',')
    {
        port1[i] = ports[i];
        i++;
    }
    i++;
    while (ports[i] != ',')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    i++;
    j = 0;
    while (ports[i] != ')')
    {
        port3[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    portsList = addString(portsList, port3);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] = ports.%s[time]", port3, port1, port3);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 2;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    condition = (char *)malloc(600 * sizeof(char));
    portsList = NULL;
    portsList = addString(portsList, port2);
    portsList = addString(portsList, port3);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] = ports.%s[time]", port3, port2, port3);
    transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 2;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "merger%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
    return automato;
}

struct Automato *createReplicator(char *ports, int nAuto)
{
    char port1[20];
    char port2[20];
    char port3[20];
    memset(port1, 0, sizeof(port1));
    memset(port2, 0, sizeof(port2));
    memset(port3, 0, sizeof(port3));
    int i = 0, j = 0;
    while (ports[i] != ',')
    {
        port1[i] = ports[i];
        i++;
    }
    i++;
    while (ports[i] != ',')
    {
        port2[j] = ports[i];
        i++;
        j++;
    }
    i++;
    j = 0;
    while (ports[i] != ')')
    {
        port3[j] = ports[i];
        i++;
        j++;
    }
    struct State *state1 = newState("q0", 1);
    char *condition = (char *)malloc(600 * sizeof(char));
    struct StringList *portsList = NULL;
    portsList = addString(portsList, port1);
    portsList = addString(portsList, port2);
    portsList = addString(portsList, port3);
    snprintf(condition, 600, "ports.%s[time] != NULL & ports.%s[time] = ports.%s[time] & ports.%s[time] = ports.%s[time]", port1, port1, port2, port1, port3);
    struct Transition *transition = (struct Transition *)malloc(sizeof(struct Transition));
    transition->start = state1;
    transition->end = state1;
    transition->nPorts = 3;
    transition->ports = portsList;
    transition->condition = condition;
    addTransition(transition);
    char *automatoName = (char *)malloc(600 * sizeof(char));
    snprintf(automatoName, 600, "replicator%d", nAuto);
    struct Automato *automato = newAutomato(automatoName);
    addState(state1, automato);
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
        while ((line[i] != '\n') && (line[i] != '\0'))
        {
            ports[j] = line[i];
            i++;
            j++;
        }
        if (strcmp(command, "sync") == 0)
        {
            temp = createSync(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "lossySync") == 0)
        {
            temp = createLossy(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "fifo") == 0)
        {
            temp = createFifo(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "syncDrain") == 0)
        {
            temp = createSyncDrain(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "asyncDrain") == 0)
        {
            temp = createAsync(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "merger") == 0)
        {
            temp = createMerger(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
        if (strcmp(command, "replicator") == 0)
        {
            temp = createReplicator(ports, nAuto);
            automatoList = addAutomato(automatoList, temp);
        }
    }
    return automatoList;
}