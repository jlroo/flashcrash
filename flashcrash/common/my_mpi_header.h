#ifndef __my_mpi_header_h
#define __my_mpi_header_h

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define callMPI( _cmd_ ) \
{ \
   /* Execute the function and capture the return int. */ \
   int _mpi_cmd_flag = (_cmd_); \
   /* Check the error flag. Print if something went wrong and then abort. */ \
   if (_mpi_cmd_flag != MPI_SUCCESS) \
   { \
      int  _mpi_error_str_length = 0; \
      char _mpi_error_str[MPI_MAX_ERROR_STRING]; \
      MPI_Error_string(_mpi_cmd_flag, _mpi_error_str, &_mpi_error_str_length); \
      fprintf(stderr,"MPI Error caught: errcode=%d errstring=%s\n", _mpi_cmd_flag, _mpi_error_str); \
      MPI_Abort( MPI_COMM_WORLD, _mpi_cmd_flag ); \
      exit(_mpi_cmd_flag); \
   } \
}
 
#endif
