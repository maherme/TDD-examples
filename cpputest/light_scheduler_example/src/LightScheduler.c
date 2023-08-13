#include <stdbool.h>
#include "LightScheduler.h"
#include "LightController.h"

enum
{
    UNUSED = -1,
    TURN_ON, TURN_OFF,
    MAX_EVENTS = 128
};

typedef struct
{
    int id;
    int minuteOfDay;
    int day;
    int event;
}ScheduledLightEvent;

static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

static int scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
    int i;

    if(id < 0 || id >= MAX_LIGHTS)
        return LS_ID_OUT_OF_BOUNDS;

    for(i = 0; i < MAX_EVENTS; i++)
    {
        if(scheduledEvents[i].id == UNUSED)
        {
            scheduledEvents[i].minuteOfDay = minuteOfDay;
            scheduledEvents[i].day = day;
            scheduledEvents[i].event = event;
            scheduledEvents[i].id = id;
            return LS_OK;
        }
    }

    return LS_TOO_MANY_EVENTS;
}

static void operateLight(ScheduledLightEvent* lightEvent)
{
    if(lightEvent->event == TURN_ON)
        LightController_On(lightEvent->id);
    else if(lightEvent->event == TURN_OFF)
        LightController_Off(lightEvent->id);
}

static bool DoesLightRespondToday(Time* time, int reactionDay)
{
    int today = time->dayOfWeek;

    if(reactionDay == EVERYDAY)
        return true;
    if(reactionDay == today)
        return true;
    if(reactionDay == WEEKEND && (today == SATURDAY || today == SUNDAY))
        return true;
    if(reactionDay == WEEKDAY && today >= MONDAY && today <= FRIDAY)
        return true;

    return false;
}

static void processEventDueNow(Time* time, ScheduledLightEvent* lightEvent)
{
    int reactionDay = lightEvent->day;

    if(lightEvent->id == UNUSED)
        return;
    if(!DoesLightRespondToday(time, lightEvent->day))
        return;
    if(lightEvent->minuteOfDay != time->minuteOfDay)
        return;

    operateLight(lightEvent);
}

void LightScheduler_Create(void)
{
    int i;

    for(i = 0; i < MAX_EVENTS; i++)
        scheduledEvents[i].id = UNUSED;

    TimeService_SetPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_Destroy(void)
{
    TimeService_CancelPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    return scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
    return scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_WakeUp(void)
{
    int i;
    Time time;
    TimeService_GetTime(&time);

    for(i = 0; i < MAX_EVENTS; i++)
    {
        processEventDueNow(&time, &scheduledEvents[i]);
    }
}

void LightScheduler_ScheduleRemove(int id, Day day, int minute)
{
    int i;

    for(i = 0; i < MAX_EVENTS; i++)
    {
        if(scheduledEvents[i].id == id &&
           scheduledEvents[i].day == day &&
           scheduledEvents[i].minuteOfDay == minute)
        {
            scheduledEvents[i].id = UNUSED;
        }
    }
}
