#include "CppUTest/TestHarness.h"
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
#include "FakeRandomMinute.h"

TEST_GROUP(LightSchedulerRandomize)
{
    void setup(void)
    {
        LightController_Create();
        LightScheduler_Create();
        UT_PTR_SET(RandomMinute_Get, FakeRandomMinute_Get);
    }

    void teardown(void)
    {
        LightScheduler_Destroy();
        LightController_Destroy();
    }

    void setTimeTo(int day, int minuteOfDay)
    {
        FakeTimeService_SetDay(day);
        FakeTimeService_SetMinute(minuteOfDay);
    }

    void checkLightState(int id, int level)
    {
        LONGS_EQUAL(level, LightControllerSpy_GetLightState(id));
    }
};

TEST(LightSchedulerRandomize, TurnsOnEarly)
{
    FakeRandomMinute_SetFirstAndIncrement(-10, 5);
    LightScheduler_ScheduleTurnOn(4, EVERYDAY, 600);
    LightScheduler_Randomize(4, EVERYDAY, 600);
    setTimeTo(MONDAY, 600-10);
    LightScheduler_WakeUp();
    checkLightState(4, LIGHT_ON);
}
