#include "unity.h"
#include "LedDriver.h"

static uint16_t virtualLeds;

void setUp(void)
{
    LedDriver_Create(&virtualLeds);
}

void tearDown(void)
{
}

void LedsOffAfterCreate(void)
{
    uint16_t virtualLeds = 0xFFFF;
    LedDriver_Create(&virtualLeds);
    TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}

void TurnOnLedOne(void)
{
    LedDriver_TurnOn(1);
    TEST_ASSERT_EQUAL_HEX16(1, virtualLeds);
}

void TurnOffLedOne(void)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}

void TurnOnMultipleLeds(void)
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);
    TEST_ASSERT_EQUAL_HEX16(0x180, virtualLeds);
}

void TurnOffAnyLed(void)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    TEST_ASSERT_EQUAL_HEX16(0xff7f, virtualLeds);
}

void AllOn(void)
{
    LedDriver_TurnAllOn();
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, virtualLeds);
}

void LedMemoryIsNotReadable(void)
{
    virtualLeds = 0xFFFF;
    LedDriver_TurnOn(8);
    TEST_ASSERT_EQUAL_HEX16(0x80, virtualLeds);
}

void UpperAndLowerBounds(void)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);
    TEST_ASSERT_EQUAL_HEX16(0x8001, virtualLeds);
}

void OutOfBoundsTurnOnDoesNoHarm(void)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);
    TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}

void OutOfBoundsTurnOffDoesNoHarm(void)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, virtualLeds);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(LedsOffAfterCreate);
    RUN_TEST(TurnOnLedOne);
    RUN_TEST(TurnOffLedOne);
    RUN_TEST(TurnOnMultipleLeds);
    RUN_TEST(TurnOffAnyLed);
    RUN_TEST(AllOn);
    RUN_TEST(LedMemoryIsNotReadable);
    RUN_TEST(UpperAndLowerBounds);
    RUN_TEST(OutOfBoundsTurnOnDoesNoHarm);
    RUN_TEST(OutOfBoundsTurnOffDoesNoHarm);
    return UNITY_END();
}
