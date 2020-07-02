
#ifndef CITEM_H_INCLUDED
#define CITEM_H_INCLUDED

#include "ctimer.h"

enum ITEM_STATE
{
    ITEM_ENTERING_BRIDGE,
    ITEM_ON_BRIDGE,

    ITEM_WEIGHT_TRANSITION,
    ITEM_WEIGHT_STABLE,
    ITEM_WEIGHT_MEASURE,
    ITEM_WEIGHT_DONE,

    ITEM_SORTED
};

class CItem
{
    public:
        CItem();
        ~CItem();

        ITEM_STATE getState();
        void advanceState();

        double calcLength(double beltSpeed);
        double getLength();

        void setMass(double grams);
        double getMass();

        double getT(int);

        void reset();

    private:
        ITEM_STATE state;

        CTimer timer;	// Tracks the time since the leading edge of object broke the pcell.

        double length;	// length of obj in cm.
        double mass;	// unrounded mass in grams

        // scheduling info...
        int gateNum;		// Which gate to put this into.
        double schedTime;	// What time to activate the gate

        double t1;

};


#endif // CITEM_H_INCLUDED
