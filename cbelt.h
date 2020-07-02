
#ifndef CBELT_H_INCLUDED
#define CBELT_H_INCLUDED


#include "ctimer.h"
#include "cadc.h"
#include "cconfigfile.h"
#include "citem.h"
#include "miscfunc.h"



#define CAL_CYCLES 3
#define BELT_ZERO_TIME 4

#define AUTOZERO_LIMIT 0.002					  // Expressed as a percentage of load cell rating

class CBelt
{
    public:
        CBelt();
        ~CBelt();

        int     getBeltProfile();
        void    applyNewBeltProfile();
        int     getCal(double grams, double unitSize, const char* unitName);

        bool    isReady();

        //void    setBeltLength(double length);
        double  getBeltLength();

        long    getMax();
        long    getMin();
        void    setMinMax(long min, long max);

        void    discardCal();
        void    setDiv(double divGrams);
        bool    processCount(long adc_count, double *rawMass = NULL);
        CItem*  getItem();

        bool    timeoutTick();
        bool    isObjPresent();

        double  beltMass();
        long    getCurrentProfile();

        double  lastObjectMass(bool clear = false);
        double  lastObjectLength();

        double  motThresh();
        double  getMotThreshDiv();
        void    setMotThreshDiv(double newThresh);
        double  getswThreshCount();
        double  beltMassRange();
        double  filteredMass();
        double  recMass();
        bool    pinPresent();

        int     getBeltLinks();
        void    setBeltLinks(int);

        double  getBeltSpeed(void);

        double  calMass;
        bool    b_stable;
        long    calCount;
        bool    profiling;
        bool    autoZero;
        bool    zero_ready;
        double  bridgeLength;
        double  beltSpeed;
        double  debounceTime;

        int  belt_sections;

        CBoxcar *autoZeroBox;
        long belt_min_count;					  // minimum value in the belt profile
        long belt_max_count;					  // maximum value in the belt profile
        long* belt_profile;

    private:
        double belt_length;

        long (*new_belt_profile)[CAL_CYCLES*2];

        long new_belt_min_count;
        long new_belt_max_count;

        double oneDiv;
        double minObjectMass;

        bool processFilter(long adc_count);
        void processAutoZero(long adc_count);

        CTimer item_timer;

        double swThreshDiv;

        long swThreshCount;

        double lastMass;
        double lastLength;

        CBoxcar *m_box1;
        CBoxcar *m_box2;

        long b_filtered;

        long calCountPrevious;
        bool get_cal;

        CAdc *adc_ptr;


};
#endif											  // CBELT_H_INCLUDED
