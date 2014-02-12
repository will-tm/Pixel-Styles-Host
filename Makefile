# LINKED LIBRARIES PATH
LIBRARIES=./lib

# ALL ADDITIONAL INCLUDES
INCLUDES =-I./src -I./src/generic_classes -I./src/led_strip_controller -I./src/modes -I./src/sockets -I./src/settings

# ALL SOURCES
SOURCES=$(shell find . -name *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

# COMPILER SETTINGS
CXX=arm-linux-gnueabihf-g++
CXXFLAGS=-g -std=c++0x $(INCLUDES) -DMUDUO_STD_STRING -DNO_FREETYPE -pthread -lboost_system
LDFLAGS=-lpthread -lbass -lbass_fx -ljson_spirit -lmuduo_base -lmuduo_net -lz -lrt
BINARY=pixel-styles

# Makefile
.PHONY:
	$(BINARY) clean

all: dist-clean $(BINARY) clean

$(BINARY): $(OBJECTS)
	@echo " "
	@echo "Linking $(BINARY)..."
	$(CXX) $(CXXFLAGS) -o $(BINARY) $(OBJECTS) $(LDFLAGS) -L$(LIBRARIES)

clean:
	rm -rf $(OBJECTS)

dist-clean:
	rm -rf $(OBJECTS);rm -f $(BINARY)

