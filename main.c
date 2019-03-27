#include <stdio.h>
#include <stdlib.h>
#include "headers/state.h"
#include "headers/caToNuXmv.h"

int main(void)
{
    struct State *state1 = newState("teste1");
    struct State *state2 = newState("teste2");
    struct State *state3 = newState("teste3");

    struct Condition condition = {"portA", '=', "50"};
    struct Condition *conditions = malloc(1 * sizeof(struct Condition));
    conditions[0] = condition;
    printf("executando...\n");
    struct Transition transition1 = {state1, state3, 1, conditions};
    addTransition(&transition1);
    addTransition(&transition1);
    addTransition(&transition1);
    struct Transition transition2 = {state2, state3, 1, conditions};
    struct Transition transition3 = {state3, state1, 1, conditions};
    addTransition(&transition2);
    addTransition(&transition3);

    int nStates = 3;
    struct State *states[nStates];
    states[0] = state1;
    states[1] = state2;
    states[2] = state3;

    startNuxmv(states, nStates);

    free(conditions);
    delState(state1);
    delState(state2);
    delState(state3);
}