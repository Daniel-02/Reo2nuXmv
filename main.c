#include <stdio.h>
#include "headers/state.h"

int main(void)
{
    struct State *state = newState("teste", 5);
    printf(state->name);
}