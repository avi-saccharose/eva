FLAGS = -Wall -std=c++17 -ggdb3
LD = clang++
LD_FLAGS = -o program
FILES = main.cpp eva_vm.cpp

all: compile

compile:
	$(LD) $(FLAGS) $(FILES) $(LD_FLAGS)

clean:
	rm -rf program

#clang++ -Wall main.cpp eva_vm.cpp -std=c++17 -ggdb3 ; ./a.out 
