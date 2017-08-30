make:
	gcc -c mmio.c
	gcc -c main.c
	gcc mmio.o main.o -o main

run:
	./main example.mtx

clean:
	rm *.o
	rm main
