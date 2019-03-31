#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

void nullPorts(struct Automato *automato, struct Transition *transition, FILE *f)
{
    int exists = 0;
    for (size_t i = 0; i < automato->nPorts; i++)
    {
        exists = 0;
        for (size_t j = 0; j < transition->nPorts; j++)
        {
            if (strcmp(automato->ports[i], transition->conditions[j].port) == 0)
                exists = 1;
        }
        if (!exists)
            fprintf(f, "ports.%s = NULL & ", automato->ports[i]);
    }
}

void caToNuxmv(struct Automato *automato, FILE *f)
{
    int nStates = automato->nStates;
    struct State **states = automato->states;
    fprintf(f, "MODULE %s(time)\nVAR\n\tports: ", "automato");
    fprintf(f, "ports");
    fprintf(f, "cs: {");
    for (size_t i = 0; i < nStates; i++)
    {
        fprintf(f, "%s%s", states[i]->name, i < nStates - 1 ? "," : "");
    }
    fprintf(f, "};\n");
    fprintf(f, "TRANS\n\t");
    struct Transition **transitions;
    for (size_t i = 0; i < nStates; i++)
    {
        transitions = states[i]->transitions;
        for (size_t j = 0; j < states[i]->nTrans; j++)
        {
            fprintf(f, "((cs = %s & ", transitions[j]->start->name);
            nullPorts(automato, transitions[j], f);
            for (size_t k = 0; k < transitions[j]->nPorts; k++)
            {
                fprintf(f, "ports.%s %c %s%s", transitions[j]->conditions[k].port, transitions[j]->conditions[k].operation,
                        transitions[j]->conditions[k].value, k < transitions[j]->nPorts - 1 ? " & " : ") ");
            }
            fprintf(f, "-> next(cs) = %s)%s", transitions[j]->end->name, i < nStates - 1 ? " & " : ";\n\n");
        }
    }
    // fprintf(f, ";\n");
}

void startNuxmv(struct Automato **automatos, int nAutomatos)
{
    FILE *f = fopen("nuxmv.smv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "MODULE main\nVAR\n\ttime: 0..3;\n\t%s: %s(time);\n", "automato", "automato");
    fprintf(f, "ASSIGN\n\tinit(time) := 0;\n\tnext(time) := case\n\t\ttime < 3: time + 1;\n\t\tTRUE: time;\nesac;\n\n");
    for (size_t i = 0; i < nAutomatos; i++)
    {
        caToNuxmv(automatos[i], f);
    }

    fclose(f);
}