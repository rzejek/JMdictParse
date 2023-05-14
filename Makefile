CXXFLAGS=-g `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0` -lxerces-c
OBJ_DIR  := ./obj
TARGET   := parse
TARGET2   := transform
CXX=g++

SRC=$(wildcard *.cpp)
SRC2 := $(filter-out Main.cpp, $(SRC))
SRC := $(filter-out ShortTransform.cpp, $(SRC))
OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS2  := $(SRC2:%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o bin/$(TARGET) $^ $(LDFLAGS)

transform: $(OBJECTS2)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o bin/$(TARGET2) $^ $(LDFLAGS)
