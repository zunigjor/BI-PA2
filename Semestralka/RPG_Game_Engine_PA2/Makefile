.PHONY: all compile run doc clean
.DEFAULT_GOAL = all

CXX = g++
CXXFLAGS = -std=c++11 -Werror -Wall -pedantic -Wno-long-long -O0 -ggdb
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
EXECUTABLE = zunigjor
## Wildcard regex to find all source files
SOURCES := $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp)

## Create .o files from .cpp files
## https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
## $@ = Name of target rule file.
## $< = Name of first prerequisite
## $*.o = set to target name minus suffix
## -MM  = output rule for make, describes dependencies, like -M but doesnt mention headers. More in man g++
## -MT target = changes the target of the rule
## > $*.d  = typical bash output
%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c -o $@ -c $< $(LDFLAGS)
	@$(CXX) $(CXXFLAGS) -MM -MT $*.o $*.cpp $(LDFLAGS) > $*.d

all: doc compile

## Link all .o. If .o not found creates it.
compile: $(SOURCES:.cpp=.o)
	@$(CXX) $(CXXFLAGS) $(SOURCES:.cpp=.o) -o $(EXECUTABLE) $(LDFLAGS)

## run needs compile
run: compile
	@./$(EXECUTABLE)

## clean all created files
clean:
	@rm -f -- src/*.o src/*/*.o src/*/*/*.o
	@rm -f -- src/*.d src/*/*.d src/*/*/*.d
	@rm -f ./$(EXECUTABLE)
	@rm -rf -- doc/

## documentation with doxygen
doc:
	doxygen Doxyfile

## https://www.gnu.org/software/make/manual/html_node/Include.html
## If you want make to simply ignore a makefile which does not exist or cannot be remade,
## with no error message, use the -include directive instead of include, like this:
##                -include filenames…
-include $(SOURCES:.cpp=.d)