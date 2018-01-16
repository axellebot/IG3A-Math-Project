#---------------------------------------------------------------------
# settings
#---------------------------------------------------------------------
CFLAGS=-std=c++11 -pthread
LDFLAGS=-lGL -lGLU -lglut

SOURCES=main.cpp tools.cpp algorithm.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=math

#---------------------------------------------------------------------
# Commands
#---------------------------------------------------------------------
MD := mkdir
RM := rm -rf
CC := g++

#---------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


.PHONY: clean
clean:
	$(RM) *~ *.o $(EXECUTABLE)