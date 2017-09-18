make:
	g++ -I boost_1_65_1 main.cpp CSRMatrix.cpp mmio.c -o main -fopenmp
	#gcc -c mmio.c
	#gcc -c main.c -lcilkrts -ldl -fcilkplus
	#gcc mmio.o main.o -o main -lcilkrts -ldl -fcilkplus

run:
	./main example.mtx

clean:
	rm *.o
	rm main
