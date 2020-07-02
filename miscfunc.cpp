// -----------------------------------------------------------------------------
// File: miscFunc.cpp
// Date: 20071211
// Author(s): Nicholas Corey
//
// A place to put various static functions that are useful
// for other libraries.
//
// Copyright (c) 2007, RYCO Equipment, Inc
// All rights reserved worldwide
// -----------------------------------------------------------------------------
// History:
//
// 20071211
// Initial C++ Release
// -----------------------------------------------------------------------------

#include "miscfunc.h"
#include <sstream>
#include <math.h>


string doubleToString(double input, int minDecPlaces, int maxDecPlaces)
{
        int decLoc;
        string temp;

        ostringstream oss;
        oss << fixed << input;

        // remove trailing zeroes
        while((string("0")).compare(oss.str().substr(oss.str().length()-1, 1)) == 0)
        {
            oss.str(oss.str().substr(0, oss.str().length()-1));
        }

        // remove period if last character
        if((string(".")).compare(oss.str().substr(oss.str().length()-1, 1)) == 0)
        {
            oss.str(oss.str().substr(0, oss.str().length()-1));
        }

        if(minDecPlaces > 0)
        {
                decLoc = oss.str().length();
                while(decLoc >= 0 && oss.str()[decLoc] != '.')
                {
                        if(oss.str()[decLoc] == 'e') return oss.str();
                        decLoc--;
                }

                if(decLoc >= 0)
                {
                        for(int i=0 ; i<minDecPlaces-(int)oss.str().length()+decLoc+1 ; i++)
                        {
                                temp = temp + "0";
                        }
                }else
                {
                        temp = ".";
                        for(int i=0 ; i<minDecPlaces ; i++)
                        {
                                temp = temp + "0";
                        }
                }
        }

        temp = oss.str() + temp;

        decLoc = temp.length();
        while(decLoc >= 0 && temp[decLoc] != '.')
        {
                if(temp[decLoc] == 'e') return temp;
                decLoc--;
        }

        if(decLoc >= 0)
        {
                if(maxDecPlaces <= 0)
                {
                        temp = temp.substr(0, decLoc);
                }else
                {
                        temp = temp.substr(0, decLoc+maxDecPlaces+1);
                }
        }

        return temp;
}

/*string longToString(long input)
{
        ostringstream oss;
        oss << input;
        return oss.str();
}*/

double stringToDouble(const string* input)
{
        double output;
        istringstream iss(*input);

        iss >> output;

        return output;
}

double absd(double input)
{
        if(input < 0.0) return -input;
        return input;
}

long absl(long input)
{
    if(input < 0) return -input;
    return input;
}


/*string resizeLeft(string input, int newSize, char padChar)
{
        if((int)input.length() > newSize)
        {
                return(input.substr(input.length()-newSize, newSize));
        }

        while((int)input.length() < newSize)
        {
                input = padChar + input;
        }

        return input;
}

string capitalize(string input)
{
    if(input[0] >= 'a')
    {
        input[0] = input[0] - ('a' - 'A');
    }
    return input;
}

#define CALSWITCH_ADDR 0x300
#define CALSWITCH_MASK 0x08

bool calSwitch()
{
    int addr;
    int ret;

    addr = CALSWITCH_ADDR;

    // get keypad status
    asm("movw %0, %%dx           \n"
        "inb %%dx, %%al          \n"
        "movb %%al, %1           \n"
        :
        : "m" (addr), "m" (ret)
        : "edx", "eax");

    return ((ret & CALSWITCH_MASK) == 0);
}


void funcKey(CVga *vga, int buttonNum, CFont *font, char *text)
{
        switch(buttonNum)
        {
                case 1:
                        vga->drawRectBorder(   0,
                                               CVGA_HEIGHT - 2*SMALL_FONT_HEIGHT,
                                               (CVGA_F1_CENTER + CVGA_F2_CENTER) / 2,
                                               CVGA_HEIGHT - 1);
                        font->printAt(vga, CVGA_F1_CENTER, CVGA_HEIGHT - SMALL_FONT_HEIGHT, text, true, true);
                        break;
                case 2:
                        vga->drawRectBorder(   (CVGA_F1_CENTER + CVGA_F2_CENTER) / 2,
                                               CVGA_HEIGHT - 2*SMALL_FONT_HEIGHT,
                                               (CVGA_F2_CENTER + CVGA_F3_CENTER) / 2,
                                               CVGA_HEIGHT - 1);
                        font->printAt(vga, CVGA_F2_CENTER, CVGA_HEIGHT - SMALL_FONT_HEIGHT, text, true, true);
                        break;
                case 3:
                        vga->drawRectBorder(   (CVGA_F2_CENTER + CVGA_F3_CENTER) / 2,
                                               CVGA_HEIGHT - 2*SMALL_FONT_HEIGHT,
                                               (CVGA_F3_CENTER + CVGA_F4_CENTER) / 2,
                                               CVGA_HEIGHT - 1);
                        font->printAt(vga, CVGA_F3_CENTER, CVGA_HEIGHT - SMALL_FONT_HEIGHT, text, true, true);
                        break;
                case 4:
                        vga->drawRectBorder(   (CVGA_F3_CENTER + CVGA_F4_CENTER) / 2,
                                               CVGA_HEIGHT - 2*SMALL_FONT_HEIGHT,
                                               CVGA_WIDTH-1,
                                               CVGA_HEIGHT - 1);
                        font->printAt(vga, CVGA_F4_CENTER, CVGA_HEIGHT - SMALL_FONT_HEIGHT, text, true, true);
                        break;
        }
}*/


double stdDev(double *array, int arraySize)
{
    double temp1;
    double average;
    double stddev;

    // calulate average of array
    average = 0;
    for(int i=0 ; i<arraySize ; i++)
    {
        average += (array[i] / (double)arraySize);
    }

    // calculate stddev of array
    temp1 = 0;
    for(int i=0 ; i<arraySize ; i++)
    {
        temp1 += (pow(array[i] - average, 2) / ((double)arraySize - 1.0));
    }
    stddev = sqrt(temp1);

    return stddev;
}
