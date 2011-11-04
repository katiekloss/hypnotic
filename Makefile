hypnotic: discover.o main.o
	gcc -L libhdhomerun -lhdhomerun discover.o main.o -o hypnotic

all: hypnotic

clean:
	rm -f *.core core *.o hypnotic
