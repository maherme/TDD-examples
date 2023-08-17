#ifndef MOCKIO_H
#define MOCKIO_H

#include "IO.h"

#ifdef __cplusplus
extern "C" {
#endif

void MockIO_Create(int maxExpectations);
void MockIO_Destroy(void);
void MockIO_Expect_Write(ioAddress offset, ioData data);
void MockIO_Expect_ReadThenReturn(ioAddress offset, ioData returnData);
void MockIO_Verify_Complete(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCKIO_H */
