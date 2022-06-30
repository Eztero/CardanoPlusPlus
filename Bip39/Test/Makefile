CFLAGS:= -Wall -O2

all: mnemotic_test

mnemotic_test: mnemotic_test.o bip39.o
	$(CXX) -o mnemotic_test mnemotic_test.o  bip39.o -std=c++11 -lsodium
	@- $(RM) *.o

mnemotic_test.o: mnemotic_test.cpp 
	$(CXX) $(CFLAGS) -c mnemotic_test.cpp -std=c++11

bip39.o: ../bip39.cpp ../bip39.hpp ../diccionary.hpp
	$(CXX) $(CFLAGS) -c ../bip39.cpp -std=c++11 -lsodium

clean:
	$(RM) *.o mnemotic_test
