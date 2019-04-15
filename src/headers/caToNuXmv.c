#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

void nullPorts(struct Automato *automato, struct Transition *transition, FILE *f)
{
    int exists = 0;
    struct ConditionList *conditions = transition->conditions;
    for (size_t i = 0; i < automato->nPorts; i++)
    {
        exists = 0;
        while (conditions != NULL)
        {
            if (strcmp(automato->ports[i], conditions->condition->port) == 0)
                exists = 1;
            conditions = conditions->nextCondition;
        }
        if (!exists)
            fprintf(f, "ports.%s = NULL & ", automato->ports[i]);
    }
}

void caToNuxmv(struct Automato *automato, FILE *f)
{
    int nStates = automato->nStates;
    struct StateList *states = automato->states;
    fprintf(f, "MODULE %s(time)\nVAR\n\tports: ", automato->name);
    fprintf(f, "ports\n");
    fprintf(f, "\tcs: {");
    while (states != NULL)
    {
        fprintf(f, "%s%s", states->state->name, states->nextState != NULL ? "," : "");
        states = states->nextState;
    }
    states = automato->states;
    fprintf(f, "};\n");
    fprintf(f, "TRANS\n\t");
    struct TransitionList *transitions;
    struct ConditionList *conditions;
    while (states != NULL)
    {
        transitions = states->state->transitions;
        while (transitions != NULL)
        {
            fprintf(f, "((cs = %s & ", transitions->transition->start->name);
            // nullPorts(automato, transitions->transition, f);
            conditions = transitions->transition->conditions;
            while (conditions != NULL)
            {
                fprintf(f, "ports.%s %c %s%s", transitions->transition->conditions->condition->port,
                        transitions->transition->conditions->condition->operation,
                        transitions->transition->conditions->condition->value, conditions->nextCondition != NULL ? " & " : ") ");
                conditions = conditions->nextCondition;
            }
            fprintf(f, "-> next(cs) = %s)%s", transitions->transition->end->name, states->nextState != NULL ? " & " : ";\n\n");
            transitions = transitions->nextTransition;
        }
        states = states->nextState;
    }
}

struct StringList *portsIntersection(struct Transition *transition, struct Automato *automato)
{
    struct StringList *list = NULL;
    char *concat = (char *)malloc(60 * sizeof(char));
    struct ConditionList *conditions = transition->conditions;
    while (conditions != NULL)
    {
        for (size_t j = 0; j < automato->nPorts; j++)
        {
            if (strcmp(conditions->condition->port, automato->ports[j]) == 0)
            {
                strcpy(concat, automato->ports[j]);
                list = addString(list, concat);
            }
        }
        conditions = conditions->nextCondition;
    }
    return list;
}

int equalIntersections(struct StringList *list1, struct StringList *list2)
{
    if (list1 == NULL && list2 != NULL)
    {
        return 0;
    }
    int equal = 1;
    int matches = 0;
    struct StringList *list1Start = list1;
    struct StringList *list2Start = list2;
    int list1Len = 0;
    int list2Len = 0;
    while (list1 != NULL)
    {
        while (list2 != NULL)
        {
            if (list1Len == 0)
                list2Len++;
            if (strcmp(list1->string, list2->string) == 0)
                matches = 1;
            break;
            list2 = list2->nextString;
        }
        list1Len++;
        if (!matches)
            return 0;
        matches = 0;
        list2 = list2Start;
        list1 = list1->nextString;
    }
    if (list2Len > list1Len)
    {
        list1 = list1Start;
        list2 = list2Start;
        while (list2 != NULL)
        {
            while (list1 != NULL)
            {
                if (strcmp(list2->string, list1->string) == 0)
                    matches = 1;
                break;
                list1 = list1->nextString;
            }
            if (!matches)
                return 0;
            matches = 0;
            list1 = list1Start;
            list2 = list2->nextString;
        }
    }
    return 1;
}

int equalPorts(struct Transition *transition1, struct Transition *transition2)
{
    int equal = 0;
    struct ConditionList *biggerConditions;
    struct ConditionList *lesserConditions;
    struct ConditionList *tempLesserConditions;
    if (transition1->nPorts > transition2->nPorts)
    {
        biggerConditions = transition1->conditions;
        lesserConditions = transition2->conditions;
    }
    else
    {
        biggerConditions = transition2->conditions;
        lesserConditions = transition1->conditions;
    }
    tempLesserConditions = lesserConditions;
    while (biggerConditions != NULL)
    {
        while (lesserConditions != NULL)
        {
            if (strcmp(biggerConditions->condition->port, lesserConditions->condition->port) == 0)
            {
                equal = 1;
                break;
            }
            lesserConditions = lesserConditions->nextCondition;
        }
        if (!equal)
            return 0;
        equal = 0;
        biggerConditions = biggerConditions->nextCondition;
        lesserConditions = tempLesserConditions;
    }
    return 1;
}

void printsList(struct StringList *list)
{
    while (list != NULL)
    {
        printf("%s\n", list->string);
        list = list->nextString;
    }
}

void printToNuXmv(struct StringList *trans, struct StringList *states, struct StringList *invar, FILE *f)
{
    fprintf(f, "MODULE %s(time)\nVAR\n\tports: ", "prod");
    fprintf(f, "ports\n");
    fprintf(f, "\tcs: {");
    while (states != NULL)
    {
        fprintf(f, "%s%s", states->string, states->nextString != NULL ? "," : "");
        states = states->nextString;
    }
    fprintf(f, "};\n");
    fprintf(f, "TRANS\n");
    while (trans != NULL)
    {
        fprintf(f, "\t%s%s", trans->string, trans->nextString != NULL ? " &\n" : ";\n");
        trans = trans->nextString;
    }
    fprintf(f, "INVAR\n");
    while (invar != NULL)
    {
        fprintf(f, "\t%s%s", invar->string, invar->nextString != NULL ? " &\n" : ";\n\n");
        invar = invar->nextString;
    }
}

void productInSmv(struct AutomatoList *automatos, FILE *f)
{
    struct Automato *automato1 = automatos->automato;
    struct Automato *automato2;
    char *concat = (char *)malloc(60 * sizeof(char));
    char *transString = (char *)malloc(60 * sizeof(char));
    char *portString = (char *)malloc(6000 * sizeof(char));
    struct StringList *intersection1 = NULL;
    struct StringList *intersection2 = NULL;
    struct StateList *states1;
    struct StateList *states2;
    struct TransitionList *transitions1 = NULL;
    struct TransitionList *transitions2 = NULL;
    struct StringList *trans = NULL;
    struct StringList *invar = NULL;
    struct StringList *states = NULL;
    struct StringList *inalcStates = NULL;
    struct ConditionList *conditions;
    int firstPass = 1;

    while (automatos->nextAutomato != NULL)
    {
        automato2 = automatos->nextAutomato->automato;
        states1 = automato1->states;
        while (states1 != NULL)
        {
            states2 = automato2->states;
            while (states2 != NULL)
            {
                snprintf(concat, 600, "%s%s", states1->state->name, states2->state->name);
                states = addString(states, concat);
                snprintf(transString, 600, "(((prod1.cs = %s) & (prod2.cs = %s)) <-> (cs = %s)", states1->state->name, states2->state->name, concat);
                invar = addString(invar, transString);
                states2 = states2->nextState;
            }
            states1 = states1->nextState;
        }
        states1 = automato1->states;
        states2 = automato2->states;
        while (states1 != NULL)
        {
            while (states2 != NULL)
            {
                transitions1 = states1->state->transitions;
                delStringList(inalcStates);
                inalcStates = cpyStringList(inalcStates, states);
                snprintf(concat, 600, "%s%s", states1->state->name, states2->state->name);
                inalcStates = delString(inalcStates, concat);
                firstPass = 1;
                while (transitions1 != NULL)
                {
                    intersection1 = portsIntersection(transitions1->transition, automato2);
                    if (intersection1 == NULL)
                    {
                        snprintf(concat, 600, "%s%s", transitions1->transition->end->name, states2->state->name);
                        inalcStates = delString(inalcStates, concat);
                    }
                    transitions2 = states2->state->transitions;
                    while (transitions2 != NULL)
                    {
                        intersection2 = portsIntersection(transitions2->transition, automato1);
                        if (intersection2 == NULL && firstPass)
                        {
                            snprintf(concat, 600, "%s%s", states1->state->name, transitions2->transition->end->name);
                            inalcStates = delString(inalcStates, concat);
                        }
                        if (equalIntersections(intersection1, intersection2))
                        {
                            snprintf(concat, 600, "%s%s", transitions1->transition->end->name, transitions2->transition->end->name);
                            inalcStates = delString(inalcStates, concat);
                            if (!equalPorts(transitions1->transition, transitions2->transition))
                            {
                                strcpy(portString, "");
                                strcpy(transString, "");
                                snprintf(transString, 60, "((cs = %s%s) & (", states1->state->name, states2->state->name);
                                conditions = transitions1->transition->conditions;
                                while (conditions != NULL)
                                {
                                    snprintf(portString, 60, "ports.%s %c %s & ", transitions1->transition->conditions->condition->port, transitions1->transition->conditions->condition->operation,
                                             transitions1->transition->conditions->condition->value);
                                    strcat(transString, portString);
                                    conditions = conditions->nextCondition;
                                }
                                conditions = transitions2->transition->conditions;
                                while (conditions != NULL)
                                {
                                    snprintf(portString, 60, "ports.%s %c %s%s", transitions2->transition->conditions->condition->port, transitions2->transition->conditions->condition->operation,
                                             transitions2->transition->conditions->condition->value, conditions->nextCondition != NULL ? " & " : ") ");
                                    strcat(transString, portString);
                                    conditions = conditions->nextCondition;
                                }
                                snprintf(portString, 60, " -> next(cs) = %s%s)", transitions1->transition->end->name, transitions2->transition->end->name);
                                strcat(transString, portString);
                                trans = addString(trans, transString);
                            }
                        }
                        transitions2 = transitions2->nextTransition;
                    }
                    transitions1 = transitions1->nextTransition;
                    firstPass = 0;
                }
                if (states1->state->transitions == NULL)
                {
                    transitions2 = states2->state->transitions;
                    while (transitions2 != NULL)
                    {
                        intersection2 = portsIntersection(transitions2->transition, automato1);
                        if (intersection2 == NULL)
                        {
                            snprintf(concat, 60, "%s%s", states1->state->name, transitions2->transition->end->name);
                            inalcStates = delString(inalcStates, concat);
                        }
                        transitions2 = transitions2->nextTransition;
                    }
                }
                snprintf(transString, 600, "((cs = %s%s) -> (", states1->state->name, states2->state->name);
                while (inalcStates != NULL)
                {
                    snprintf(concat, 60, "(next(cs) != %s)%s", inalcStates->string, inalcStates->nextString != NULL ? " & " : ")");
                    strcat(transString, concat);
                    inalcStates = inalcStates->nextString;
                }
                trans = addString(trans, transString);
                states2 = states2->nextState;
            }
            states2 = automato2->states;
            states1 = states1->nextState;
        }
        printToNuXmv(trans, states, invar, f);
    }
}

void automatoFromProduct(struct Automato **automatos, int nAutomatos, FILE *f)
{
    struct Automato *automato1 = automatos[0];
    struct Automato *automato2;
    char *concat = (char *)malloc(60 * sizeof(char));
    char *transString = (char *)malloc(60 * sizeof(char));
    char *portString = (char *)malloc(6000 * sizeof(char));
    struct StringList *intersection1 = NULL;
    struct StringList *intersection2 = NULL;
    struct StateList *states1;
    struct StateList *states2;
    struct TransitionList *transitions1 = NULL;
    struct TransitionList *transitions2 = NULL;
    struct State *tempState = NULL;
    int firstPass = 1;

    for (size_t i = 1; i < nAutomatos; i++)
    {
        automato2 = automatos[i];
        states1 = automato1->states;
        states2 = automato2->states;
        states1 = automato1->states;
        while (states1 != NULL)
        {
            states2 = automato2->states;
            while (states2 != NULL)
            {
                snprintf(concat, 600, "%s%s", states1->state->name, states2->state->name);
                tempState = newState(concat);
                states2 = states2->nextState;
            }
            states1 = states1->nextState;
        }
        states1 = automato1->states;
        states2 = automato2->states;
        while (states1 != NULL)
        {
            while (states2 != NULL)
            {
                transitions1 = states1->state->transitions;
                while (transitions1 != NULL)
                {
                    intersection1 = portsIntersection(transitions1->transition, automato2);
                    if (intersection1 == NULL)
                    {
                        snprintf(concat, 600, "%s%s", transitions1->transition->end->name, states2->state->name);
                    }
                    transitions2 = states2->state->transitions;
                    while (transitions2 != NULL)
                    {
                        intersection2 = portsIntersection(transitions2->transition, automato1);
                        if (intersection2 == NULL && firstPass)
                        {
                        }
                        if (equalIntersections(intersection1, intersection2))
                        {
                            if (!equalPorts(transitions1->transition, transitions2->transition))
                            {
                            }
                        }
                        transitions2 = transitions2->nextTransition;
                    }
                    transitions1 = transitions1->nextTransition;
                    firstPass = 0;
                }
                if (states1->state->transitions == NULL)
                {
                    transitions2 = states2->state->transitions;
                    while (transitions2 != NULL)
                    {
                        intersection2 = portsIntersection(transitions2->transition, automato1);
                        if (intersection2 == NULL)
                        {
                        }
                        transitions2 = transitions2->nextTransition;
                    }
                }
                snprintf(transString, 600, "((cs = %s%s) -> (", states1->state->name, states2->state->name);
                states2 = states2->nextState;
            }
            states2 = automato2->states;
            states1 = states1->nextState;
        }
        // printToNuXmv(trans, states, invar, f);
    }
}

void portsToNuXmv(FILE *f)
{
    fprintf(f, "MODULE portsModule\nFROZENVAR\n\tportA: array 0..3 of {NULL, 30, 50};\n\tportA: array 0..3 of {NULL, 30, 50};\n");
    fprintf(f, "ASSIGN\n");
    fprintf(f, "\tinit(portA[0]) := 30;\n\tinit(portB[0]) := NULL;\n");
    fprintf(f, "\tinit(portA[1]) := NULL;\n\tinit(portB[1]) := 50;\n");
    fprintf(f, "\tinit(portA[2]) := 50;\n\tinit(portB[2]) := 30;\n");
}

void startNuxmv(struct AutomatoList *automatos)
{
    struct AutomatoList *automatoList = automatos;
    FILE *f = fopen("nuxmv.smv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "MODULE main\nVAR\n\ttime: 0..3;\n\t%s: %s(time);\n", "prod", "prod");
    fprintf(f, "ASSIGN\n\tinit(time) := 0;\n\tnext(time) := case\n\t\ttime < 3: time + 1;\n\t\tTRUE: time;\nesac;\n\n");
    while (automatoList != NULL)
    {
        caToNuxmv(automatoList->automato, f);
        automatoList = automatoList->nextAutomato;
    }
    // productInSmv(automatos, f);
    portsToNuXmv(f);

    fclose(f);
}