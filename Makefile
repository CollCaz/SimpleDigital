P=main
OBJECTS= Gates.cpp
CFLAGS = -pg -g -Wall -Wextra -O0 `pkg-config --cflags raylib`
LDLIBS= `pkg-config --libs raylib` 
# CC=g++
CC=clang++

$(P): $(OBJECTS)
