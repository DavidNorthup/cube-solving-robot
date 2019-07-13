

all: csr.exe


csr.exe: Driver.cpp SerialCommunication.o
	g++ Driver.cpp *.o -o csr.exe

SerialCommunication.o: SerialCommunication.h SerialCommunication.cpp
	g++ -c SerialCommunication.cpp

clean:
	rm *.o *.exe