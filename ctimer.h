
#ifndef CTIMER_H_INCLUDED
#define CTIMER_H_INCLUDED

#include <time.h>

#define CLOCKS_SEC 1024

class CTimer
{
        public:
                CTimer();
                ~CTimer();

                static void wait(double seconds);

                void start();
                void pause();
                void reset();
                void restart();
                double elapsed() const;

                bool isRunning();
        private:
                clock_t startTime;
                double totalTime;
                bool running;
};

#endif // CTIMER_H_INCLUDED
