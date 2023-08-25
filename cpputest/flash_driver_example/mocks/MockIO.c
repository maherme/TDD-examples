#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "CppUTest/TestHarness_c.h"
#include "MockIO.h"

enum
{
    FLASH_READ, FLASH_WRITE,
    NoExpectedValue = -1
};

typedef struct Expectation
{
    int kind;
    ioAddress addr;
    ioData value;
}Expectation;

static Expectation* expectations = NULL;
static int setExpectationCount;
static int getExpectationCount;
static int maxExpectationCount;
static int failureAlreadyReported = 0;
static Expectation expected;
static Expectation actual;

static const char* report_MockIO_not_initialized = "MockIO not initialized, call MockIO_Create()";
static const char* report_too_many_write_expectations = "MockIO_Expect_IO_Write: Too many expectations";
static const char* report_too_many_read_expectations = "MockIO_Expect_IO_Read: Too many expectations";
static const char* report_write_but_out_of_expectations = "IO_Write(0x%x, 0x%x)";
static const char* report_write_does_not_match = "Expected IO_Write(0x%x, 0x%x)\n"
                                                 "\t        But was IO_Write(0x%x, 0x%x)";
static const char* report_read_but_out_of_expectations = "IO_Read(0x%x)";
static const char* report_no_more_expectations = "R/W %d: No more expectations but was ";
static const char* report_expect_read_was_write = "Expected IO_Read(0x%x) would return 0x%x)\n"
                                                  "\t        But was IO_Write(0x%x, 0x%x)";
static const char* report_expect_write_was_read = "Expected IO_Write(0x%x, 0x%x)\n"
                                                  "\t        But was IO_Read(0x%x)";
static const char* report_expectation_number = "R/W %d ";
static const char* report_read_wrong_address = "Expected IO_Read(0x%x) returns 0x%x;\n"
                                               "\t        But was IO_Read(0x%x)";

static void fail(const char* message)
{
    failureAlreadyReported = 1;
    FAIL_TEXT_C(message);
}

static void failWhenNotInitialized(void)
{
    if(expectations == NULL)
        fail(report_MockIO_not_initialized);
}

static void failWhenNoRoomForExpectations(const char* message)
{
    failWhenNotInitialized();
    if(setExpectationCount >= maxExpectationCount)
        fail(message);
}

static void recordExpectation(int kind, ioAddress addr, ioData value)
{
    expectations[setExpectationCount].kind = kind;
    expectations[setExpectationCount].addr = addr;
    expectations[setExpectationCount].value = value;
    setExpectationCount++;
}

static void failWhenNotAllExpectationsUsed(void)
{
    char format[] = "Expected %d reads/writes but got %d";
    char message[sizeof format + 5 + 5];

    if(getExpectationCount == setExpectationCount)
        return;

    snprintf(message, sizeof message, format, setExpectationCount, getExpectationCount);
    fail(message);
}

static void setExpectedAndActual(ioAddress addr, ioData data)
{
    expected.addr = expectations[getExpectationCount].addr;
    expected.value = expectations[getExpectationCount].value;
    actual.addr = addr;
    actual.value = data;
}

static void failWhenNotUnusedExpectations(const char* format)
{
    char message[100];
    int size = sizeof message - 1;

    if(getExpectationCount >= setExpectationCount)
    {
        int offset = snprintf(message, size, report_no_more_expectations, getExpectationCount + 1);
        snprintf(message + offset, size - offset, format, actual.addr, actual.value);
        fail(message);
    }
}

static bool expectationIsNot(int kind)
{
    return kind != expectations[getExpectationCount].kind;
}

static void failExpectation(const char* expectationFailMessage)
{
    char message[100];
    int size = sizeof message - 1;
    int offset = snprintf(message, size, report_expectation_number, getExpectationCount + 1);

    snprintf(message + offset, size - offset, expectationFailMessage,
             expected.addr, expected.value, actual.addr, actual.value);
    fail(message);
}

static void failWhen(int condition, const char* expectationFailMessage)
{
    if(condition)
        failExpectation(expectationFailMessage);
}

static bool expectedAddressIsNot(ioAddress addr)
{
    return expected.addr != addr;
}

static bool expectedDataIsNot(ioData data)
{
    return expected.value != data;
}

void IO_Write(ioAddress addr, ioData data)
{
    setExpectedAndActual(addr, data);
    failWhenNotInitialized();
    failWhenNotUnusedExpectations(report_write_but_out_of_expectations);
    failWhen(expectationIsNot(FLASH_WRITE), report_expect_read_was_write);
    failWhen(expectedAddressIsNot(addr), report_write_does_not_match);
    failWhen(expectedDataIsNot(data), report_write_does_not_match);
    getExpectationCount++;
}

ioData IO_Read(ioAddress addr)
{
    setExpectedAndActual(addr, NoExpectedValue);
    failWhenNotInitialized();
    failWhenNotUnusedExpectations(report_read_but_out_of_expectations);
    failWhen(expectationIsNot(FLASH_READ), report_expect_write_was_read);
    failWhen(expectedAddressIsNot(addr), report_read_wrong_address);

    return expectations[getExpectationCount++].value;
}

void MockIO_Create(int maxExpectations)
{
    expectations = (Expectation*)calloc(maxExpectations, sizeof(Expectation));
    setExpectationCount = 0;
    getExpectationCount = 0;
    maxExpectationCount = maxExpectations;
    failureAlreadyReported = 0;
}

void MockIO_Destroy(void)
{
    if(expectations)
        free(expectations);
    expectations = NULL;
}

void MockIO_Expect_Write(ioAddress offset, ioData data)
{
    failWhenNoRoomForExpectations(report_too_many_write_expectations);
    recordExpectation(FLASH_WRITE, offset, data);
}

void MockIO_Expect_ReadThenReturn(ioAddress offset, ioData returnData)
{
    failWhenNoRoomForExpectations(report_too_many_read_expectations);
    recordExpectation(FLASH_READ, offset, returnData);
}

void MockIO_Verify_Complete(void)
{
    if(failureAlreadyReported)
        return;
    failWhenNotAllExpectationsUsed();
}

