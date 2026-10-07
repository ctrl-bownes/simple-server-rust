CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g

SRC = 	src/main.cpp\
		src/terminal.cpp\
		src/tui.cpp\
		src/create_server.cpp\
		src/utils.cpp

LINUX_CXX = c++
WINDOWS_CXX = x86_64-w64-mingw32-g++

BIN_DIR = bin

LINUX_BIN = $(BIN_DIR)/ServerSetupApp-linux
WINDOWS_BIN = $(BIN_DIR)/ServerSetupApp-windows.exe

all: linux windows

linux: $(BIN_DIR)
	$(LINUX_CXX) $(CXXFLAGS) $(SRC) -o $(LINUX_BIN)

windows: $(BIN_DIR)
	$(WINDOWS_CXX) $(CXXFLAGS) $(SRC) -o $(WINDOWS_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

re: clean all
