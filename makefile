all:
	g++ -o benchmark benchmark.cpp
	./benchmark

clean:
	rm benchmark
