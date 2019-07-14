all: csr.exe

serialtest.exe: DriverSerialTest.cpp SerialCommunication.o
	g++ DriverSerialTest.cpp *.o -o serialtest.exe

csr.exe: Driver.cpp Robot.o SerialCommunication.o
	g++ Driver.cpp *.o -o csr.exe

Robot.o: Robot.h Robot.cpp
	g++ -c Robot.cpp

SerialCommunication.o: SerialCommunication.h SerialCommunication.cpp 
	g++ -c SerialCommunication.cpp

clean:
	rm *.o *.exe