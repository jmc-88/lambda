CXX=g++
CXXFLAGS+=-Wall -pedantic -std=c++11 -Iinc

all : bin/lambda

clean :
	rm -rf bin
	rm -f inc/PCH.h.gch

bin :
	mkdir -p bin

bin/lambda : bin/Lexer.o bin/Error.o bin/Values.o bin/Environment.o bin/AST.o bin/Scanner.o bin/Main.o
	$(CXX) $(CXXFLAGS) -o bin/lambda bin/Lexer.o bin/Error.o bin/Values.o bin/Environment.o bin/AST.o bin/Scanner.o bin/Main.o

inc/PCH.h.gch : inc/PCH.h
	$(CXX) $(CXXFLAGS) -o inc/PCH.h.gch inc/PCH.h

bin/Lexer.o : bin inc/PCH.h.gch inc/Lexer.h src/Lexer.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Lexer.o src/Lexer.cpp

bin/Error.o : bin inc/PCH.h.gch inc/Error.h src/Error.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Error.o src/Error.cpp

bin/Values.o : bin inc/PCH.h.gch inc/Values.h src/Values.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Values.o src/Values.cpp

bin/Environment.o : bin inc/PCH.h.gch inc/Environment.h src/Environment.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Environment.o src/Environment.cpp

bin/AST.o : bin inc/PCH.h.gch inc/AST.h src/AST.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/AST.o src/AST.cpp

bin/Scanner.o : bin inc/PCH.h.gch inc/Scanner.h src/Scanner.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Scanner.o src/Scanner.cpp

bin/Main.o : bin inc/PCH.h.gch src/Main.cpp
	$(CXX) $(CXXFLAGS) -c -o bin/Main.o src/Main.cpp
