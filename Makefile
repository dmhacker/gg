CC=g++
FLAGS=-std=c++11
LIBS=-lreadline `wx-config --cxxflags --libs` 

.PHONY: clean

build/gg: src/gg.cpp src/gg.hpp
	$(CC) $(FLAGS) src/gg.cpp -o build/gg $(LIBS)
clean:
	rm -rf build/
