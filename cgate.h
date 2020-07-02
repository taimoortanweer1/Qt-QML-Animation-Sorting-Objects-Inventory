#ifndef CGATE_H_INCLUDED
#define CGATE_H_INCLUDED

#include <stdio.h>

#define OUT_OF_RANGE_GATE_NUM 	-1
#define MAX_GATES 		28

enum GATE_STATE
{
    GATE_NORMAL,
    GATE_INUSE,
    GATE_DISABLED
};

class CGate
{
    public:
        CGate(int gateNum);
        ~CGate();

        int gateNum();

        double getLimitLower();
        double getLimitUpper();

        GATE_STATE getState();

        double getTotalItems();
                double getTotalWeight();

                double getContainedItems();
                double getContainedWeight();

                double getLastWeight();

                void setNewPendingItem(double itemMass);
                void setNewContainedItem(double itemMass);

        bool setLimitLower(double newLimit);
        bool setLimitUpper(double newLimit);

        void setState(GATE_STATE newState);

        void reset(bool buttonPress = false);

        int x,y;
        bool top;

        double batchLimit;
        bool full;

        int batchesCount;
        double batchesTotalWeight;

        bool buffer;

    private:
        int gateNumber;

        double limitLower;
        double limitUpper;

                // items in the bin + items pending on the belt
        int totalItems;
        double totalWeight;

                // items in the bin only
        int containedItems;
        double containedWeight;

        double lastWeight;


        GATE_STATE state;

};


#endif // CGATE_H_INCLUDED
