CXX = g++
EXEC = solarsystem
SOURCES = main.cpp solarsystem.cpp stars.cpp
OBJECTS = main.o  solarsystem.o stars.o
LDFLAGS = -lglut -lGL -lGLU

all :
	$(CXX) $(SOURCES) $(LDFLAGS) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o