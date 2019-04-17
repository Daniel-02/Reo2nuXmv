#ifndef STATE_HEADER_GUARD
#define STATE_HEADER_GUARD

struct Automato
{
    char name[20];
    int nStates;
    struct StateList *states;
    int nPorts;
    char (*ports)[20];
};

struct State
{
    char name[20];
    int nTrans;
    struct TransitionList *transitions;
};

struct Transition
{
    struct State *start;
    struct State *end;
    int nPorts;
    struct ConditionList *conditions;
};

struct Condition
{
    char port[20];
    char operation;
    char value[20];
};

struct StateList
{
    struct State *state;
    struct StateList *nextState;
};

struct TransitionList
{
    struct Transition *transition;
    struct TransitionList *nextTransition;
};

struct StringList
{
    char *string;
    struct StringList *nextString;
};

struct AutomatoList
{
    struct Automato *automato;
    struct AutomatoList *nextAutomato;
};

struct ConditionList
{
    struct Condition *condition;
    struct ConditionList *nextCondition;
};

struct State *newState(char name[20]);

void addTransition(struct Transition *transition);

void delState(struct State *state);

struct Automato *newAutomato(char name[20]);

void addState(struct State *state, struct Automato *automato);

void delStates(struct Automato *automato);

struct State *findState(struct StateList *states, char name[20]);

void delAutomatoList(struct AutomatoList *automatos);

struct StringList *addString(struct StringList *stringlist, char *string);

struct StringList *delString(struct StringList *stringlist, char *string);

void delStringList(struct StringList *stringList);

struct StringList *cpyStringList(struct StringList *newList, struct StringList *stringList);

struct AutomatoList *addAutomato(struct AutomatoList *automatoList, struct Automato *automato);

struct ConditionList *addConditionToList(struct ConditionList *conditionList, struct Condition *condition);

#endif