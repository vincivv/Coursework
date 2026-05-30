CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2
INCLUDES = -Iinclude
LIBS = -lpthread

OBJ = $(wildcard lib/*)

# Your application
milestone2: src/milestone2.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/milestone2.cpp $(OBJ) $(LIBS) -o milestone2

clean:
	rm -f milestone2

.PHONY: clean
