#include <stdio.h>
#include <stdlib.h>

#ifdef __ALIGNMENT
#  define Alignment() (__ALIGNMENT)
#else
#  define Alignment() (sizeof(void*))
#endif

template <typename T>
bool isAligned( const T* ptr, const size_t nbytes = Alignment() )
{
   return uintptr_t(ptr) % nbytes == 0;
}

template <typename T>
T * aligned_alloc (size_t nelems)
{
   T *ptr;
   // Allocate with specified alignment. Default is the word size.
   size_t alignment = Alignment();
   int ierr = posix_memalign((void**)&ptr, alignment, sizeof(T)*nelems);
   if (ierr)
   {
      fprintf(stderr,"Aligned allocation error %s %d\n", __FILE__, __LINE__);
      return NULL;
   }
   return ptr;
}

template <typename T>
T * aligned_alloc (size_t nelems, size_t alignment)
{
   T *ptr;
   // Allocate with specified alignment. Default is the word size.
   int ierr = posix_memalign((void**)&ptr, alignment, sizeof(T)*nelems);
   if (ierr)
   {
      fprintf(stderr,"Aligned allocation error %s %d\n", __FILE__, __LINE__);
      return NULL;
   }
   return ptr;
}

template <typename T>
void Deallocate (T* &ptr)
{
   if (ptr)
   {
      free(ptr);
      ptr = NULL;
   }
}

template <typename T>
T* Allocate (T* &ptr, const size_t nelems)
{
   if (ptr)
      Deallocate(ptr);

   ptr = aligned_alloc<T>(nelems);
   if (!ptr)
   {
      fprintf(stderr,"Allocation error for %f kb\n", sizeof(T)*nelems/1024.0);
      exit(-1);
   }

   return ptr;
}
