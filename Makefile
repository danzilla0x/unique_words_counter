APP_NAME := main

all:
	g++ -std=c++17 -lpthread -Wall -Wextra -o $(APP_NAME) main.cpp

.PHONY: clean
clean:
	rm -rf *.o *.out *.exe *.app $(APP_NAME)