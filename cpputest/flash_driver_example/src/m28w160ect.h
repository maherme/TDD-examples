#ifndef M28W160ECT_H
#define M28W160ECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CommandRegister = 0x0,
    StatusRegister = 0x0
}FlashRegisters;

typedef enum
{
    ProgramCommand = 0x40,
    Reset = 0xFF
}FlashCommand;

typedef enum
{
    ReadyBit = 1 << 7
}StatusRegisterBits;

#ifdef __cplusplus
}
#endif

#endif /* M28W160ECT_H */
