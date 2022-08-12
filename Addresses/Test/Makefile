CFLAGS:= -Wall -O2

all: addresses_test

addresses_test: addresses_test.o addresses.o bip32_ed25519.o pbkdf2_hmac512_libsodium.o bech32.o bip44_ed25519.o
	$(CXX) -o addresses_test addresses_test.o addresses.o bip32_ed25519.o bip44_ed25519.o pbkdf2_hmac512_libsodium.o bech32.o -std=c++11 -lsodium
	@- $(RM) *.o

addresses_test.o: addresses_test.cpp 
	$(CXX) $(CFLAGS) -c addresses_test.cpp -std=c++11

addresses.o: ../addresses.cpp ../addresses.hpp
	$(CXX) $(CFLAGS) -c ../addresses.cpp -std=c++11 -lsodium

bip44_ed25519.o: ../../Bip44-ed25519/bip44_ed25519.hpp ../../Bip44-ed25519/bip44_ed25519.cpp
	$(CXX) $(CFLAGS) -c ../../Bip44-ed25519/bip44_ed25519.cpp -std=c++11 -lsodium

bech32.o: ../../Hash/bech32.cpp ../../Hash/bech32.hpp
	$(CXX) $(CFLAGS) -c ../../Hash/bech32.cpp -std=c++11

bip32_ed25519.o: ../../Bip32-ed25519/bip32_ed25519.cpp ../../Bip32-ed25519/bip32_ed25519.hpp
	$(CXX) $(CFLAGS) -c ../../Bip32-ed25519/bip32_ed25519.cpp -std=c++11 -lsodium

pbkdf2_hmac512_libsodium.o: ../../Hash/pbkdf2_hmac512_libsodium.cpp ../../Hash/pbkdf2_hmac512_libsodium.hpp
	$(CXX) $(CFLAGS) -c ../../Hash/pbkdf2_hmac512_libsodium.cpp -std=c++11 -lsodium


clean:
	$(RM) *.o addresses_test
