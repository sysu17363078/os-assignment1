objects = mycall 
.PHONY : clean

dph : dph.o
	gcc -o dph dph.o -lpthread 

prod : prod.o
	gcc -o prod prod.o -lrt -pthread -lm

cons : cons.o
	gcc -o cons cons.o -lrt -pthread -lm

mycall : mycall.o
	gcc -o mycall mycall.o
clean :
	rm -rf *.o
