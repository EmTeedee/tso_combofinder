/*
 *  This file is part of TSO Combofinder.
 *
 *  Copyright 2010-2014 by malkolm <tobid@gmx.ch>
 *  Copyright 2016 by EmTeedee <emteedee@web.de>
 *
 *  TSO Combofinder is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  TSO Combofinder is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with TSO Combofinder.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tsofinder.h"

//THIS FILE CONTAINS BASIC INFORMATION ON EVENTS AND RELATED ITEMS
//FOR DETAILED ITEM DEFINITIONS SEE items.cpp

void TSOFinder::init_events()
{
    eventcount = 8;

    events = new event[eventcount];

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
