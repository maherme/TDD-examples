#include <stdbool.h>
#include "LightScheduler.h"
#include "LightController.h"

enum
{
    UNUSED = -1,
    TURN_ON, TURN_OFF
};

typedef struct
{
    int id;
    int minuteOfDay;
    int day;
    int event;
}ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

static void scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
    scheduledEvent.minuteOfDay = minuteOfDay;
    scheduledEvent.day = day;
    scheduledEvent.event = event;
    scheduledEvent.id = id;
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
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{
}

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
    scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
    scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_WakeUp(void)
{
    Time time;
    TimeService_GetTime(&time);

    processEventDueNow(&time, &scheduledEvent);
}
