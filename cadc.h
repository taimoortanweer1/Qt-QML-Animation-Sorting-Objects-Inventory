
#ifndef CADC_H_INCLUDED
#define CADC_H_INCLUDED

#include "cconfigfile.h"
#include "cboxcar.h"


class CCounts
{
    public:
        long ch1, ch2;
};

class CAdc
{
    public:
        enum ADC_TYPE
        {
            D4601_200,
            D4601_201,
            D4601_202,
            D4601_203,
            D4601_204,
            D4601_205,
            D4601_206,
            D4627,
            D4627v2
        };


        CAdc(ADC_TYPE type, int* hwVer = 0);
        ~CAdc();

        void clearBuffer();			// remove all counts from input buffer
        int updateCounts();			// store next set of counts from the input buffer
        CCounts getCounts() const;	        // get the last retrieved set of counts

                // get ADC DataRate in Hz
        int getDataRate();                      // Added this function - 10-14-09 Ashis Vaidya
        void setDataRate(int newDataRate);      // Added this function - 10-22-09 Ashis Vaidya

    private:
        enum ADC_CHANNELS
        {
            CHANNEL_1,
            CHANNEL_2,
            CHANNEL_BOTH
        };

                int modeReg;
                int conReg;

                void updateModeReg(bool disableInts);

        bool setup();	// init devices

        void writeDataBit(int channel, bool bit);		// send bit to input pin on device
        void writeDataByte(int channel, char datum);	        // send 8 bits to input pin

        void writeModeReg(int channel, long cmd);		// write to mode register
        void writeConfigReg(int channel, long cmd);		// write to config register
        bool selfCal();						// runs internal ADC calibration

        int adcInternalIndex;		// this instance's index in the static object
        CCounts latestCounts;		// latest counts from buffer
};

#endif // CADC_H_INCLUDED
