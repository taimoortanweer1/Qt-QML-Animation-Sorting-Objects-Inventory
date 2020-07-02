
#include "cgate.h"
CGate::CGate(int gateNum)
{
        gateNumber = gateNum;

        limitLower = 0;
        limitUpper = 0;

        totalItems = 0;
        totalWeight = 0;

        containedItems = 0;
        containedWeight = 0;

        lastWeight = 0;
        batchLimit = 0;

        buffer = false;

        if(gateNum == -1)
        {
                state = GATE_NORMAL;
        }else{
                state = GATE_DISABLED;
        }

        x = 0;
        y = 0;

        full = false;

        batchesCount = 0;
        batchesTotalWeight = 0;
}

CGate::~CGate()
{

}

int CGate::gateNum()
{
        return(gateNumber);
}

double CGate::getLimitLower()
{
        return(limitLower);
}

double CGate::getLimitUpper()
{
        return(limitUpper);
}

GATE_STATE CGate::getState()
{
        return(state);
}


double CGate::getContainedItems()
{
        return(containedItems);
}

double CGate::getContainedWeight()
{
        return(containedWeight);
}


double CGate::getTotalItems()
{
        return(totalItems);
}

double CGate::getTotalWeight()
{
        return(totalWeight);
}


double CGate::getLastWeight()
{
        return(lastWeight);
}

void CGate::setNewContainedItem(double itemMass){
        // add it to the contained stats...
        containedItems++;
        containedWeight += itemMass;
        lastWeight = itemMass;
}

void CGate::setNewPendingItem(double itemMass){
        totalItems++;
        totalWeight += itemMass;

}

bool CGate::setLimitLower(double newLimit)
{
        limitLower = newLimit;
        return(true);

        // don't do this check...
        /*if( (newLimit >= 0) && (newLimit < limitUpper) )
        {
                limitLower = newLimit;
                return(true);
        }
        return(false);*/
}

bool CGate::setLimitUpper(double newLimit)
{
        limitUpper = newLimit;
        return(true);

        // don't do this check...
        /*if( newLimit > limitLower )
        {
                limitUpper = newLimit;
                return(true);
        }
        return(false);*/
}

void CGate::setState(GATE_STATE newState)
{
        state = newState;
}

void CGate::reset(bool buttonPress)
{
        if(buttonPress)
        {
                batchesCount++;
                batchesTotalWeight += containedWeight;
        }

        totalItems = 0;
        totalWeight = 0;
        lastWeight = 0;

        buffer = false;
        state = GATE_NORMAL;

        containedItems = 0;
        containedWeight = 0;

        full = false;

}
