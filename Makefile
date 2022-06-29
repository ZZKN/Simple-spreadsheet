SOURCE_DIR = src
BIN = knapozu1
CXX = g++
RM = rm
CXXFLAGS = -std=c++17 -pedantic -Wall -Wno-long-long 

.PHONY: all clean

all: main.o Cell.o TextCell.o Function.o Leaf.o Operator.o TermCell.o Sheet.o SheetController.o Overhead.o

main.o: $(SOURCE_DIR)/main.cpp
	@echo Building main.o
	@$(CXX) -c -o main.o $(CXXFLAGS) $^
	@echo Done

Cell.o: $(SOURCE_DIR)/Cell.cpp
	@echo Building Cell.o
	@$(CXX) -c -o Cell.o $(CXXFLAGS) $^
	@echo Done

TextCell.o: $(SOURCE_DIR)/TextCell.cpp
	@echo Building TextCell.o
	@$(CXX) -c -o TextCell.o $(CXXFLAGS) $^
	@echo Done

Operator.o: $(SOURCE_DIR)/Operator.cpp
	@echo Building Operator.o
	@$(CXX) -c -o Operator.o $(CXXFLAGS) $^
	@echo Done

Overhead.o: $(SOURCE_DIR)/Overhead.cpp
	@echo Building Overhead.o
	@$(CXX) -c -o Overhead.o $(CXXFLAGS) $^
	@echo Done

Function.o: $(SOURCE_DIR)/Function.cpp
	@echo Building Function.o
	@$(CXX) -c -o Function.o $(CXXFLAGS) $^
	@echo Done

Leaf.o: $(SOURCE_DIR)/Leaf.cpp
	@echo Building Leaf.o
	@$(CXX) -c -o Leaf.o $(CXXFLAGS) $^
	@echo Done
	
	
TermCell.o: $(SOURCE_DIR)/TermCell.cpp
	@echo Building TermCell.o
	@$(CXX) -c -o TermCell.o $(CXXFLAGS) $^
	@echo Done

Sheet.o: $(SOURCE_DIR)/Sheet.cpp
	@echo Building Sheet.o
	@$(CXX) -c -o Sheet.o $(CXXFLAGS) $^
	@echo Done

SheetController.o: $(SOURCE_DIR)/SheetController.cpp
	@echo Building SheetController.o
	@$(CXX) -c -o SheetController.o $(CXXFLAGS) $^
	@echo Done

compile: all
	@echo Building executable
	@$(CXX) $(LIB) -o $(BIN) main.o Cell.o TextCell.o Function.o Leaf.o Operator.o  TermCell.o Sheet.o SheetController.o Overhead.o
	@echo Done

clean:
	@echo Cleaning objects...
	@rm *.o $(BIN)
	@rm -rf doc
	@echo Done cleaning

run:
	@./$(BIN)

doc:
	doxygen doxyfile.conf
