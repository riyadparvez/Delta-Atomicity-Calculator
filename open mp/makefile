OBJS = Zone.o InputProcessor.o Main.o
CC = g++ -pipe
DEBUG = -g
CFLAGS = -Wall -O3 -std=c++11 -fopenmp -c $(DEBUG)
LFLAGS = -Wall -fopenmp -lglog -ltbb -ltbbmalloc -ltbbmalloc_proxy -lrt $(DEBUG)

DeltaComputation: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o DeltaComputation

Zone.o : Zone.h Zone.cpp Score.h 
	$(CC) $(CFLAGS) Zone.cpp

InputProcessor.o : InputProcessor.h InputProcessor.cpp Clusters.h Operation.h FileReader.h StringComparer.h 
	$(CC) $(CFLAGS) InputProcessor.cpp

Main.o : InputProcessor.h InputProcessor.cpp DefaultScoreComputer.h
	$(CC) $(CFLAGS) Main.cpp

clean:
	\rm *.o *~ DeltaComputation
