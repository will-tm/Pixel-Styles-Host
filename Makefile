# LINKED LIBRARIES PATH
LIBRARIES=./lib

# ALL ADDITIONAL INCLUDES
INCLUDES =-I./src -I./src/generic_classes -I./src/led_strip_controller -I./src/modes -I./src/sockets -I./src/settings

# ALL SOURCES
SOURCES=$(shell find . -name *.cpp -not -path */src/modes/lib*/*)
OBJECTS=$(SOURCES:%.cpp=%.o)

# COMPILER SETTINGS
CXX=arm-linux-gnueabihf-g++
CXXFLAGS=-g -std=c++11 $(INCLUDES) -DMUDUO_STD_STRING -DNO_FREETYPE -pthread -O2
LDFLAGS=-lpthread -lbass -lbass_fx -ljson_spirit -lmuduo_base -lmuduo_net -lz -lrt -ldl
BINARY=pixel-styles

# Makefile
# Makefile
T := $(shell find . -name *.cpp -exec ls {} \;|wc -l)
N := x
C = $(words $N)$(eval N := x $N)
ECHO = echo ["`expr "   \`expr $C '*' 100 / $T\`" : '.*\(....\)$$'`%" ]

all: init $(BINARY) modes clean

init: 
	@echo "[ MAKE ] building $(BINARY)"

$(BINARY): $(OBJECTS)
	@echo "[  LD  ] linking $(BINARY)"
	@mkdir -p ./output/bin
	@$(CXX) $(CXXFLAGS) -o ./output/bin/$(BINARY) $(OBJECTS) $(LDFLAGS) -L$(LIBRARIES)
	@echo "[ MAKE ] success"

$(OBJECTS): $(SOURCES)
	@$(ECHO) $(@:%.o=%.cpp)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(@:%.o=%.cpp)

modes:
	@echo "[ MAKE ] building all modes..."
	@./tools/build_modes

clean:
	@echo "[ MAKE ] cleaning objects..."
	@rm -rf $(OBJECTS)

dist-clean:
	@echo "[ MAKE ] cleaning objects..."
	@rm -rf $(OBJECTS)
	@echo "[ MAKE ] cleaning binaries..."
	@rm -rf ./output

install:
	@mkdir -p /etc/pixel_styles/modes-available
	@mkdir -p /etc/pixel_styles/modes-enabled
	@mkdir -p /etc/pixel_styles/settings
	install -m 0755 ./output/bin/$(BINARY) /usr/bin
	install -m 0755 ./tools/psenmod /usr/bin
	install -m 0755 ./tools/psdismod /usr/bin
	install -m 0644 ./output/lib/*.so /etc/pixel_styles/modes-available
	install -m 0644 ./tools/config.cfg /etc/pixel_styles