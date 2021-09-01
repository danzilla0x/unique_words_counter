APP_NAME := main

all: $(APP_NAME)
	g++ -std=c++17 -Wall -Wextra main.cpp

.PHONY: clean
clean:
	rm -rf *.o *.out *.exe *.app $(APP_NAME)