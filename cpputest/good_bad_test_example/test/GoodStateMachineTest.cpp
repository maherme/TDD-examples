#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupport_c.h"

#include "StateMachine.h"

/**
 * @brief Mock funcion for input.
 */
int input(void)
{
    mock_c()->actualCall("input");

    return mock_c()->returnValue().value.intValue;
}

/**
 * @brief Mock funcion for output.
 */
void output(int i)
{
    mock_c()->actualCall("output")
            ->withIntParameters("i", i);
}

TEST_GROUP(StateMachine)
{
    void setup(void)
    {
        state_machine_init();
    }

    void teardown(void)
    {
        mock().checkExpectations();
        mock().clear();
    }

    void SetInputExpectOutput(int input, int output)
    {
        mock().expectOneCall("input")
              .andReturnValue(input);
        mock().expectOneCall("output")
              .withParameter("i", output);
    }

    void SetInvalidInput(void)
    {
        mock().expectOneCall("input")
              .andReturnValue(0);
        mock().expectNoCall("output");
    }

    void NoInput(void)
    {
        mock().expectNoCall("input");
        mock().expectNoCall("output");
    }
};

TEST(StateMachine, SetOutputTo1)
{
    SetInputExpectOutput(1,1);

    state_machine_process();
}

TEST(StateMachine, NoSetAnyOutput)
{
    SetInvalidInput();

    state_machine_process();
}

TEST(StateMachine, SetOutputTo2)
{
    SetInputExpectOutput(1,1);
    SetInputExpectOutput(2,2);

    for(int i = 0; i < 2; i++)
        state_machine_process();
}

TEST(StateMachine, KeepOutputTo2)
{
    SetInputExpectOutput(1,1);
    SetInputExpectOutput(2,2);
    SetInvalidInput();

    for(int i = 0; i < 3; i++)
        state_machine_process();
}

TEST(StateMachine, SetOutputTo3)
{
    SetInputExpectOutput(1,1);
    SetInputExpectOutput(2,2);
    SetInputExpectOutput(3,3);

    for(int i = 0; i < 3; i++)
        state_machine_process();
}

TEST(StateMachine, KeepOutputTo3)
{
    SetInputExpectOutput(1,1);
    SetInputExpectOutput(2,2);
    SetInputExpectOutput(3,3);
    NoInput();

    for(int i = 0; i < 4; i++)
        state_machine_process();
}

