CFLAGS:= -Wall -O2

all: bip32_test

bip32_test: bip32_test.o bip32_ed25519.o pbkdf2_hmac512_libsodium.o
	$(CXX) -o bip32_test bip32_test.o bip32_ed25519.o pbkdf2_hmac512_libsodium.o -std=c++11 -lsodium
	@- $(RM) *.o

bip32_test.o: bip32_test.cpp 
	$(CXX) $(CFLAGS) -c bip32_test.cpp -std=c++11

bip32_ed25519.o: ../bip32_ed25519.cpp ../bip32_ed25519.hpp
	$(CXX) $(CFLAGS) -c ../bip32_ed25519.cpp -std=c++11 -lsodium

pbkdf2_hmac512_libsodium.o: ../../Hash/pbkdf2_hmac512_libsodium.cpp ../../Hash/pbkdf2_hmac512_libsodium.hpp
	$(CXX) $(CFLAGS) -c ../../Hash/pbkdf2_hmac512_libsodium.cpp -std=c++11 -lsodium

clean:
	$(RM) *.o bip32_test
