#ifndef FAKERANDOMMINUTE_H
#define FAKERANDOMMINUTE_H

#include "RandomMinute.h"

#ifdef __cplusplus
extern "C" {
#endif

int FakeRandomMinute_Get(void);
void FakeRandomMinute_SetFirstAndIncrement(int s, int i);

#ifdef __cplusplus
}
#endif

#endif /* FAKERANDOMMINUTE_H */
