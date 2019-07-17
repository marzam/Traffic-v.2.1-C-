################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#
#  versão de 2.1 C++
#
################################################################################
EXEFILE      = TModelCA
VERSION      = -D_VERSION=\"0.1\"
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
CPUCC     = /opt/intel/bin/icpc
#CPUCC     = /usr/bin/g++-7
#CPUCC     = g++
CPPFLAGS  = --std=c++11 -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      = $(APPLICATION)  $(VERSION) -DALIGN=64
INCLUDES  =	-I. -I/usr/include/x86_64-linux-gnu/
LIBDIR   = -L/usr/lib
LIBS     =  -lm -lpthread
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

ifeq ($(compiler), g++)
	CPUCC   = g++
	EXEFILE = TModelCA.g++
endif

ifeq ($(target), release)
  DEFS      +=
  CPPFLAGS  +=      -Ofast
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all:		pThreadClass   		\
		CellularAutomata 	\
		Entity 		        \
		main

	$(CPU_COMPILE)  pThreadClass.o      \
			CellularAutomata.o \
			Entity.o           \
			main.o             \
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp

Entity:
	$(CPU_COMPILE) -c Entity.cpp

TModel:
	$(CPU_COMPILE) -c TModel.cpp


#MovementSensor:
#	$(CPU_COMPILE) -c MovementSensor.cpp

CellularAutomata:
	$(CPU_COMPILE) -c CellularAutomata.cpp

pThreadClass:
	$(CPU_COMPILE) -c pThreadClass.cpp



clean:
	rm -rf *.o
