#
#    _  _     __    P  R  O  J  E  C  T       _
#   FJ / ;    LJ    _ ___       ___ _      ___FJ      ____      _ _____
#   J |/ (|         J '__ J     F __` L    F __  L    F __ J    J '_  _ `,
#  |     L    FJ   | |__| |   | |--| |   | |--| |   | |--| |   | |_||_| |
#  F L:\  L  J  L  F L  J J   F L__J J   F L__J J   F L__J J   F L LJ J J
#  J__L \\__L J__L J__L  J__L  )-____  L J\____,__L J\______/F J__L LJ J__L
# |__L  \L_| |__| |__L  J__| J\______/F  J____,__F  J______F  |__L LJ J__|
# 						      J______/
#   M a k e f i l e
#

# Directory from sources files

SRC_DIR = ./src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

INC_DIR = ./inc
INC = $(wildcard $(INC_DIR)/*.h)

OBJ_DIR = ./obj
OBJ = $(patsubst %.cpp, %.o, $(subst $(SRC_DIR), $(OBJ_DIR), $(SRC)))


# Compiler options
CXX = g++
CXXFLAGS = -Wall -Wextra -g -I $(INC_DIR)
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

# Delete all
# mrproper: clean
# 	rm -f $(EXEC)
