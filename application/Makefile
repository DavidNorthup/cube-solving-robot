CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lsfml-window -lsfml-graphics -lsfml-system -pthread `pkg-config --cflags --libs opencv` 
SOURCES=Driver.cpp Robot.cpp SerialCommunication.cpp Camera.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=csr.exe


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o *.exe

serialtest.exe: DriverSerialTest.cpp SerialCommunication.o
	g++ DriverSerialTest.cpp *.o -o serialtest.exe
