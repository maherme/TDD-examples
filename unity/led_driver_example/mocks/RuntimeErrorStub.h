#ifndef RUNTIMEERRORSTUB_H
#define RUNTIMEERRORSTUB_H

#include "RuntimeError.h"

void RuntimeErrorStub_Reset(void);
const char* RuntimeErrorStub_GetLastError(void);
int RuntimeErrorStub_GetLastParameter(void);

#endif /* RUNTIMEERRORSTUB_H */
