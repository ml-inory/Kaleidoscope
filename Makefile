kal : parser.cpp ast.hpp lexer.h
	g++ -o kal main.cpp parser.cpp lexer.cpp

clean:
	rm -vf kal