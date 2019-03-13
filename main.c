#include <stdio.h>
#include "headers/state.h"

int main(void)
{
    struct State *state1 = newState("teste1");
    struct State *state2 = newState("teste2");
    struct State *state3 = newState("teste3");

    char ports[20] = "portA";
    struct Condition condition = newCondition(ports, "=", "50");
    struct Transition transition = newTransition(state1, state2, 1, ports, &condition);
    addTransition(transition);
    printf("executando...\n");
    printf("\n");
    printf("%d", state1->nTrans);
    struct Transition transition1 = newTransition(state1, state3, 1, ports, &condition);
    addTransition(transition1);
    // addTransition(state1, state2);
    // addTransition(state1, state3);

    printf((state1->transitions + 1)->end->name);
    printf("\n");
    printf("%d", state1->nTrans);
    delState(state1);
    delState(state2);
    delState(state3);
}