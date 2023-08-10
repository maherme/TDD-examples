#ifndef RUNTIMEERRORSTUB_H
#define RUNTIMEERRORSTUB_H

#include "RuntimeError.h"

#ifdef __cplusplus
extern "C" {
#endif

void RuntimeErrorStub_Reset(void);
const char* RuntimeErrorStub_GetLastError(void);
int RuntimeErrorStub_GetLastParameter(void);

#ifdef __cplusplus
}
#endif

#endif /* RUNTIMEERRORSTUB_H */
