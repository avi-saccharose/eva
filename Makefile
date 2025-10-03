FLAGS = -Wall -std=c++20 -ggdb3
LD = clang++
LD_FLAGS = -o program
FILES = main.cpp vm/eva_vm.cpp parser/lexer.cpp parser/parser.cpp parser/ast_printer.cpp

all: compile

compile:
	$(LD) $(FLAGS) $(FILES) $(LD_FLAGS)

clean:
	rm -rf program

#clang++ -Wall main.cpp eva_vm.cpp -std=c++17 -ggdb3 ; ./a.out 
