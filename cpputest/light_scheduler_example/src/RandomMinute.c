#include <stdlib.h>
#include "RandomMinute.h"

static int bound = 0;

static int RandomMinute_GetImpl(void)
{
    return bound - rand() % (bound*2 + 1);
}

int (*RandomMinute_Get)(void) = RandomMinute_GetImpl;

void RandomMinute_Create(int b)
{
    bound = b;
    srand(10);
}

