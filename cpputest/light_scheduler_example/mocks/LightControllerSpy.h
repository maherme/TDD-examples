#ifndef LIGHTCONTROLLERSPY_H
#define LIGHTCONTROLLERSPY_H

#include "LightController.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    LIGHT_ID_UNKNOWN = -1, LIGHT_STATE_UNKNOWN = -1,
    LIGHT_OFF = 0, LIGHT_ON = 1
};

int LightControllerSpy_GetLastId(void);
int LightControllerSpy_GetLastState(void);
int LightControllerSpy_GetLightState(int id);

#ifdef __cplusplus
}
#endif

#endif /* LIGHTCONTROLLERSPY_H */

