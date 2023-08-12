#ifndef TIMESERVICE_H
#define TIMESERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int minuteOfDay;
    int dayOfWeek;
}Time;

typedef void (*WakeUpCallback)(void);

void TimeService_Create(void);
void TimeService_GetTime(Time* time);
void TimeService_SetPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb);
void TimeService_CancelPeriodicAlarmInSeconds(int seconds, WakeUpCallback cb);

#ifdef __cplusplus
}
#endif

#endif /* TIMESERVICE_H */

