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

void TimeService_Create(void);
void TimeService_GetTime(Time* time);

#ifdef __cplusplus
}
#endif

#endif /* TIMESERVICE_H */

