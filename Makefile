make:
	g++ main.cpp CSRMatrix.cpp mmio.c -o main -fopenmp

run:
	./main channel-500x100x100-b050.mtx
	./main delaunay_n19.mtx
	./main NLR.mtx

clean:
	rm *.o
	rm main
