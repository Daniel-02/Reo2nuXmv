#ifndef CATONUXMV_HEADER_GUARD
#define CATONUXMV_HEADER_GUARD

void startNuxmv(struct Automato **automato, int nAutomatos);

void caToNuxmv(struct State **states, int nStates, FILE *f);

#endif