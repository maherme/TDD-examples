#include "CppUTest/TestHarness.h"

#include "LedDriver.h"
#include "RuntimeErrorStub.h"

static uint16_t virtualLeds;

TEST_GROUP(LedDriver)
{
    void setup(void)
    {
        LedDriver_Create(&virtualLeds);
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
    LedDriver_TurnOn(1);
    LONGS_EQUAL(1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnMultipleLeds)
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);
    LONGS_EQUAL(0x180, virtualLeds);
}

TEST(LedDriver, AllOn)
{
    LedDriver_TurnAllOn();
    LONGS_EQUAL(0xFFFF, virtualLeds);
}

TEST(LedDriver, TurnOffAnyLed)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    LONGS_EQUAL(0xFF7F, virtualLeds);
}

TEST(LedDriver, LedMemoryIsNotReadable)
{
    virtualLeds = 0xFFFF;
    LedDriver_TurnOn(8);
    LONGS_EQUAL(0x80, virtualLeds);
}

TEST(LedDriver, UpperAndLowerBounds)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);
    LONGS_EQUAL(0x8001, virtualLeds);
}


TEST(LedDriver, OutOfBoundsChangesNothing)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);
    LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);
    LONGS_EQUAL(0xFFFF, virtualLeds);
}

TEST(LedDriver, OutOfBoundsProducesRuntimeError)
{
    LedDriver_TurnOn(-1);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError());
    LONGS_EQUAL(-1, RuntimeErrorStub_GetLastParameter());
}

TEST(LedDriver, OutOfBoundsLedsAreAlwaysOff)
{
    CHECK_EQUAL(true, LedDriver_IsOff(0));
    CHECK_EQUAL(true, LedDriver_IsOff(0));
    CHECK_EQUAL(false, LedDriver_IsOn(0));
    CHECK_EQUAL(false, LedDriver_IsOn(117));
}

IGNORE_TEST(LedDriver, OutOfBoundsToDo)
{
    /* TODO */
}

TEST(LedDriver, IsOn)
{
    CHECK_EQUAL(false, LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    CHECK_EQUAL(true, LedDriver_IsOn(11));
}

TEST(LedDriver, IsOff)
{
    CHECK_EQUAL(true, LedDriver_IsOff(12));
    LedDriver_TurnOn(12);
    CHECK_EQUAL(false, LedDriver_IsOff(12));
}

TEST(LedDriver, TurnOffMultipleLeds)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(9);
    LedDriver_TurnOff(8);
    LONGS_EQUAL((~0x180)&0xFFFF, virtualLeds);
}

TEST(LedDriver, AllOff)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnAllOff();
    LONGS_EQUAL(0, virtualLeds);
}
