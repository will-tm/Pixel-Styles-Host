# LINKED LIBRARIES PATH
LIBRARIES=./lib

# ALL ADDITIONAL INCLUDES
INCLUDES =-I./src -I./src/generic_classes -I./src/led_strip_controller -I./src/modes -I./src/sockets -I./src/settings

# ALL SOURCES
SOURCES=$(shell find . -name *.cpp -not -path */src/modes/lib/*)
OBJECTS=$(SOURCES:%.cpp=%.o)

# COMPILER SETTINGS
CXX=arm-linux-gnueabihf-g++
CXXFLAGS=-g -std=c++11 $(INCLUDES) -DMUDUO_STD_STRING -DNO_FREETYPE -pthread -O2
LDFLAGS=-lpthread -lbass -lbass_fx -ljson_spirit -lmuduo_base -lmuduo_net -lz -lrt -ldl
BINARY=pixel-styles

# Makefile
.PHONY:
	$(BINARY) clean

all: dist-clean $(BINARY) modes clean

$(BINARY): $(OBJECTS)
	@echo " "
	@echo "Linking $(BINARY)..."
	@mkdir -p ./output/bin
	@$(CXX) $(CXXFLAGS) -o ./output/bin/$(BINARY) $(OBJECTS) $(LDFLAGS) -L$(LIBRARIES)

modes:
	@echo " "
	@echo "Building all modes..."
	@./build_modes

clean:
	@echo " "
	@echo "Cleaning objects..."
	@rm -rf $(OBJECTS)

dist-clean:
	@echo " "
	@echo "Cleaning objects..."
	@rm -rf $(OBJECTS)
	@echo "Cleaning binaries..."
	@rm -rf ./output

install:
	@mkdir -p /etc/pixel_styles/modes-available
	@mkdir -p /etc/pixel_styles/modes-enabled
	@mkdir -p /etc/pixel_styles/settings
	install -m 0755 ./output/bin/$(BINARY) /usr/bin
    install -m 0644 ./output/lib/*.so /etc/pixel_styles/modes-available
