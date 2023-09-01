#include <stddef.h>
#include "StateMachine.h"
#include "InputsOutputs.h"

state_t state;
int internal_condition = 0;

typedef void transition_function_t(void);
typedef void state_function_t(void);
static void transition_error(void);

static void do_state1(void);
static void do_state2(void);
static void do_state3(void);
static void do_state4(void);

static void do_state1_to_state2(void);
static void do_state2_to_state3(void);
static void do_state3_to_state4(void);

state_function_t* const state_table[NUMBER_OF_STATES] =
{
    do_state1,
    do_state2,
    do_state3,
    do_state4,
};

transition_function_t * const transitions_table[NUMBER_OF_STATES][NUMBER_OF_STATES] =
{           /* STATE1   STATE2                STATE3                STATE4                */
/*STATE1*/  {  NULL,    do_state1_to_state2,  NULL,                 NULL                  },
/*STATE2*/  {  NULL,    NULL,                 do_state2_to_state3,  NULL                  },
/*STATE3*/  {  NULL,    NULL,                 NULL,                 do_state3_to_state4   },
/*STATE4*/  {  NULL,    NULL,                 NULL,                 NULL                  },
};

void state_machine_init(void)
{
    state = STATE1;
}

void state_machine_process(void)
{
    state_t current_state = state;

    state_table[current_state]();

    transition_function_t *transition = transitions_table[current_state][state];

    if(transition != NULL)
        transition();
    else
    {
        if(current_state != state)
            transition_error();
    }
}

static void do_state1(void)
{
    state_t temporal_state = STATE1;

    if(input() == 1)
        temporal_state = STATE2;

    state = temporal_state;
}

static void do_state2(void)
{
    state_t temporal_state = STATE2;

    if(input() == 2)
//    if(input() == 2 && internal_condition == 1)
        temporal_state = STATE3;

    state = temporal_state;
}

static void do_state3(void)
{
    state_t temporal_state = STATE3;

    if(input() == 3)
        temporal_state = STATE4;

    state = temporal_state;
}

static void do_state4(void)
{
    state_t temporal_state = STATE4;

    state = temporal_state;
}

static void do_state1_to_state2(void)
{
    output(1);
//    internal_condition = 1;
}

static void do_state2_to_state3(void)
{
    output(2);
}

static void do_state3_to_state4(void)
{
    output(3);
}

static void transition_error(void)
{
    output(0);
}

