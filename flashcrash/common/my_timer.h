#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define HighResTimer

#ifdef HighResTimer

   typedef struct timespec myTimer_t;

   //#warning 'Using HighResTimer'

#else

   typedef struct timeval myTimer_t;

   //#warning 'Using LowResTimer'

#endif

myTimer_t getTimeStamp(void);
uint64_t getClockTicks(void);
double getElapsedTime (myTimer_t t0, myTimer_t t1);
double getTicksPerSecond (void);

#ifdef __cplusplus
} // extern "C"
#endif
