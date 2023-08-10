#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void LedDriver_Create(uint16_t* address);

#ifdef __cplusplus
}
#endif

#endif /* LEDDRIVER_H */
