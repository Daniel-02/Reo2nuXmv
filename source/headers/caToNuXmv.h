#ifndef CATONUXMV_HEADER_GUARD
#define CATONUXMV_HEADER_GUARD

void startNuxmv(struct Automato **automato, int nAutomatos);

void caToNuxmv(struct Automato *automato, FILE *f);

#endif