# MUDUO LIBRARY PATH
MUDUODIR=/Volumes/arm-gcc/libs/boost_1_54_0
BOOSTDIR=/Volumes/arm-gcc/libs/boost_1_54_0
JSONSPIRITDIR=/Volumes/arm-gcc/libs/json_spirit_v4.06

# LINKED LIBRARIES PATH
LIBRARIES=./lib

# ALL ADDITIONAL INCLUDES
INCLUDES =-I./src -I./src/generic_classes -I./src/led_strip_controller -I./src/modes -I$(MUDUODIR) -I$(BOOSTDIR) -I$(JSONSPIRITDIR)

# ALL SOURCES
SOURCES=$(shell find . -name *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

# COMPILER SETTINGS
CXX=arm-linux-gnueabihf-g++
CXXFLAGS=-g -std=c++0x $(INCLUDES) -DMUDUO_STD_STRING -DNO_FREETYPE
LDFLAGS=-lpthread -lbass -lbass_fx -ljson_spirit -lmuduo_base -lmuduo_net -lz
BINARY=pixel-styles

# Makefile
.PHONY:
	$(BINARY) clean

all: dist-clean $(BINARY) clean

$(BINARY): $(OBJECTS)

	$(CXX) $(CXXFLAGS) -o $(BINARY) $(OBJECTS) $(LDFLAGS) -L$(LIBRARIES) -pthread

clean:
	rm -rf $(OBJECTS)

dist-clean:
	rm -rf $(OBJECTS);rm -f $(BINARY)
