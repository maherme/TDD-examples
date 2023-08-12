#ifndef FAKETIMESERVICE_H
#define FAKETIMESERVICE_H

#include "TimeService.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    TIME_UNKNOWN = -1
};

void FakeTimeService_SetDay(int day);
void FakeTimeService_SetMinute(int minute);
WakeUpCallback FakeTimeService_GetAlarmCallback(void);
int FakeTimeService_GetAlarmPeriod(void);

#ifdef __cplusplus
}
#endif

#endif /* FAKETIMESERVICE_H */
