target := main
targetPlain := $(target)

ifeq ($(OS), Windows_NT)
	target := $(target).exe
endif

.PHONY: run 

run: $(target)
	./$(target)

$(target): $(targetPlain).cpp dbg.hpp
	g++ -include dbg.hpp -o $(target) $(targetPlain).cpp -Wall -std=c++17
