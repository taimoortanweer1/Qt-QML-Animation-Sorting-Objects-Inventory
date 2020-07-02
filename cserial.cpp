#include <dos.h>
#include <iostream>


using namespace std;

#include "cserial.h"

extern void toggleProxInt(bool on);

// use circular bufs
char Serial1_RXbuf[SERIAL_BUF_SIZE];
char Serial2_RXbuf[SERIAL_BUF_SIZE];
char Serial3_RXbuf[SERIAL_BUF_SIZE];

int Serial1_RXbuf_start = 0;
int Serial1_RXbuf_idx = 0;

int Serial2_RXbuf_start = 0;
int Serial2_RXbuf_idx = 0;

int Serial3_RXbuf_start = 0;
int Serial3_RXbuf_idx = 0;

char Serial1_TXbuf[SERIAL_BUF_SIZE];
char Serial2_TXbuf[SERIAL_BUF_SIZE];
char Serial3_TXbuf[SERIAL_BUF_SIZE];

int Serial1_TXbuf_start = 0;
int Serial1_TXbuf_idx = 0;

int Serial2_TXbuf_start = 0;
int Serial2_TXbuf_idx = 0;

int Serial3_TXbuf_start = 0;
int Serial3_TXbuf_idx = 0;


bool serialCharAvailable(int com_port)
{
    bool ret_val = false;

    toggleProxInt(false);

    if(com_port == COM1)
    {
        if(Serial1_RXbuf_idx > 0)
        {
            ret_val = true;
        }
    }
    else if(com_port == COM2)
    {
        if(Serial2_RXbuf_idx > 0)
        {

            ret_val = true;
        }
    }
    else if(com_port == COM3)
    {
        if(Serial3_RXbuf_idx > 0)
        {

            ret_val = true;
        }
    }

    toggleProxInt(true);

    return(ret_val);
}

bool serialGetChar(int com_port, char* in)
{
    bool ret_val = false;

    toggleProxInt(false);

    if(com_port == COM1)
    {
        if(Serial1_RXbuf_idx > 0)
        {
            *in = Serial1_RXbuf[Serial1_RXbuf_start];

            Serial1_RXbuf_start = (Serial1_RXbuf_start+1)%SERIAL_BUF_SIZE;
            Serial1_RXbuf_idx--;

            ret_val = true;
        }
    }
    else if(com_port == COM2)
    {
        if(Serial2_RXbuf_idx > 0)
        {
            *in = Serial2_RXbuf[Serial2_RXbuf_start];

            Serial2_RXbuf_start = (Serial2_RXbuf_start+1)%SERIAL_BUF_SIZE;
            Serial2_RXbuf_idx--;

            ret_val = true;
        }
    }
    else if(com_port == COM3)
    {
        if(Serial3_RXbuf_idx > 0)
        {
            *in = Serial3_RXbuf[Serial3_RXbuf_start];

            Serial3_RXbuf_start = (Serial3_RXbuf_start+1)%SERIAL_BUF_SIZE;
            Serial3_RXbuf_idx--;

            ret_val = true;
        }
    }

    toggleProxInt(true);

    return(ret_val);
}


/*bool serialGetString(string* in)
{
        if(!RXbuf.empty())
        {
                //toggleProxInt(false);

                *in += RXbuf;
                RXbuf.clear();

                //toggleProxInt(true);

                return(true);
        }
        return(false);
}*/


bool serialSendChar(int com_port, char out)
{
    bool error_f = true;

    toggleProxInt(false);

    if(com_port == COM1)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial1_TXbuf_idx >= 1)
        {

            Serial1_TXbuf[(Serial1_TXbuf_start+Serial1_TXbuf_idx++) % SERIAL_BUF_SIZE] = out;
            error_f = false;
        }
    }
    else if(com_port == COM2)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial2_TXbuf_idx >= 1)
        {
            Serial2_TXbuf[(Serial2_TXbuf_start+Serial2_TXbuf_idx++) % SERIAL_BUF_SIZE] = out;
            error_f = false;
        }
    }
    else if(com_port == COM3)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial3_TXbuf_idx >= 1)
        {
            Serial3_TXbuf[(Serial3_TXbuf_start+Serial3_TXbuf_idx++) % SERIAL_BUF_SIZE] = out;
            error_f = false;
        }
    }

    toggleProxInt(true);

    return(error_f);
}

bool serialSendString(int com_port, const char* out, int size)
{
    bool error_f = true;

    toggleProxInt(false);

    if(com_port == COM1)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial1_TXbuf_idx >= size)
        {
            for(int i=0; i < size; i++)
            {
                Serial1_TXbuf[(Serial1_TXbuf_start+Serial1_TXbuf_idx++) % SERIAL_BUF_SIZE] = out[i];
            }
            error_f = false;
        }
    }
    else if(com_port == COM2)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial2_TXbuf_idx >= size)
        {
            for(int i=0; i < size; i++)
            {
                Serial2_TXbuf[(Serial2_TXbuf_start+Serial2_TXbuf_idx++) % SERIAL_BUF_SIZE] = out[i];
            }
            error_f = false;
        }
    }
    else if(com_port == COM3)
    {
        // check free space in the buffer
        if(SERIAL_BUF_SIZE-Serial3_TXbuf_idx >= size)
        {
            for(int i=0; i < size; i++)
            {
                Serial3_TXbuf[(Serial3_TXbuf_start+Serial3_TXbuf_idx++) % SERIAL_BUF_SIZE] = out[i];
            }
            error_f = false;
        }
    }

    toggleProxInt(true);

    return(error_f);
}


void serialDTR(int com, int newState)
{
}
