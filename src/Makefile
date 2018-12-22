# This makefile is designed to work on a system with a GNU or compatible build
# system

# Compiler Specifications (Replace with desired compiler and flags)
CPP = g++
CPPSTD = c++11
CPPFLAGS = -Wall -Wextra -g -std=$(CPPSTD)

# Project Source Variables
INCLUDEDIR = ../include
TESTDIR = ../tests

OBJFILES = main.o Token.o Lexer.o

# Compilation Rules

# Interpreter executable
cri-scm : $(OBJFILES)
	$(CPP) $(CPPFLAGS) $^ -o $@


# Source File Compile
%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -I $(INCLUDEDIR)


# Cleanup
clean :
	rm *.o cri-scm

