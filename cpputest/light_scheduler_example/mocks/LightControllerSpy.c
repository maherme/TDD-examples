#include <string.h>
#include <stdbool.h>
#include "LightControllerSpy.h"

static int lastId;
static int lastState;
static int lights[MAX_LIGHTS];

static bool isIdInRange(int id)
{
    return (id >= 0 && id <= 31);
}

void LightController_Create(void)
{
    lastId = LIGHT_ID_UNKNOWN;
    lastState = LIGHT_STATE_UNKNOWN;
    memset(lights, LIGHT_STATE_UNKNOWN, sizeof(lights));
}

void LightController_Destroy(void)
{
}

void LightController_On(int id)
{
    lastId = id;
    lastState = LIGHT_ON;
    if(isIdInRange(id))
        lights[id] = LIGHT_ON;
}

void LightController_Off(int id)
{
    lastId = id;
    lastState = LIGHT_OFF;
    if(isIdInRange(id))
        lights[id] = LIGHT_OFF;
}

int LightControllerSpy_GetLastId(void)
{
    return lastId;
}

int LightControllerSpy_GetLastState(void)
{
    return lastState;
}

int LightControllerSpy_GetLightState(int id)
{
    if(!isIdInRange(id))
        return LIGHT_STATE_UNKNOWN;

    return lights[id];
}
