COMMON    = ../common

DBG      ?=
CXX      ?= g++
CXXFLAGS  = -O3 -fopenmp -I. -std=c++11 -I$(COMMON) $(DBG)

ifeq ($(CXX),icpc)
  CXXFLAGS += -xHost #-no-vec
  CXXFLAGS += -qopt-report=5
  CXXFLAGS += -Wunknown-pragmas # Disable warning about OpenMP pragma no defined.
endif

ifeq ($(CXX),g++)
  CXXFLAGS += -mtune=native -march=native
endif

EXEC = fixanalyzer

all: $(EXEC)

include $(COMMON)/Makefile.common
LDFLAGS      = $(COMMON_LIBS)

util.o: util.cpp
	$(CXX) $(CXXFLAGS) -c util.cpp

fixanalyzer: util.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o fixanalyzer util.o $(COMMON_OBJS) $(LDFLAGS)

clean: clean_common
	/bin/rm -fv $(EXEC) *.o *.optrpt


# sbatch -J dvolume -p normal -N 1 -n 128 -t 09:00:00 benchmark.sh
# for t in 1 2 4 6 8 16 32 64; do export OMP_NUM_THREADS=$t; for i in $(ls $WORK/data/05/);do $HOME/fixanalyzer -p $i -t 52 -n 4 -m 8 > $t-${i:11:17}.log; done; done




