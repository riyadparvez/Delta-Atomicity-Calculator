OBJS = Zone.o InputProcessor.o Main.o
CC = g++-4.8 -pipe
DEBUG = -g
CFLAGS = -Wall -O3 -std=c++11 -fopenmp -c $(DEBUG)
LFLAGS = -Wall -fopenmp -lglog -ltbb -ltbbmalloc -ltbbmalloc_proxy -lrt $(DEBUG)

DeltaComputation: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o DeltaComputation

Zone.o : Zone.hpp Zone.cpp Score.hpp 
	$(CC) $(CFLAGS) Zone.cpp

InputProcessor.o : InputProcessor.hpp InputProcessor.cpp Clusters.hpp Operation.hpp FileReader.hpp StringComparer.hpp 
	$(CC) $(CFLAGS) InputProcessor.cpp

Main.o : InputProcessor.hpp InputProcessor.cpp DefaultScoreComputer.hpp
	$(CC) $(CFLAGS) Main.cpp

clean:
	\rm *.o *~ DeltaComputation
