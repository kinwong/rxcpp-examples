CXX		  := clang++
CXX_FLAGS := -Wall -Wextra -std=c++17 -g

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
VCPKG := /Users/kinwong/dev/tools/vcpkg/installed/arm64-osx/include

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -cxx-isystem$(VCPKG) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*