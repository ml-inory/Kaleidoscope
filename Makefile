kal : parser.cpp ast.hpp lexer.h
	clang++ -g -O3 main.cpp parser.cpp lexer.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o kal

clean:
	rm -vf kal