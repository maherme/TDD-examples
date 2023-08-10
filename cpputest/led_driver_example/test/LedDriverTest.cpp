#include "CppUTest/TestHarness.h"

#include "LedDriver.h"

TEST_GROUP(LedDriver)
{
    void setup(void)
    {
    }

    void teardown(void)
    {
    }
};

TEST(LedDriver, LedsOffAfterCreate)
{
    uint16_t virtualLeds = 0xFFFF;
    LedDriver_Create(&virtualLeds);
    LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnLedOne)
{
    uint16_t virtualLeds;
    LedDriver_Create(&virtualLeds);
    LedDriver_TurnOn(1);
    LONGS_EQUAL(1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne)
{
    uint16_t virtualLeds;
    LedDriver_Create(&virtualLeds);
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    LONGS_EQUAL(0, virtualLeds);
}
