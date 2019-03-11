#include <stdlib.h>
#include <string.h>
#include "state.h"

struct State *newState(char name[20], int nTrans)
{
    struct State *state = (struct State *)malloc(sizeof(struct State));
    if (state == NULL)
        return NULL;
    state->transitions = (struct Transition *)malloc(nTrans * sizeof(struct Transition));
    if (state->transitions == NULL)
    {
        free(state);
        return NULL;
    }
    strcpy(state->name, name);
    return state;
}

void delState(struct State *state)
{
    if (state != NULL)
    {
        free(state->transitions);
        free(state);
    }
}
