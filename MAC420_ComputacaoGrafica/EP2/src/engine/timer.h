/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: timer.h                                            **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
/*
I wanted to have a simple way to measure time intervals with great accuracy, like greater than a few milliseconds, so I've come up with this.

Unfortunately C/C++ does not have any standard way of doing so (at least ANSI C++... Apparently C++11x has it).
cross-platform functions such as time.h's clock() or time() are innacurate.
Instead, we need to rely on platform-specific methods to measure time with great accuracy, so... Get ready for lots of macros along with code =P

NOTE:
I saw the basis for this class online, thanks to StackOverflow.com
*/
#ifndef TIMER_H_
#define TIMER_H_

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

namespace engine {

class Timer {
public:
	Timer();
	~Timer();
    void   Start();
    void   Stop();
    double elapsed_time_secs();                    // get elapsed time in second
    double elapsed_time_millisecs();          // get elapsed time in milli-second
    double elapsed_time_microsecs();          // get elapsed time in micro-second


private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
};

}
#endif