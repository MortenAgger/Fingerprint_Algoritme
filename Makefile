# ==========================
# Shazam Project Makefile
# ==========================

CXX = clang++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pedantic
INCLUDES = -Iinclude -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lfftw3 -lsndfile \
        -framework Accelerate -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC_DIR = src
OBJ_DIR = build
BIN = $(OBJ_DIR)/outDebug

# Alle .cpp filer i src/ inkluderes automatisk
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(OBJ) -o $(BIN) $(LIBS)
	@echo "Build complete: $(BIN)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN)
	@echo "Project cleaned"
