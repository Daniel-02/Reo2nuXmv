#include <stdio.h>
#include <stdlib.h>
#include "state.h"

void caToNuxmv(struct State **states, int nStates, FILE *f)
{
    fprintf(f, "MODULE %s(time)\nVAR\n\tports: ", "automato");
    fprintf(f, "ports");
    fprintf(f, "cs: {");
    for (size_t i = 0; i < nStates; i++)
    {
        fprintf(f, "%s, ", states[i]->name);
    }
    fprintf(f, "};\n");
    fprintf(f, "TRANS\n\t");
    struct Transition **transitions;
    for (size_t i = 0; i < nStates; i++)
    {
        transitions = states[i]->transitions;
        for (size_t j = 0; j < states[i]->nTrans; j++)
        {
            fprintf(f, "cs=%s &", transitions[j]->start->name);
            for (size_t k = 0; k < transitions[j]->nPorts; k++)
            {
                fprintf(f, "ports.%s %c %s  &", transitions[j]->conditions[k].port, transitions[j]->conditions[k].operation, transitions[j]->conditions[k].value);
            }
            fprintf(f, "-> next(cs) = %s &", transitions[j]->end->name);
        }
    }
    fprintf(f, ";\n");
}

void startNuxmv(struct State **states, int nStates)
{
    FILE *f = fopen("nuxmv.smv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "MODULE main\nVAR\n\ttime: 0..3;\n\t%s: %s(time);\n", "automato", "automato");
    fprintf(f, "ASSIGN\n\tinit(time) := 0;\n\tnext(time) := case\n\t\ttime < 3: time + 1;\n\t\tTRUE: time;\nesac;\n\n");
    caToNuxmv(states, nStates, f);

    fclose(f);
}