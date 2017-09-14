make:
	gcc -c mmio.c
	gcc -c main.c -lcilkrts -ldl -fcilkplus
	gcc mmio.o main.o -o main -lcilkrts -ldl -fcilkplus

run:
	./main example.mtx

clean:
	rm *.o
	rm main
