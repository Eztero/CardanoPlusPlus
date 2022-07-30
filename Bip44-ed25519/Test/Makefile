CFLAGS:= -Wall -O2

all: bip44_test

bip44_test: bip44_test.o bip32_ed25519.o pbkdf2_hmac512_libsodium.o bech32.o bip44_ed25519.o
	$(CXX) -o bip44_test bip44_test.o bip32_ed25519.o bip44_ed25519.o pbkdf2_hmac512_libsodium.o bech32.o -std=c++11 -lsodium
	@- $(RM) *.o

bip44_test.o: bip44_test.cpp 
	$(CXX) $(CFLAGS) -c bip44_test.cpp -std=c++11
	
bip44_ed25519.o: ../bip44_ed25519.hpp ../bip44_ed25519.cpp
	$(CXX) $(CFLAGS) -c ../bip44_ed25519.cpp -std=c++11 -lsodium

bech32.o: ../../Hash/bech32.cpp ../../Hash/bech32.hpp
	$(CXX) $(CFLAGS) -c ../../Hash/bech32.cpp -std=c++11

bip32_ed25519.o: ../../Bip32-ed25519/bip32_ed25519.cpp ../../Bip32-ed25519/bip32_ed25519.hpp
	$(CXX) $(CFLAGS) -c ../../Bip32-ed25519/bip32_ed25519.cpp -std=c++11 -lsodium

pbkdf2_hmac512_libsodium.o: ../../Hash/pbkdf2_hmac512_libsodium.cpp ../../Hash/pbkdf2_hmac512_libsodium.hpp
	$(CXX) $(CFLAGS) -c ../../Hash/pbkdf2_hmac512_libsodium.cpp -std=c++11 -lsodium


clean:
	$(RM) *.o bip44_test
