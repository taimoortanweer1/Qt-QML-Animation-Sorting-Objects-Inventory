// -----------------------------------------------------------------------------
// File: CTimer.cpp
// Date: 20071211
// Author(s): Nicholas Corey
//
// The C++ stopwatch timer class
//
// Allows for high resolution timing of various program events through
// a simple "stopwatch" interface.  Time units are in seconds.
//
// Copyright (c) 2007, RYCO Equipment, Inc
// All rights reserved worldwide
// -----------------------------------------------------------------------------
// History:
//
// 20071211
// Initial C++ Release
// -----------------------------------------------------------------------------

#include "ctimer.h"

using namespace std;

volatile long timerTicks;

// -----------------------------------------------------------------------------
// CTimer Constructor
// Public member of CTimer class
//
// Initializes internal values.
// -----------------------------------------------------------------------------
CTimer::CTimer()
{
        totalTime = 0;
        running = false;
}

// -----------------------------------------------------------------------------
// CTimer Destructor
// Public member of CTimer class
//
// Currently does nothing.
// -----------------------------------------------------------------------------
CTimer::~CTimer()
{
}

void CTimer::wait(double seconds)
{
    CTimer temp;

    temp.reset();
    temp.start();
    while(temp.elapsed() < seconds);
}

// -----------------------------------------------------------------------------
// start()
// Public member of CTimer class
//
// Starts or resumes the timer.
// -----------------------------------------------------------------------------
void CTimer::start()
{
        if(running == false)
        {
                startTime = timerTicks; //clock();
                running = true;
        }
}

// -----------------------------------------------------------------------------
// pause()
// Public member of CTimer class
//
// Pauses the timer.
// -----------------------------------------------------------------------------
void CTimer::pause()
{
        if(running == true)
        {
                //totalTime += ((double)(clock() - startTime) / (double)CLOCKS_SEC);
                totalTime += ((double)(timerTicks - startTime) / (double)CLOCKS_SEC);
                running = false;
        }
}

// -----------------------------------------------------------------------------
// reset()
// Public member of CTimer class
//
// Stops the timer and resets it back to zero.
// -----------------------------------------------------------------------------
void CTimer::reset()
{
        totalTime = 0;
        running = false;
}

// -----------------------------------------------------------------------------
// restart()
// Public member of CTimer class
//
// Resets timer back to zero and continues
// -----------------------------------------------------------------------------
void CTimer::restart()
{
    totalTime = 0;
    startTime = timerTicks;
    running = true;
}

// -----------------------------------------------------------------------------
// elapsed()
// Public member of CTimer class
//
// Returns the current elapsed time in seconds.
// -----------------------------------------------------------------------------
double CTimer::elapsed() const
{
        if(running)
        {
                //return totalTime + ((double)(clock() - startTime) / (double)CLOCKS_SEC);
                return totalTime + ((double)(timerTicks - startTime) / (double)CLOCKS_SEC);
        }else{
                return totalTime;
        }
}

bool CTimer::isRunning()
{
    return running;
}
