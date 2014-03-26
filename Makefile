CXX=g++
CXXFLAGS+=-Wall -pedantic -std=c++11 -Iinc

all : bin/lambda

clean :
	rm -rf bin
	rm -f inc/PCH.h.gch

bin :
	mkdir -p bin

bin/lambda : bin/Lexer.o bin/Error.o bin/Main.o
	$(CXX) $(CXXFLAGS) -o bin/lambda bin/Lexer.o bin/Error.o bin/Main.o

inc/PCH.h.gch : inc/PCH.h
	$(CXX) $(CXXFLAGS) -o inc/PCH.h.gch inc/PCH.h

bin/Lexer.o : bin inc/PCH.h.gch inc/Lexer.h src/Lexer.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Lexer.o src/Lexer.cpp

bin/Error.o : bin inc/PCH.h.gch inc/Error.h src/Error.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Error.o src/Error.cpp

bin/Main.o : bin inc/PCH.h.gch src/Main.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Main.o src/Main.cpp
