#include "tsofinder.h"

//THIS FILE CONTAINS BASIC INFORMATION ON EVENTS AND RELATED ITEMS
//FOR DETAILED ITEM DEFINITIONS SEE items.cpp

void TSOFinder::init_events()
{
    eventcount = 8;

    events = new event[eventcount];
    i=0;

    events[0].caption = "None";
    events[0].id = 0;
    events[0].itemcount = 0;

    events[1].caption = "Halloween 2013";
    events[1].id = 1;
    events[1].itemcount = 1;

    events[2].caption = "Christmas 2013/14";
    events[2].id = 2;
    events[2].itemcount = 3;

    events[3].caption = "Easter 2014";
    events[3].id = 3;
    events[3].itemcount = 4;

    events[4].caption = "Soccer WM 2014";
    events[4].id = 4;
    events[4].itemcount = 8;

    events[5].caption = "Halloween 2014/15";
    events[5].id = 5;
    events[5].itemcount = 1;

    events[6].caption = "Easter 2015/16";
    events[6].id = 6;
    events[6].itemcount = 1;

    events[7].caption = "Soccer Euro 2016";
    events[7].id = 7;
    events[7].itemcount = 8;
}
