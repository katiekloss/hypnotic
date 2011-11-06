CFLAGS=-g -I/usr/local/include
LDFLAGS=-L libhdhomerun -L /usr/local/lib

hypnotic: discover.o record.o main.o
	cc $(LDFLAGS) $(CFLAGS) -lhdhomerun -lsqlite3 discover.o record.o main.o -o hypnotic

all: hypnotic

clean:
	rm -f *.core core *.o hypnotic
