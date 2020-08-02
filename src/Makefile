# This makefile is designed to work on a system with a GNU or compatible build
# system

# Compiler Specifications (Replace with desired compiler and flags)
CPP = g++
CPPSTD = c++11
CPPFLAGS = -Wall -Wextra -g -std=$(CPPSTD)

# Project Source Variables
INCLUDEDIR = . 
TESTDIR = ../tests

TESTS = simple.test sort.test

OBJFILES = Token.o Lexer.o Parser.o Exp.o Data.o Eval.o \
	   Env.o Error.o Interpreter.o Language.o Heap.o Pair.o Procedure.o main.o

# Compilation Rules

# Interpreter executable
cri-scm : $(OBJFILES)
	$(CPP) $(CPPFLAGS) $^ -o $@


test : cri-scm $(TESTS)

%.test : $(TESTDIR)/%.scm
	./cri-scm < $^

# Source File Compile
%.o : %.cpp
	$(CPP) -c $(CPPFLAGS) $^ -I $(INCLUDEDIR)


# Cleanup
clean :
	rm -f *.o cri-scm

