#ifndef CATONUXMV_HEADER_GUARD
#define CATONUXMV_HEADER_GUARD

void startNuxmv(struct State **states, int nStates);

void caToNuxmv(struct State **states, int nStates, FILE *f);

#endif