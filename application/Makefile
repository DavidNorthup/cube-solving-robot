CC=g++
CFLAGS=-c -Wall
LDFLAGS=`pkg-config --cflags --libs opencv`
SOURCES=Driver.cpp Robot.cpp SerialCommunication.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=csr.exe


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o *.exe

serialtest.exe: DriverSerialTest.cpp SerialCommunication.o
	g++ DriverSerialTest.cpp *.o -o serialtest.exe