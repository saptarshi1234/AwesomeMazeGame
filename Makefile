CXX 				:= g++
CXXFLAGS		:= -std=c++17 -Wall -Wextra -g
LFLAGS 			:= -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
OUTPUT			:= output
BUILD 			:= build
SRC					:= src
INCLUDE			:= include
LIB					:= lib

MAIN				:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS			:= $(shell find $(LIB) -type d)
FIXPATH 		:= $1
RM 					:= rm -f
MD					:= mkdir -p

INCLUDES		:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS				:= $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES			:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
OBJECTS			:= $(addprefix $(BUILD)/, $(SOURCES:.cpp=.o))

OUTPUTMAIN := $(OUTPUT)/$(MAIN)

all: $(OUTPUT) $(BUILD)/$(SOURCEDIRS) $(MAIN) 
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(BUILD)/$(SOURCEDIRS):
	$(MD) $(BUILD)/$(SOURCEDIRS)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS)   

$(BUILD)/%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(OBJECTS)
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!