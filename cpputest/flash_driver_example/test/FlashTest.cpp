#include "CppUTest/TestHarness.h"
#include "Flash.h"
#include "MockIO.h"
#include "m28w160ect.h"
#include "FakeMicroTime.h"

TEST_GROUP(Flash)
{
    ioAddress address;
    ioData data;
    int result;

    void setup(void)
    {
        address = 0x1000;
        data = 0xBEEF;
        result = -1;

        MockIO_Create(12);
    }

    void teardown(void)
    {
        MockIO_Verify_Complete();
        MockIO_Destroy();
    }
};

TEST(Flash, WriteSucceeds_ReadyImmediately)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteSucceeds_NotImmediatelyReady)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, 0);
    MockIO_Expect_ReadThenReturn(StatusRegister, 0);
    MockIO_Expect_ReadThenReturn(StatusRegister, 0);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteSucceeds_IgnoresOtherBitsUntilReady)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteFails_VppError)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | VppErrorBit);
    MockIO_Expect_Write(CommandRegister, Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_VPP_ERROR, result);
}

TEST(Flash, WriteFails_ProgramError)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | ProgramErrorBit);
    MockIO_Expect_Write(CommandRegister, Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_PROGRAM_ERROR, result);
}

TEST(Flash, WriteFails_ProtectedBlockError)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit | BlockProtectionErrorBit);
    MockIO_Expect_Write(CommandRegister, Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, result);
}

TEST(Flash, WriteFails_FlashUnknownProgramError)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(
        StatusRegister,
        ReadyBit | EraseSuspendBit | EraseErrorBit | ProgramSuspendBit | ReservedBit
    );
    MockIO_Expect_Write(CommandRegister, Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_UNKNOWN_PROGRAM_ERROR, result);
}

TEST(Flash, WriteFails_FlashReadBackError)
{
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    MockIO_Expect_ReadThenReturn(StatusRegister, ReadyBit);
    MockIO_Expect_ReadThenReturn(address, data-1);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_READ_BACK_ERROR, result);
}

TEST(Flash, WriteFails_Timeout)
{
    FakeMicroTime_Init(0, 500);
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    for(int i = 0; i < 10; i++)
        MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}

TEST(Flash, WriteFails_TimeoutAtEndOfTime)
{
    FakeMicroTime_Init(0xFFFFFFFF, 500);
    MockIO_Expect_Write(CommandRegister, ProgramCommand);
    MockIO_Expect_Write(address, data);
    for(int i = 0; i < 10; i++)
        MockIO_Expect_ReadThenReturn(StatusRegister, ~ReadyBit);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}

