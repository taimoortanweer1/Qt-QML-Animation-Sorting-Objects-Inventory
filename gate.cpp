#include "gate.h"

Gate::Gate(int gateNum)
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

Gate::Gate()
{
        // nothing to delete for now
}

int Gate::gateNum()
{
        return(gateNumber);
}

double Gate::getLimitLower()
{
        return(limitLower);
}

double Gate::getLimitUpper()
{
        return(limitUpper);
}

GATE_STATE Gate::getState()
{
        return(state);
}


double Gate::getContainedItems()
{
        return(containedItems);
}

double Gate::getContainedWeight()
{
        return(containedWeight);
}


double Gate::getTotalItems()
{
        return(totalItems);
}

double Gate::getTotalWeight()
{
        return(totalWeight);
}


double Gate::getLastWeight()
{
        return(lastWeight);
}

void Gate::setNewContainedItem(double itemMass){
        // add it to the contained stats...
        containedItems++;
        containedWeight += itemMass;
        lastWeight = itemMass;
}

void Gate::setNewPendingItem(double itemMass){
        totalItems++;
        totalWeight += itemMass;

}

bool Gate::setLimitLower(double newLimit)
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

bool Gate::setLimitUpper(double newLimit)
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

void Gate::setState(GATE_STATE newState)
{
        state = newState;
}

void Gate::reset(bool buttonPress)
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
