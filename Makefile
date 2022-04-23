kal : lexer.cpp scanner.cpp ast.hpp scanner.h
	g++ -o kal main.cpp lexer.cpp scanner.cpp

clean:
	rm -vf kal