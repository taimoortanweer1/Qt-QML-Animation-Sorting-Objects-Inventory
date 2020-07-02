#include "CAdc.h"
#include "CTimer.h"
#include "miscFunc.h"
#include <iostream>

// Output pins (with reference to ADC)
#define AD7190_DOUT0			        0x01	// channel1 data out
#define AD7190_DOUT1			        0x02	// channel2 data out

// Input pins (with reference to ADC)
#define AD7190_CS0				0x01	// channel1 chip select
#define AD7190_SCLK0			        0x02	// channel1 clock
#define AD7190_DIN0				0x04	// channel1 data in
#define AD7190_SYNC				0x08	// common sync
#define AD7190_CS1				0x10	// channel2 select
#define AD7190_SCLK1			        0x20	// channel2 clock
#define AD7190_DIN1				0x40	// channel2 data in

// ----------------------------------
// IRQ SETTING

//#define AD7190_IRQ3
//#define AD7190_IRQ4
#define AD7190_IRQ5
//#define AD7190_IRQ6
//#define AD7190_IRQ7

// ----------------------------------
// This automatically sets certain
// interrupt constants

#ifdef AD7190_IRQ3
#define AD7190_INTERRUPT 0xB
#define AD7190_MASKBIT 0x08
#endif

#ifdef AD7190_IRQ4
#define AD7190_INTERRUPT 0xC
#define AD7190_MASKBIT 0x10
#endif

#ifdef AD7190_IRQ5
#define AD7190_INTERRUPT 0xD
#define AD7190_MASKBIT 0x20
#endif

#ifdef AD7190_IRQ6
#define AD7190_INTERRUPT 0xE
#define AD7190_MASKBIT 0x40
#endif

#ifdef AD7190_IRQ7
#define AD7190_INTERRUPT 0xF
#define AD7190_MASKBIT 0x80
#endif
// ----------------------------------

#define AD7190_MCLK			4915200			// master clock frequency
#define AD7190_FMOD			(AD7190_MCLK/16)        // internal modulator frequency
#define AD7190_SYNCFILT_N		4			// sinc filter order (settling time)

#define AD7190_MODE_EXTCLK		0x040000	// MODE register: external clock
#define AD7190_MODE_INTCLK		0x080000	// MODE register: internal clock
#define AD7190_MODE_INTCLKOUT	        0x0C0000	// MODE register: internal clock w/ freq out

#define AD7190_MODE_SINC3               0x8000
#define AD7190_MODE_SINC4               0x0000

// MODE register: ADC count update frequency (assuming chopping is off)
#define AD7190_MODE_NOCHOP_8HZ		(AD7190_FMOD / 64 / 8)
#define AD7190_MODE_NOCHOP_12HZ		(AD7190_FMOD / 64 / 12)
#define AD7190_MODE_NOCHOP_20HZ		(AD7190_FMOD / 64 / 20)
#define AD7190_MODE_NOCHOP_16HZ		(AD7190_FMOD / 64 / 16)
#define AD7190_MODE_NOCHOP_100HZ	(AD7190_FMOD / 64 / 100)
#define AD7190_MODE_NOCHOP_200HZ	(AD7190_FMOD / 64 / 200)
#define AD7190_MODE_NOCHOP_400HZ	(AD7190_FMOD / 64 / 400)
#define AD7190_MODE_NOCHOP_800HZ	(AD7190_FMOD / 64 / 800)
#define AD7190_MODE_NOCHOP_2400HZ	(AD7190_FMOD / 64 / 2400)
#define AD7190_MODE_NOCHOP_4800HZ	(AD7190_FMOD / 64 / 4800)

// MODE register: ADC count update frequency (assuming chopping is on)
#define AD7190_MODE_CHOP_2HZ		(AD7190_FMOD / 64 / 2 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_4HZ		(AD7190_FMOD / 64 / 4 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_25HZ		(AD7190_FMOD / 64 / 25 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_50HZ		(AD7190_FMOD / 64 / 50 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_100HZ		(AD7190_FMOD / 64 / 100 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_200HZ		(AD7190_FMOD / 64 / 200 / AD7190_SYNCFILT_N)
#define AD7190_MODE_CHOP_1200HZ		(AD7190_FMOD / 64 / 1200 / AD7190_SYNCFILT_N)

// MODE register:
// Do an internal fullscale calibration when setting gain higher than 1.
// Internal zero-scale calibration not needed with chopping on.
#define AD7190_MODE_DOZEROCAL	(0x8003F0 | AD7190_MODE_INTCLK)
#define AD7190_MODE_DOFULLCAL	(0xA003F0 | AD7190_MODE_INTCLK)

// CONFIG register:
#define AD7190_CONFIG_CHOP		0x800000	// chopping on
#define AD7190_CONFIG_AIN1AIN2	0x000100	// get counts from analog pins
#define AD7190_CONFIG_TEMPSENSE	0x000400	// get counts from temperature sensor
#define AD7190_CONFIG_BUFFERED	0x000010	// buffered inputs on
#define AD7190_CONFIG_GAIN128	0x000007	// set pre-amp gain to 128x (highest value)

#define SELF_CAL_TIMEOUT        2

// used for preventing memory from being sent to a page file
#define LOCK_VARIABLE(x)   _go32_dpmi_lock_data((void *)&x,(long)sizeof(x))
#define LOCK_FUNCTION(x)   _go32_dpmi_lock_code((void *)x,(long)x##_end - (long)x)
#define END_OF_FUNCTION(x) static void x##_end() { }


using namespace std;

// stores the old ISR pointer

// this class is used to provide access to data
// needed by the interrupt service routine
class CAdcInternal
{
    public:
        // pointer to the main ADC object
        CAdc* mainPointer;

        // AD7190 address
        int AD7190;

        // Output pin state
        char controlByte;

        // stores counts until they're read
        CCounts adcBuffer[32];

        // number of times we've interrupted since the last time the buffer was read
        int adcCounter;
};

// this global static array stores pointers to all instances of adcInternal
CAdcInternal 	**adcInternal = NULL;
int 		adcInternalSize = 0;

int adc_ticks = 0;

// ISR function prototypes
static CCounts isrGetCounts(CAdcInternal *adcInternal);
static bool isrIsRdy(CAdcInternal *adcInternal, int channel);
static void isrWriteInterfaceBits(CAdcInternal *adcInternal, char mask, bool onflag);
static void isrSync(CAdcInternal *adcInternal);

// -----------------------------------------------------------------------------
// adcHandler()
// Global static function
//
// Interrupt service routine
//
// Retrieves counts if available from all instances of CAdc
// -----------------------------------------------------------------------------
static void adcHandler(void)
{
    int pic, byte;
    CCounts counts;

    // disable interrupt
    pic = 0x21;

    asm(    "xor %%ax, %%ax         \n"		// read PIC
            "mov %0, %%dx           \n"
            "in %%dx, %%al          \n"
            "mov %%ax, %1           \n"
            :
            : "m" (pic), "m" (byte)
            : "edx", "eax");

    byte = byte | AD7190_MASKBIT;			// set mask bit

    asm(    "movb %0, %%al          \n"		// write to PIC
            "movw %1, %%dx          \n"
            "outb %%al, %%dx        \n"
            :
            : "m" (byte), "m" (pic)
            : "edx", "eax");

    for(int i=0 ; i<adcInternalSize ; i++)
    {
        counts = isrGetCounts(adcInternal[i]);
        if(counts.ch1 >= 0)
        {
                        adc_ticks++;

                        if(adcInternal[i]->adcCounter < 32)
                        {
                                adcInternal[i]->adcBuffer[adcInternal[i]->adcCounter] = counts;
                                adcInternal[i]->adcCounter++;
                        }
        }
    }

    // enable interrupt
    pic = 0x21;

    asm(    "xor %%ax, %%ax         \n"		// read PIC
            "mov %0, %%dx           \n"
            "in %%dx, %%al          \n"
            "mov %%ax, %1           \n"
            :
            : "m" (pic), "m" (byte)
            : "edx", "eax");

    byte = byte & (~AD7190_MASKBIT);		// reset mask bit

    asm(    "movb %0, %%al          \n"		// write to PIC
            "movw %1, %%dx          \n"
            "outb %%al, %%dx        \n"
            :
            : "m" (byte), "m" (pic)
            : "edx", "eax");

    // send EOI
    pic = 0x20;
    byte = 0x20;

    asm(    "movb %0, %%al          \n"
            "movw %1, %%dx          \n"
            "outb %%al, %%dx        \n"
            :
            : "m" (byte), "m" (pic)
            : "edx", "eax");
}
END_OF_FUNCTION(adcHandler)

// -----------------------------------------------------------------------------
// isrGetCounts()
// Global static function
//
// Retrieves counts from both ADC devices
// -----------------------------------------------------------------------------
static CCounts isrGetCounts(CAdcInternal *adcInternal)
{
    int din, sclk, b;
    long i;

    CCounts counts;

    // Initializes pin values to access both devices at once
    din = AD7190_DIN0 + AD7190_DIN1;
    sclk = AD7190_SCLK0 + AD7190_SCLK1;

    // check if both devices have data
    if(isrIsRdy(adcInternal, 3))
    {
        // Set the ADC data in pin low
        isrWriteInterfaceBits(adcInternal, din, 0);

        counts.ch1 = 0;
        counts.ch2 = 0;
        b = 0;

        // Set to read from the MSB first
        i = 0x800000;

        // Until we've read the last bit
        while(i != 0)
        {
            // Data clock active
            isrWriteInterfaceBits(adcInternal, sclk, 0);

            // Retrieve the whole input byte
            asm(    "xor %%ax, %%ax         \n"
                    "mov %0, %%dx           \n"
                    "in %%dx, %%al          \n"
                    "mov %%ax, %1           \n"
                    :
                    : "m" (adcInternal->AD7190), "m" (b)
                    : "edx", "eax");

            // Data clock inactive
            isrWriteInterfaceBits(adcInternal, sclk, 1);

            // Update the bit in each count value if needed
            if((b & AD7190_DOUT0) != 0) counts.ch1 = counts.ch1 + i;
            if((b & AD7190_DOUT1) != 0) counts.ch2 = counts.ch2 + i;

            // Next bit
            i = i >> 1;
        }
    }else
    {
        // indicates data is not ready and shouldn't be added to buffer
        counts.ch1 = -1;
    }

    return counts;
}
END_OF_FUNCTION(isrGetCounts)

// -----------------------------------------------------------------------------
// isrIsRdy()
// Global static function
//
// Returns whether one or both devices is ready for data transfer
//
// int channel:
// 1 = ADC1
// 2 = ADC2
// 3 = ADC1 & ADC2
// -----------------------------------------------------------------------------
static bool isrIsRdy(CAdcInternal *adcInternal, int channel)
{
    int rdy, b;

    rdy = 0;

    // Set pin value for channel 1 if selected
    if((channel & 1) != 0) rdy = rdy + AD7190_DOUT0;

    // Set pin value for channel 2 if selected
    if((channel & 2) != 0) rdy = rdy + AD7190_DOUT1;

    // Retrieve data from output pins
    asm(    "nop                    \n"
            "xor %%ax, %%ax         \n"
            "mov %0, %%dx           \n"
            "in %%dx, %%al          \n"
            "mov %%ax, %1           \n"
            :
            : "m" (adcInternal->AD7190), "m" (b)
            : "edx", "eax");

    if((rdy & b) == 0)
    {
        return true;
    }

    return false;
}
END_OF_FUNCTION(isrIsRdy)

// -----------------------------------------------------------------------------
// isrWriteInterfaceBits()
// Global static function
//
// Updates specific input pins on the ADC devices.
// -----------------------------------------------------------------------------
static void isrWriteInterfaceBits(CAdcInternal *adcInternal, char mask, bool onflag)
{
    // If bits should be turned on
    if(onflag == true)
    {
        // Set appropriate bits in buffer
        adcInternal->controlByte = adcInternal->controlByte | mask;
    }else{
        // Clear appropriate bits in buffer
        adcInternal->controlByte = adcInternal->controlByte & (0xFF - mask);
    }

    // Update device pins from buffer
    asm(    "movb %0, %%al          \n"
            "movw %1, %%dx          \n"
            "outb %%al, %%dx        \n"
            :
            : "m" (adcInternal->controlByte), "m" (adcInternal->AD7190)
            : "edx", "eax");
}
END_OF_FUNCTION(isrWriteInterfaceBits)

// -----------------------------------------------------------------------------
// isrSync()
// Global static function
//
// synchronizes both chips
// -----------------------------------------------------------------------------
static void isrSync(CAdcInternal *adcInternal)
{
    // set SYNC low for decent amount of time
    for(int i=0 ; i<50 ; i++)
    {
        isrWriteInterfaceBits(adcInternal, AD7190_SYNC, 0);
    }

    // set SYNC high
    isrWriteInterfaceBits(adcInternal, AD7190_SYNC, 1);
}
END_OF_FUNCTION(isrSync)

// -----------------------------------------------------------------------------
// installHandler()
// Public static function
//
// Used by the CKeypad constructor to configure the interrupt to use the ISR.
// -----------------------------------------------------------------------------
void installAdcHandler()
{

    // chain the new ISR onto the old one so that first the old
    // timer ISR will be called, then the new timer ISR
    // enable interrupt
    int pic, byte;
    pic = 0x21;

    asm(    "xor %%ax, %%ax         \n"		// read PIC
            "mov %0, %%dx           \n"
            "in %%dx, %%al          \n"
            "mov %%ax, %1           \n"
            :
            : "m" (pic), "m" (byte)
            : "edx", "eax");

    byte = byte & (~AD7190_MASKBIT);		// reset mask bit

    asm(    "movb %0, %%al          \n"		// write to PIC
            "movw %1, %%dx          \n"
            "outb %%al, %%dx        \n"
            :
            : "m" (byte), "m" (pic)
            : "edx", "eax");
}

// -----------------------------------------------------------------------------
// CAdc Constructor
// Public member of CAdc class
//
// Accepts a value of type ADC_TYPE to set what hardware to access.
// -----------------------------------------------------------------------------
CAdc::CAdc(ADC_TYPE type, int* hwVer)
{
    // used while expanding the static instance list
    CAdcInternal** adcInternalTemp;

    // if this is the first instance...
    if(adcInternal == NULL)
    {
        // allocate array
        adcInternalSize++;
        adcInternal = new CAdcInternal*[adcInternalSize];
    }
    else
    {
        // expand array
        //
        // backup old pointer
        adcInternalTemp = adcInternal;

        // allocate new array
        adcInternalSize++;
        adcInternal = new CAdcInternal*[adcInternalSize];

        // copy all pointers to new array
        for(int i=0 ; i<adcInternalSize-1 ; i++)
        {
            adcInternal[i] = adcInternalTemp[i];
        }

        // clean up old array
        delete[] adcInternalTemp;
        adcInternalTemp = NULL;
    }

    // set this object's index
    adcInternalIndex = adcInternalSize-1;

    // create new CAdcInternal object and set mainPointer
    adcInternal[adcInternalIndex] = new CAdcInternal;
    adcInternal[adcInternalIndex]->mainPointer = this;

    // set I/O address
    switch(type)
    {
        case D4601_200:
            adcInternal[adcInternalIndex]->AD7190 = 0x200;	// AD7190 address (PC104)
            break;
        case D4601_201:
            adcInternal[adcInternalIndex]->AD7190 = 0x201;	// AD7190 address (PC104)
            break;
        case D4601_202:
            adcInternal[adcInternalIndex]->AD7190 = 0x202;	// AD7190 address (PC104)
            break;
        case D4601_203:
            adcInternal[adcInternalIndex]->AD7190 = 0x203;	// AD7190 address (PC104)
            break;
        case D4601_204:
            adcInternal[adcInternalIndex]->AD7190 = 0x204;	// AD7190 address (PC104)
            break;
        case D4601_205:
            adcInternal[adcInternalIndex]->AD7190 = 0x205;	// AD7190 address (PC104)
            break;
        case D4601_206:
            adcInternal[adcInternalIndex]->AD7190 = 0x206;	// AD7190 address (PC104)
            break;
        case D4627:
            adcInternal[adcInternalIndex]->AD7190 = 0x300;	// AD7190 address (PC104)
            break;
        case D4627v2:
            adcInternal[adcInternalIndex]->AD7190 = 0x180;	// AD7190 address (PC104)
            break;
    }

        adcInternal[adcInternalIndex]->adcCounter = 0;

        #ifndef VIRTUALMACHINE
    // initialize hardware
    if(!setup())
    {
                // hw init failed...
                if(type == D4627v2)
                {
                        // D4627 rev 2
                        // try to detect on 0x300
                        adcInternal[adcInternalIndex]->AD7190 = 0x300;
                        adcInternal[adcInternalIndex]->adcCounter = 0;

                        if(setup())
                        {
                                // fix this...
                                *hwVer = 2;
                        }
                        else
                        {
                                // ADC init failed..
                        }

                }
        }


    installAdcHandler();
    #endif
}

// -----------------------------------------------------------------------------
// CAdc Destructor
// Public member of CAdc class
//
// Cleans up dynamically allocated variables and unwanted pointers from
// the static self-reference array.
// -----------------------------------------------------------------------------
CAdc::~CAdc()
{
    CAdcInternal** adcInternalTemp;

    delete adcInternal[adcInternalIndex];
    adcInternal[adcInternalIndex] = NULL;

    // if this is the last instance...
    if(adcInternalSize == 1)
    {
        // de-allocate array
        adcInternalSize--;
        delete[] adcInternal;
        adcInternal = NULL;
    }else
    {
        // delete pointer from array
        //
        // backup old pointer
        adcInternalTemp = adcInternal;

        // allocate new array
        adcInternalSize--;
        adcInternal = new CAdcInternal*[adcInternalSize];

        // copy all pointers to new array
        // except the one for this object
        //
        // copy before
        for(int i=0 ; i<adcInternalIndex ; i++)
        {
                adcInternal[i] = adcInternalTemp[i];
        }
        // copy after
        for(int i=adcInternalIndex+1 ; i<=adcInternalSize ; i++)
        {
                adcInternal[i-1] = adcInternalTemp[i];
                adcInternal[i-1]->mainPointer->adcInternalIndex--;
        }

        // clean up old array
        delete[] adcInternalTemp;
        adcInternalTemp = NULL;
    }
}

// -----------------------------------------------------------------------------
// clearBuffer()
// Public member of CAdc class
//
// Clears input buffer and waits for all filters to settle
// -----------------------------------------------------------------------------
void CAdc::clearBuffer()
{
        adcInternal[0]->adcCounter = 0;
}

// -----------------------------------------------------------------------------
// updateCounts()
// Checks to see if new data is available, copies new counts if available.
// Returns # of updates since the time it was checked. (0 means no new data)
//
// Must disable ints before reading
// -----------------------------------------------------------------------------
int CAdc::updateCounts()
{
    int ret = 0;
        int pic, byte;

        /* disable interrupt */
        pic = 0x21;

        asm(    "xor %%ax, %%ax         \n"
                        "mov %0, %%dx           \n"
                        "in %%dx, %%al          \n"
                        "mov %%ax, %1           \n"
                        :
                        : "m" (pic), "m" (byte)
                        : "edx", "eax");

        byte = byte | AD7190_MASKBIT;

        asm(    "movb %0, %%al          \n"
                        "movw %1, %%dx          \n"
                        "outb %%al, %%dx        \n"
                        :
                        : "m" (byte), "m" (pic)
                        : "edx", "eax");

        if(adcInternal[adcInternalIndex]->adcCounter == 0)
        {
                // no new data from the interrupt
                ret = 0;
        }
        else
        {
                latestCounts.ch1 = adcInternal[adcInternalIndex]->adcBuffer[0].ch1;
                latestCounts.ch2 = adcInternal[adcInternalIndex]->adcBuffer[0].ch2;

                for(int i=0; i<(adcInternal[adcInternalIndex]->adcCounter - 1); i++)
                {
                        adcInternal[adcInternalIndex]->adcBuffer[i] = adcInternal[adcInternalIndex]->adcBuffer[i+1];
                }

                // reset the counter
                ret = adcInternal[adcInternalIndex]->adcCounter;
                adcInternal[adcInternalIndex]->adcCounter--;
        }

        /* enable interrupt */
        pic = 0x21;

        asm(    "xor %%ax, %%ax         \n"
                        "mov %0, %%dx           \n"
                        "in %%dx, %%al          \n"
                        "mov %%ax, %1           \n"
                        :
                        : "m" (pic), "m" (byte)
                        : "edx", "eax");

        byte = byte & (~AD7190_MASKBIT);

        asm(    "movb %0, %%al          \n"
                        "movw %1, %%dx          \n"
                        "outb %%al, %%dx        \n"
                        :
                        : "m" (byte), "m" (pic)
                        : "edx", "eax");

        return(ret);
}

// -----------------------------------------------------------------------------
// getCounts()
// Public member of CAdc class
//
// Retrieves stored ADC counts from the object.
// -----------------------------------------------------------------------------
CCounts CAdc::getCounts() const
{
    return latestCounts;
}

// -----------------------------------------------------------------------------
// getN()
// Public member of CAdc class
//
// Added this function - 10-14-09 Ashis Vaidya
//
// Returns the value of the FS[0-9] bits stored in modeReg, expressed in Hz.
// Currently only used in the filter summary menu.
//
// Data Rate = (MCLK/16/64)/FS
// (MCLK = 4915200 here)
// -----------------------------------------------------------------------------
int CAdc::getDataRate()
{
        return(4800/(modeReg & 0x03FF));
}


void CAdc::setDataRate(int newDataRate)
{
        // if new and present are same, quit
        if( newDataRate == 4800/(modeReg & 0x03FF) ||
            newDataRate < 5 || newDataRate >4800 ){return;}

        modeReg &= ~0x03FF;
        modeReg |= (4800/newDataRate);
        updateModeReg(true);
}


// -----------------------------------------------------------------------------
// updateModeReg()
// Private member of CAdc class
//
// Added this function - 10-14-09 Ashis Vaidya
//
// Updates the Mode register of the ADC with the new value.
//
// Plan to add capability to read the mode register in next rev
//      - Can autodetect ADC (new/old) using this
//
// Set disableInts to false if calling from within an interrupt
// -----------------------------------------------------------------------------
void CAdc::updateModeReg(bool disableInts)
{
        int pic, byte;

        if(disableInts)
        {
                // disable interrupt
                pic = 0x21;

                asm(    "xor %%ax, %%ax         \n"
                                "mov %0, %%dx           \n"
                                "in %%dx, %%al          \n"
                                "mov %%ax, %1           \n"
                                :
                                : "m" (pic), "m" (byte)
                                : "edx", "eax");

                byte = byte | AD7190_MASKBIT;

                asm(    "movb %0, %%al          \n"
                                "movw %1, %%dx          \n"
                                "outb %%al, %%dx    s    \n"
                                :
                                : "m" (byte), "m" (pic)
                                : "edx", "eax");
        }

        // disable continuous read
        while(!isrIsRdy(adcInternal[adcInternalIndex], 3));
        writeDataByte(3, 0x58);

        // Update mode register
        writeModeReg(3,	modeReg);

        // Set for continuous read
        writeDataByte(3, 0x5C);

        if(disableInts)
        {
                // enable interrupt
                pic = 0x21;

                asm(    "xor %%ax, %%ax         \n"
                                "mov %0, %%dx           \n"
                                "in %%dx, %%al          \n"
                                "mov %%ax, %1           \n"
                                :
                                : "m" (pic), "m" (byte)
                                : "edx", "eax");

                byte = byte & (~AD7190_MASKBIT);

                asm(    "movb %0, %%al          \n"
                                "movw %1, %%dx          \n"
                                "outb %%al, %%dx        \n"
                                :
                                : "m" (byte), "m" (pic)
                                : "edx", "eax");
        }
}


// -----------------------------------------------------------------------------
// setup()
// Private member of CAdc class
//
// Initializes the ADC hardware.  Called by the CAdc constructor.
//
// Modified this function - 10-14-09 Ashis Vaidya
// Simplified and changed modeReg FS to 12hz
//
// -----------------------------------------------------------------------------
bool CAdc::setup()
{
    // set both chip's cs active
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], AD7190_CS0 + AD7190_CS1, 0);

    // set common sync inactive
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], AD7190_SYNC, 1);

    // reset both devices
    for(int i=0 ; i<45 ; i++)
    {
        writeDataBit(3, 1);
    }
    CTimer::wait(0.1);

        conReg = AD7190_CONFIG_AIN1AIN2 | AD7190_CONFIG_BUFFERED | AD7190_CONFIG_GAIN128;

    // set config registers
    writeConfigReg(3, conReg);

    if(!selfCal())
    {
                return(false);
        }

    // set mode registers
        modeReg = AD7190_MODE_EXTCLK | AD7190_MODE_NOCHOP_20HZ;
        writeModeReg(3,	modeReg);

    isrSync(adcInternal[adcInternalIndex]);

    // Set for continuous read
    writeDataByte(3, 0x5C);

    return(true);
}

// -----------------------------------------------------------------------------
// selfCal()
// Private member of CAdc class
//
// Performs internal calibration of both ADC devices and updates
// both mode registers.
// -----------------------------------------------------------------------------
bool CAdc::selfCal()
{
    CTimer selfCalTimeout;

    // run internal zero calibration
    writeModeReg(3, AD7190_MODE_DOZEROCAL);

    selfCalTimeout.reset();
    selfCalTimeout.start();

    while(!isrIsRdy(adcInternal[adcInternalIndex], 3))
        {
                if(selfCalTimeout.elapsed() > SELF_CAL_TIMEOUT)
                {
                        return(false);
                }
        }

    // run internal full calibration
    writeModeReg(3, AD7190_MODE_DOFULLCAL);

    selfCalTimeout.reset();
    selfCalTimeout.start();

    while(!isrIsRdy(adcInternal[adcInternalIndex], 3))
    {
                if(selfCalTimeout.elapsed() > SELF_CAL_TIMEOUT)
                {
                        return(false);
                }
        }

        return(true);
}

// -----------------------------------------------------------------------------
// writeDataBit()
// Private member of CAdc class
//
// Uses writeInterfaceBits() to write a data bit to the specified ADC device(s).
//
// int channel:
// 1 = ADC1
// 2 = ADC2
// 3 = ADC1 & ADC2
// -----------------------------------------------------------------------------
void CAdc::writeDataBit(int channel, bool bit)
{
    char din, sclk;

    din = 0;
    sclk = 0;

    // Set pin values for channel 1 if selected
    if((channel & 1) != 0)
    {
        din = din + AD7190_DIN0;
        sclk = sclk + AD7190_SCLK0;
    }

    // Set pin values for channel 2 if selected
    if((channel & 2) != 0)
    {
        din = din + AD7190_DIN1;
        sclk = sclk + AD7190_SCLK1;
    }

    // Write the bit(s) in the middle of a clock cycle
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], sclk,	0);
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], din,	bit);
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], sclk,	1);

    // Reset the input bit(s)
    isrWriteInterfaceBits(adcInternal[adcInternalIndex], din,	0);
}

// -----------------------------------------------------------------------------
// writeDataByte()
// Private member of CAdc class
//
// Uses writeDataBit() to write a whole byte to the specified ADC device(s).
//
// Note: Data stream is MSB first.
//
// int channel:
// 1 = ADC1
// 2 = ADC2
// 3 = ADC1 & ADC2
// -----------------------------------------------------------------------------
void CAdc::writeDataByte(int channel, char datum)
{
    unsigned char i;
    bool b;

    // Start from byte's MSB
    i = 0x80;

    // Until we're done writing bits
    while(i != 0)
    {
        // Set b to the value of the bit we want to write
        if((datum & i) != 0)
        {
            b = true;
        }else
        {
            b = false;
        }

        // Write the bit
        writeDataBit(channel, b);

        // Next bit
        i = i >> 1;
    }
}

// -----------------------------------------------------------------------------
// writeModeReg()
// Private member of CAdc class
//
// Uses writeDataByte() to write a 24-bit value to the specified
// ADC device's mode register.
//
// int channel:
// 1 = ADC1
// 2 = ADC2
// 3 = ADC1 & ADC2
// -----------------------------------------------------------------------------
void CAdc::writeModeReg(int channel, long cmd)
{
    // Tell it we're writing to the mode register
    writeDataByte(channel, 0x08);

    // Write the mode register, highest byte first
    writeDataByte(channel, cmd / 256 / 256);
    writeDataByte(channel, (cmd / 256) % 256);
    writeDataByte(channel, cmd % 256);
}

// -----------------------------------------------------------------------------
// writeConfigReg()
// Private member of CAdc class
//
// Uses writeDataByte() to write a 24-bit value to the specified
// ADC device's config register.
//
// int channel:
// 1 = ADC1
// 2 = ADC2
// 3 = ADC1 & ADC2
// -----------------------------------------------------------------------------
void CAdc::writeConfigReg(int channel, long cmd)
{
    // Tell it we're writing to the configuration register
    writeDataByte(channel, 0x10);

    // Write the configuration register, high byte first
    writeDataByte(channel, cmd / 256 / 256);
    writeDataByte(channel, (cmd / 256) % 256);
    writeDataByte(channel, cmd % 256);
}
