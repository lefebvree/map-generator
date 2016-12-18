
# Directory from sources files

SRC_DIR = ./src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

INC_DIR = ./inc
INC = $(wildcard $(INC_DIR)/*.h)

OBJ_DIR = ./obj
OBJ = $(patsubst %.cpp, %.o, $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC)))


# Compiler options
CXX = g++
CXXFLAGS = -Wall -Wextra -g -I $(INC_DIR) -std=c++11
LDFLAGS = -lrethinkdb++

# Excutable name
EXEC = map

all: $(EXEC)

# g++ -o prog file1.o file2.o fileN.o
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# g++ -o fileN.o -c fileN.cpp fileN.h
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# Compile all
run: all
	./$(EXEC) $(seed)
	images/image_compression.sh

# Clean temp files
clean:
	rm -f $(SRC_DIR)/*~
	rm -f $(INC_DIR)/*~
	rm -f  $(OBJ_DIR)/*.o $(OBJ_DIR)/*~

