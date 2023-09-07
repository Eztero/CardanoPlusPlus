lib_LTLIBRARIES = libcardanoplusplus.la
libcardanoplusplus_la_SOURCES = addresses/addresses.cpp ../include/cardanoplusplus/addresses/addresses.hpp \
                                bip32_ed25519/bip32_ed25519.cpp ../include/cardanoplusplus/bip32_ed25519/bip32_ed25519.hpp \
                                bip39/bip39.cpp Bip39/bip39.hpp ../include/cardanoplusplus/bip39/diccionary.hpp \
                                bip44_ed25519/bip44_ed25519.cpp ../include/cardanoplusplus/bip44_ed25519/bip44_ed25519.hpp \
                                hash/bech32.cpp ../include/cardanoplusplus/hash/bech32.hpp \
                                hash/pbkdf2_hmac512_libsodium.cpp ../include/cardanoplusplus/hash/pbkdf2_hmac512_libsodium.hpp \
                                transaction/auxiliary_data.cpp ../include/cardanoplusplus/transaction/auxiliary_data.hpp transaction/certificates.cpp ../include/cardanoplusplus/transaction/certificates.hpp \
                                transaction/metadata.cpp ../include/cardanoplusplus/transaction/metadata.hpp transaction/multiassets.cpp ../include/cardanoplusplus/transaction/multiassets.hpp \
                                transaction/native_scripts.cpp ../include/cardanoplusplus/transaction/native_scripts.hpp transaction/transaction_body.cpp ../include/cardanoplusplus/transaction/transaction_body.hpp \
                                transaction/transaction_witness.cpp ../include/cardanoplusplus/transaction/transaction_witness.hpp transaction/transaction.cpp ../include/cardanoplusplus/transaction/transaction.hpp \
                                transaction/transactioninput.cpp ../include/cardanoplusplus/transaction/transactioninput.hpp transaction/transactionoutput.cpp ../include/cardanoplusplus/transaction/transactionoutput.hpp \
                                transaction/withdrawal.cpp ../include/cardanoplusplus/transaction/withdrawal.hpp \
                                utils/cbor_lite.cpp ../include/cardanoplusplus/utils/cbor_lite.hpp utils/plutusjsonschema.cpp ../include/cardanoplusplus/utils/plutusjsonschema.hpp utils/txutils.cpp ../include/cardanoplusplus/utils/txutils.hpp
                                
libcardanoplusplus_la_LDFLAGS = -export-dynamic --enable-shared -no-undefined $(EXTRA_FLAGS)

