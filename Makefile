make:
	g++ main.cpp CSRMatrix.cpp -o main
	#gcc -c mmio.c
	#gcc -c main.c -lcilkrts -ldl -fcilkplus
	#gcc mmio.o main.o -o main -lcilkrts -ldl -fcilkplus

run:
	./main example.mtx

clean:
	rm *.o
	rm main
