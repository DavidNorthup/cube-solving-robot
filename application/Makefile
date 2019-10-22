CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-lsfml-window -lsfml-graphics -lsfml-system -lpthread `pkg-config --cflags --libs opencv4`
SOURCES=Driver.cpp Robot.cpp SerialCommunication.cpp Camera.cpp Options.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=csr.exe


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) *.h
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)

.cpp.o: $< *.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm *.o *.exe


sampling-config.exe: DriverSampling.cpp
	$(CC) DriverSampling.cpp -o sampling-config.exe $(LDFLAGS)

training-mask.exe: TrainingTestMask.cpp
		$(CC) TrainingTestMask.cpp -o training-mask.exe $(LDFLAGS)
