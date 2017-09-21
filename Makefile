make:
	g++ main.cpp CSRMatrix.cpp mmio.c -o main -fopenmp

run:
	./main channel-500x100x100-b050.mtx delaunay_n19.mtx NLR.mtx

clean:
	rm main
