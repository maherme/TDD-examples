#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupport_c.h"
#include "Flash.h"
#include "m28w160ect.h"
#include "FakeMicroTime.h"

/**
 * @brief Mock funcion for IO_Write.
 */
void IO_Write(ioAddress addr, ioData value)
{
    mock_c()->actualCall("IO_Write")
            ->withIntParameters("addr", addr)
            ->withIntParameters("value", value);
}

/**
 * @brief Mock funcion for IO_Read.
 */
ioData IO_Read(ioAddress addr)
{
    mock_c()->actualCall("IO_Read")
            ->withIntParameters("addr", addr);

    return mock_c()->returnValue().value.intValue;
}

TEST_GROUP(Flash)
{
    ioAddress address;
    ioData data;
    int result;

    void setup(void)
    {
        address = 0x1000;
        data = 0xBEEF;
    }

    void teardown(void)
    {
        mock().checkExpectations();
        mock().clear();
    }

    void expectCommand(ioData command)
    {
        mock().expectOneCall("IO_Write")
              .withParameter("addr", CommandRegister)
              .withParameter("value", command);
    }

    void expectWriteData(void)
    {
        mock().expectOneCall("IO_Write")
              .withParameter("addr", (int)address)
              .withParameter("value", data);
    }

    void simulateDeviceStatus(ioData status)
    {
        mock().expectOneCall("IO_Read")
              .withParameter("addr", StatusRegister)
              .andReturnValue((int)status);
    }

    void simulateDeviceStatusWithRepeat(ioData status, int repeatCount)
    {
        mock().expectNCalls(repeatCount, "IO_Read")
              .withParameter("addr", StatusRegister)
              .andReturnValue((int)status);
    }

    void simulateReadback(ioData data)
    {
        mock().expectOneCall("IO_Read")
              .withParameter("addr", (int)address)
              .andReturnValue((int)data);
    }
};

TEST(Flash, WriteSucceeds_ReadyImmediately)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit);
    simulateReadback(data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteSucceeds_NotImmediatelyReady)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatusWithRepeat(0, 3);
    simulateDeviceStatus(ReadyBit);
    simulateReadback(data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteSucceeds_IgnoresOtherBitsUntilReady)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(~ReadyBit);
    simulateDeviceStatus(ReadyBit);
    simulateReadback(data);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_SUCCESS, result);
}

TEST(Flash, WriteFails_VppError)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit | VppErrorBit);
    expectCommand(Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_VPP_ERROR, result);
}

TEST(Flash, WriteFails_ProgramError)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit | ProgramErrorBit);
    expectCommand(Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_PROGRAM_ERROR, result);
}

TEST(Flash, WriteFails_ProtectedBlockError)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit | BlockProtectionErrorBit);
    expectCommand(Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_PROTECTED_BLOCK_ERROR, result);
}

TEST(Flash, WriteFails_FlashUnknownProgramError)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit | EraseSuspendBit | EraseErrorBit | ProgramSuspendBit | ReservedBit);
    expectCommand(Reset);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_UNKNOWN_PROGRAM_ERROR, result);
}

TEST(Flash, WriteFails_FlashReadBackError)
{
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatus(ReadyBit);
    simulateReadback(data-1);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_READ_BACK_ERROR, result);
}

TEST(Flash, WriteFails_Timeout)
{
    FakeMicroTime_Init(0, 500);
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatusWithRepeat(~ReadyBit, 10);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}

TEST(Flash, WriteFails_TimeoutAtEndOfTime)
{
    FakeMicroTime_Init(0xFFFFFFFF, 500);
    expectCommand(ProgramCommand);
    expectWriteData();
    simulateDeviceStatusWithRepeat(~ReadyBit, 10);

    result = Flash_Write(address, data);
    LONGS_EQUAL(FLASH_TIMEOUT_ERROR, result);
}

