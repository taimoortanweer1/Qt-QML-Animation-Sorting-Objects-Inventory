
using namespace std;
#include <iostream>
#include <math.h>
#include <algorithm>
#include "cboxcar.h"
#include "miscfunc.h"

CBoxcar::CBoxcar(int size)
{
    m_maxSize = size;
    m_buffer = new double[m_maxSize];
    m_bufferSize = 0;
    m_bufferStart = 0;
    m_total = 0;
    m_numerator = 0;
}


CBoxcar::~CBoxcar()
{
    if(m_buffer != NULL)
    {
        delete[] m_buffer;
    }
}


int CBoxcar::samples(bool trim)
{
    if(trim && (m_bufferSize > 2))
    {
        return (m_bufferSize-2);

    }
    else
    {
        return m_bufferSize;
    }
}


// -----------------------------------------------------------------------------
// Added this function - 10-14-09 Ashis Vaidya
// Clears all values in buffer, an alternative
// to deleting and re-creating a buffer
// -----------------------------------------------------------------------------
void CBoxcar::reset()
{
    if(m_buffer != NULL)
    {
        delete[] m_buffer;
    }

    m_buffer = new double[m_maxSize];
    m_bufferSize = 0;
    m_bufferStart = 0;
    m_total = 0;
    m_numerator = 0;
}


// -----------------------------------------------------------------------------
// Added this function - 11-10-09 Ashis Vaidya
// Returns how many samples are in the boxcar.
// Makes it easy to find out if the buffer has been filled.
// -----------------------------------------------------------------------------
bool CBoxcar::full()
{
    return(m_bufferSize >= m_maxSize);
}


int CBoxcar::size()
{
    return(m_bufferSize);
}


void CBoxcar::nextValue(double value)
{
    if(m_bufferSize >= m_maxSize)
    {
        // rotate buffer
        m_numerator = m_numerator - m_total + (value * m_bufferSize);
        m_total -= m_buffer[m_bufferStart];
        m_total += value;
        m_buffer[m_bufferStart] = value;
        m_bufferStart = (m_bufferStart + 1) % m_maxSize;
    }else
    {
        // grow buffer
        m_buffer[(m_bufferStart + m_bufferSize) % m_maxSize] = value;
        m_bufferSize++;
        m_total += value;
        m_numerator += (value * m_bufferSize);
    }
}


// -----------------------------------------------------------------------------
// Modified this function - 10-14-09 Ashis Vaidya
//
// Supports a modified boxcar filtering (trim = true) which removes
// the largest and smallest values in the buffer before averaging.
// Requires that the size of the filter is 3 or greater.
// -----------------------------------------------------------------------------
double CBoxcar::getBoxcar(bool trim)
{
    double max_val, min_val;
    int search_limit_idx;

    if(trim && (m_bufferSize > 2))
    {
        // As per the Analong Devices paper, remove the max/min values.
        // Return average of remaining values.

        if(m_bufferSize >= m_maxSize)			  // all elements of m_buffer are populated
        {
            search_limit_idx = m_maxSize;

            // Don't use these because they barf when called from an interrupt
            //max_val = *max_element(m_buffer,m_buffer + (m_maxSize-1));
            //min_val = *min_element(m_buffer,m_buffer + (m_maxSize-1));
        }
        else
        {
            search_limit_idx = m_bufferSize;
            //max_val = *max_element(m_buffer,m_buffer + (m_bufferSize-1));
            //min_val = *min_element(m_buffer,m_buffer + (m_bufferSize-1));
        }

        max_val = m_buffer[0];
        min_val = m_buffer[0];

        for(int i=1; i < search_limit_idx; i++)
        {
            if(m_buffer[i] > max_val)
            {
                max_val = m_buffer[i];
            }
            if(m_buffer[i] < min_val)
            {
                min_val = m_buffer[i];
            }
        }

        return ( (m_total - max_val - min_val) / (m_bufferSize-2) );

    }
    else if(m_bufferSize == 0)
    {
        return(0);
    }
    else
    {
        return m_total / m_bufferSize;
    }
}


double CBoxcar::getMax()
{
    double max_val = 0;

    if(m_bufferSize > 0)
    {
        // As per the Analong Devices paper, remove the max/min values.
        // Return average of remaining values.

        if(m_bufferSize >= m_maxSize)			  // all elements of m_buffer are populated
        {
            //*max_element(m_buffer,m_buffer + (m_maxSize-1)) = 0;
            max_val = *max_element(m_buffer,m_buffer + (m_maxSize-1));
        }
        else
        {
            //*max_element(m_buffer,m_buffer + (m_bufferSize-1)) = 0;
            max_val = *max_element(m_buffer,m_buffer + (m_bufferSize-1));
        }
    }

    //reset();

    return ( max_val );
}


double CBoxcar::getWma()
{
    return m_numerator / (m_bufferSize * (m_bufferSize + 1) / 2);
}


double CBoxcar::standardDeviation()
{
    if(m_bufferSize == 0)
    {
        return(0);
    }
    else
    {
        return(stdDev(m_buffer, m_bufferSize));
    }
}


void CBoxcar::resize(int newSize)
{
    // remove older entries form the buffer...
    // but keep buffersize the same
    if(newSize >= m_bufferSize){return;}

    for(int i=0; i < m_bufferSize-newSize; i++)
    {
        m_total -= m_buffer[m_bufferStart];

        m_bufferStart = (m_bufferStart + 1) % m_maxSize;

        m_bufferSize--;
    }
}
/////////////////////////////////

/*CWindowedSinc::CWindowedSinc(int m, double freq)
{
    double k;
    k = 0;

    if(m%2 == 1)
    {
        cout << "Error: Bad windowed sinc filter size" << endl;
        exit(1);
    }

    m_m = m;
    m_maxSize = m + 1;
    m_buffer = new double[m_maxSize];
    m_bufferSize = 0;
    m_bufferStart = 0;
    m_kernel = new double[m_maxSize];

    // generate kernel
    for(int i=0 ; i<m_maxSize ; i++)
    {
        if(i == m_m/2)
        {
            m_kernel[i] = 2.0 * M_PI * freq;
        }else
        {
            m_kernel[i] = sin(2.0*M_PI*freq*((double)i-((double)m_m/2.0))) / ((double)i-((double)m_m/2.0)) *
                (0.42 - (0.5 * cos(2.0*M_PI*(double)i/(double)m_m)) + (0.08 * cos(4.0*M_PI*(double)i/(double)m_m)));
        }

        k += m_kernel[i];
    }

    // normalize kernel
    for(int i=0 ; i<m_maxSize ; i++)
    {
        m_kernel[i] /= k;
    }
}

CWindowedSinc::~CWindowedSinc()
{
    if(m_buffer != NULL)
    {
        delete[] m_buffer;
    }

    if(m_kernel != NULL)
    {
        delete[] m_kernel;
    }
}

void CWindowedSinc::nextValue(double value)
{
    if(m_bufferSize >= m_maxSize)
    {
        // rotate buffer
        m_buffer[m_bufferStart] = value;
        m_bufferStart = (m_bufferStart + 1) % m_maxSize;
    }else
    {
        // grow buffer
        m_buffer[(m_bufferStart + m_bufferSize) % m_maxSize] = value;
        m_bufferSize++;
    }
}

double CWindowedSinc::getOutput()
{
    double rtn;
    rtn = 0;

    if(m_bufferSize >= m_maxSize)
    {
        for(int i=0 ; i<m_maxSize ; i++)
        {
            rtn += (m_buffer[(m_bufferStart + i) % m_bufferSize] * m_kernel[i]);
        }
        return rtn;
    }else
    {
        return 0;
    }
}*/
