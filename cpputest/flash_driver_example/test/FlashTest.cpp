#include "CppUTest/TestHarness.h"
#include "Flash.h"
#include "MockIO.h"
#include "m28w160ect.h"

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

        MockIO_Create(10);
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
