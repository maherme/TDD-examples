#ifndef FAKEMICROTIME_H
#define FAKEMICROTIME_H

#include <stdint.h>
#include "MicroTime.h"

#ifdef __cplusplus
extern "C" {
#endif

void FakeMicroTime_Init(uint32_t start, uint32_t incr);

#ifdef __cplusplus
}
#endif

#endif /* FAKEMICROTIME_H */
