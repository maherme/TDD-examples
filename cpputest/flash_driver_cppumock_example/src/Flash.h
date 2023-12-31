#ifndef FLASH_H
#define FLASH_H

#include "IO.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FLASH_SUCCESS = 0,
    FLASH_VPP_ERROR,
    FLASH_PROGRAM_ERROR,
    FLASH_PROTECTED_BLOCK_ERROR,
    FLASH_UNKNOWN_PROGRAM_ERROR,
    FLASH_READ_BACK_ERROR,
    FLASH_TIMEOUT_ERROR
}FlashStatus;

int Flash_Write(ioAddress address, ioData data);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_H */
