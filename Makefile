CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude

TARGET = task

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/DungeonReader.cpp \
          $(SRC_DIR)/Rules.cpp \
          $(SRC_DIR)/Bot.cpp \
          $(SRC_DIR)/AliceBot.cpp

OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) result.txt

.PHONY: all clean
