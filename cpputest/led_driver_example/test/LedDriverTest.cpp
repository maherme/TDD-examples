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
