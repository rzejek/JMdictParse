CXXFLAGS=-g `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0` -lxerces-c
LDFLAGS2=`pkg-config --libs gtk+-3.0` -lxerces-c
LDFLAGS3=-lxerces-c -lsqlite3
OBJ_DIR  := ./obj
TARGET1  := parse
TARGET2  := transform
TARGET3  := import
CXX=g++

SRC=$(wildcard *.cpp)
SRC2:=$(filter-out Main.cpp Anki2Import.cpp, $(SRC))
SRC3:=$(filter-out Main.cpp ShortTransform.cpp, $(SRC))
SRC1:=$(filter-out ShortTransform.cpp Anki2Import.cpp, $(SRC))
OBJECTS1 := $(SRC1:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS2 := $(SRC2:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS3 := $(SRC3:%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET1)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

parse: $(OBJECTS1)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o bin/$(TARGET1) $^ $(LDFLAGS)

transform: $(OBJECTS2)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS2) -o bin/$(TARGET2) $^ $(LDFLAGS2)

import: $(OBJECTS3)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS3) -o bin/$(TARGET3) $^ $(LDFLAGS3)
