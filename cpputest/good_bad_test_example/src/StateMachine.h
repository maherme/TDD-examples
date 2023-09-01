#ifndef STATEMACHINE_H
#define STATEMACHINE_H

typedef enum
{
    STATE1,
    STATE2,
    STATE3,
    STATE4,
    NUMBER_OF_STATES
} state_t;

void state_machine_init(void);
void state_machine_process(void);

#endif /* STATE_MACHINE_H */
