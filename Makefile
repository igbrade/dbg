
target := main

ifeq ($(OS), Windows_NT)
	target := $(target).exe
endif

.PHONY: run

run: $(target)
	./$(target)

$(target): main.cpp
	g++ -o main main.cpp -Wall -std=c++17
