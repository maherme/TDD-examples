#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    MAX_LIGHTS = 32
};

void LightController_Create(void);
void LightController_Destroy(void);
void LightController_On(int id);
void LightController_Off(int id);

#ifdef __cplusplus
}
#endif

#endif /* LIGHTCONTROLLER_H */
