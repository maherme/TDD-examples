#ifndef FLASH_H
#define FLASH_H

#include "IO.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FLASH_SUCCESS = 0
}FlashStatus;

int Flash_Write(ioAddress address, ioData data);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_H */
