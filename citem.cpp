
// -----------------------------------------------------------------------------
// File: CItem.cpp
// Date: 2-3-2010
// Author(s): Ashis Vaidya
//
// The C++ Item class
//
// Instance of one Item. Created when an object is detected by the photocell sensor.
//
// Copyright (c) 2009, RYCO Equipment, Inc
// All rights reserved worldwide
// -----------------------------------------------------------------------------

using namespace std;

#include "citem.h"

CItem::CItem()
{
        timer.reset();
        timer.start();

        length = 0;
        mass = 0;

        gateNum = -1;
        schedTime = 0;

        t1 = 0;

        state = ITEM_ENTERING_BRIDGE;
}

CItem::~CItem()
{

}


ITEM_STATE CItem::getState()
{
        return(state);
}

void CItem::advanceState()
{
        if(state != ITEM_SORTED)
        {
                state = (ITEM_STATE)(state + 1);
        }
}

double CItem::calcLength(double beltSpeed)
{
    t1 = timer.elapsed();

    length = t1 * beltSpeed;

    return(length);
}


double CItem::getLength()
{
        return(length);
}

void CItem::setMass(double grams)
{
        mass = grams;
}

double CItem::getMass()
{
        return(mass);
}

double CItem::getT(int time_point)
{
    if(time_point == 0)
    {
        return(timer.elapsed());
    }
    else if(time_point == 1)
    {
        if(length != 0)
        {
            return(timer.elapsed() - t1);
        }
        else
        {
            return(timer.elapsed());
        }
    }
    else
    {
        return(0);
    }

}

void CItem::reset()
{
        timer.reset();
        timer.start();
        length = 0;
        mass = 0;
        gateNum = -1;
        schedTime = 0;
        t1 = 0;
        state = ITEM_ENTERING_BRIDGE;
}
