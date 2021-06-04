CPP=g++

BINS=mergesort mergesort4bloques parallel_mergesort parallel_mergesort4bloques parallel_2_mergesort parallel_2_mergesort4bloques 

CPPFLAGS=-Wall -O9 -g -DNDEBUG -I.

OMPFLAGS=-fopenmp

DEST=.

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: clean bin

bin: $(BINS)

mergesort:
	$(CPP) -o $(DEST)/mergesort mergesort.cpp -lm

mergesort4bloques:
	$(CPP) -o $(DEST)/mergesort4bloques mergesort4bloques.cpp -lm

parallel_mergesort:
	$(CPP) $(OMPFLAGS) -o $(DEST)/parallel_mergesort parallel_mergesort.cpp -lm

parallel_mergesort4bloques:
	$(CPP) $(OMPFLAGS) -o $(DEST)/parallel_mergesort4bloques parallel_mergesort4bloques.cpp -lm

parallel_2_mergesort:
	$(CPP) $(OMPFLAGS) -o $(DEST)/parallel_2_mergesort parallel_2_mergesort.cpp -lm

parallel_2_mergesort4bloques:
	$(CPP) $(OMPFLAGS) -o $(DEST)/parallel_2_mergesort4bloques parallel_2_mergesort4bloques.cpp -lm

clean:
	rm -f $(BINS)
	cd $(DEST); rm -f *.a $(BINS)
