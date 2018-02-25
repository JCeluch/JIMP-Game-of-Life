VPATH = main:read_write:cycles:data:free
CFLAGS = -lpng -ggdb 


r: data.o read_write.o cycles.o data.o main.o free.o
	cc -o r read_write.o cycles.o main.o data.o free.o -ggdb -lpng

main.o: main.c

data.o: data.c data.h

cycles.o: cycles.c cycles.h

read_write.o: read_write.c read_write.h

free.o: free.c free.h

clean:
	rm *.o *.obr

