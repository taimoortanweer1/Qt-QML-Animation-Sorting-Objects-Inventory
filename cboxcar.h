#ifndef CBOXCAR_H_INCLUDED
#define CBOXCAR_H_INCLUDED

using namespace std;

class CBoxcar
{
    public:
        CBoxcar(int size);
        ~CBoxcar();

                void nextValue(double value);
                double getMax();
                double getBoxcar(bool trim = false);// Modified this function - 10-14-09 Ashis Vaidya
                double getWma();
                double standardDeviation();

        int samples(bool trim = false);

        void resize(int newSize);

        void reset();
        bool full();
        int size();

    private:
        int m_maxSize;

        double* m_buffer;
        int m_bufferSize;
        int m_bufferStart;

        double m_total;
        double m_numerator;
};

/*class CWindowedSinc
{
    public:
        CWindowedSinc(int m, double freq);
        ~CWindowedSinc();

        void nextValue(double value);
        double getOutput();

    private:
        int m_m;

        int m_maxSize;
        double *m_buffer;
        int m_bufferSize;
        int m_bufferStart;

        double *m_kernel;
};*/

#endif // CBOXCAR_H_INCLUDED
