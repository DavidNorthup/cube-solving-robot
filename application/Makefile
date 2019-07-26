CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-lsfml-window -lsfml-graphics -lsfml-system `pkg-config --cflags --libs opencv` 
SOURCES=Driver.cpp Robot.cpp SerialCommunication.cpp Camera.cpp Options.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=csr.exe


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) *.h
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)

.cpp.o: $< *.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o *.exe

serialtest.exe: DriverSerialTest.cpp SerialCommunication.o
	g++ DriverSerialTest.cpp *.o -o serialtest.exe

imageptest.exe: DriverTestImageProcessing.cpp Camera.o Camera.cpp Camera.h
	$(CC) DriverTestImageProcessing.cpp Camera.o -o $@ $(LDFLAGS)
