#include <my_timer.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HighResTimer

inline uint64_t __invoke_rdtsc(void)
{
#if defined(__i386__)
   uint64_t x;
   __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
   return x;
#elif defined(__x86_64__)
   uint32_t lo, hi;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return (uint64_t)lo | (((uint64_t)hi) << 32);
#else
   return 0;
#endif
}

#if 0
uint64_t __invoke_rdtscp(void)
{
  uint32_t lo, hi;
  __asm__ volatile ("rdtscp"
      : /* outputs */ "=a" (lo), "=d" (hi)
      : /* no inputs */
      : /* clobbers */ "%rcx");
  return (uint64_t)lo | (((uint64_t)hi) << 32);
}
#endif

//inline
uint64_t getClockTicks(void)
{
   return __invoke_rdtsc();
}

myTimer_t getTimeStamp()
{
   struct timespec t;
   clock_gettime(CLOCK_MONOTONIC, &t);  
   return t;
}
double getElapsedTime(myTimer_t t0, myTimer_t t1)
{
   return double(t1.tv_sec - t0.tv_sec) + 1e-9 * double(t1.tv_nsec - t0.tv_nsec);
}

#else

/* "Accurate" wall-clock timing function. */
myTimer_t getTimeStamp (void)
{
   struct timeval  t;
   gettimeofday (&t, NULL);
   return t;
}
double getElapsedTime(myTimer_t t0, myTimer_t t1)
{
   return double(t1.tv_sec - t0.tv_sec) + 1.0e-6 * double(t1.tv_usec - t0.tv_usec);
}

uint64_t getClockTicks(void) { return 0; }

#endif

double getTicksPerSecond(void)
{
   uint64_t tick_start = getClockTicks();

   uint64_t sum = 0;
   for (int i = 0; i < 100000; ++i) sum += rand();

   uint64_t tick_stop = getClockTicks();

   if (tick_stop - tick_start <= 0)
      return 0;

   myTimer_t time_start = getTimeStamp();

   for (int i = 0; i < 100000; ++i) sum += rand();

   myTimer_t time_stop = getTimeStamp();

   double time_diff = getElapsedTime(time_start, time_stop);

   return double(tick_stop - tick_start) / time_diff;
}

#ifdef __cplusplus
} // extern "C"
#endif
