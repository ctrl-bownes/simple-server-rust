CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g

SRC =	src/main.cpp \
		src/terminal.cpp \
		src/tui.cpp \
		src/create_server.cpp \
		src/utils.cpp

LINUX_CXX = c++
WINDOWS_CXX = x86_64-w64-mingw32-g++

BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

LINUX_OBJ_DIR = $(OBJ_DIR)/linux
WINDOWS_OBJ_DIR = $(OBJ_DIR)/windows

LINUX_BIN = $(BIN_DIR)/ServerSetupApp-linux
WINDOWS_BIN = $(BIN_DIR)/ServerSetupApp-windows.exe

LINUX_OBJ = $(SRC:src/%.cpp=$(LINUX_OBJ_DIR)/%.o)
WINDOWS_OBJ = $(SRC:src/%.cpp=$(WINDOWS_OBJ_DIR)/%.o)

all: linux windows

linux: $(LINUX_BIN)

windows: $(WINDOWS_BIN)

$(LINUX_BIN): $(LINUX_OBJ) | $(BIN_DIR)
	$(LINUX_CXX) $(CXXFLAGS) $(LINUX_OBJ) -o $@

$(WINDOWS_BIN): $(WINDOWS_OBJ) | $(BIN_DIR)
	$(WINDOWS_CXX) $(CXXFLAGS) $(WINDOWS_OBJ) -o $@

$(LINUX_OBJ_DIR)/%.o: src/%.cpp | $(LINUX_OBJ_DIR)
	$(LINUX_CXX) $(CXXFLAGS) -c $< -o $@

$(WINDOWS_OBJ_DIR)/%.o: src/%.cpp | $(WINDOWS_OBJ_DIR)
	$(WINDOWS_CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(LINUX_OBJ_DIR) $(WINDOWS_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)

re: clean all
