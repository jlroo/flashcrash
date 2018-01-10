#ifndef __my_omp_h
#define __my_omp_h

#ifdef _OPENMP
#  include <omp.h>

#  if   (_OPENMP >= 201511)
#    define _MY_OPENMP_MAJOR 4
#    define _MY_OPENMP_MINOR 5
#  elif (_OPENMP >= 201307)
#    define _MY_OPENMP_MAJOR 4
#    define _MY_OPENMP_MINOR 0
#  elif (_OPENMP >= 201107)
#    define _MY_OPENMP_MAJOR 3
#    define _MY_OPENMP_MINOR 1
#  elif (_OPENMP >= 200805)
#    define _MY_OPENMP_MAJOR 3
#    define _MY_OPENMP_MINOR 0
#  elif (_OPENMP >= 200505)
#    define _MY_OPENMP_MAJOR 2
#    define _MY_OPENMP_MINOR 5
#  else
#    define _MY_OPENMP_MAJOR 2
#    define _MY_OPENMP_MINOR 0
#  endif /* (_OPENMP >= DATE) */

#  if (_OPENMP >= 200805)
#    define _MY_OPENMP_HAS_ATOMICS (1)
#  endif

#endif /* _OPENMP */

#define _TOSTRING(_STR) # _STR
#define TOSTRING(_STR) _TOSTRING(_STR)

#endif /*__my_omp_h*/
