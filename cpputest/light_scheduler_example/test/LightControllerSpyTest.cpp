#include "CppUTest/TestHarness.h"
#include "LightControllerSpy.h"

TEST_GROUP(LightControllerSpy)
{
    void setup(void)
    {
        LightController_Create();
    }

    void teardown(void)
    {
        LightController_Destroy();
    }
};

TEST(LightControllerSpy, Create)
{
    LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightControllerSpy, RememberTheLastLightIdControlled)
{
    LightController_On(10);
    LONGS_EQUAL(10, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLastState());
}