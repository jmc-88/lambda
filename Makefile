CXXFLAGS=-Wall -pedantic -std=c++0x -Iinc

bin/Lexer.o : inc/Lexer.h src/Lexer.cpp
	g++ $(CXXFLAGS) src/Lexer.cpp

all : bin/Lexer.o
