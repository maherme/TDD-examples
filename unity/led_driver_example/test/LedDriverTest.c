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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(LedsOffAfterCreate);
    RUN_TEST(TurnOnLedOne);
    RUN_TEST(TurnOffLedOne);
    RUN_TEST(TurnOnMultipleLeds);
    return UNITY_END();
}
