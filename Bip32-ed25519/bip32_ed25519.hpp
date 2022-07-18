//Documentacion:
//https://github.com/typed-io/rust-ed25519-bip32
//https://cips.cardano.org/cips/cip16/
//https://input-output-hk.github.io/adrestia/cardano-wallet/concepts

#ifndef BIP32_ED25519_HPP_INCLUDED
#define BIP32_ED25519_HPP_INCLUDED

                                                       // || = concatenar
#define XSK_LENGTH 96U                                 // Extended Private Key (64 bytes) || Chain Code (32 bytes)
#define XVK_LENGTH 64U                                 // Public Key (32 bytes) || Chain Code (32 bytes)
#define EXTENDED_MASTERSECRETKEY_LENGTH 96U            // Extended Private Key (64 bytes) || Chain Code (32 bytes)

#include <cstdint>
#include <cstring>
#include <sodium.h>
#include "../hash/pbkdf2_hmac512_libsodium.hpp"

///las llaves se codificaran ahora en el formato xsk y xvk propuesto en https://cips.cardano.org/cips/cip16/

bool raw_masterkeys_generation( std::uint8_t const *const entropy, std::size_t const entropy_len, std::uint8_t const *const password, std::size_t const password_len, std::uint8_t *const extended_mastersecretkey );

bool raw_privatekey_to_publickey( std::uint8_t const *const raw_privatekey_xsk, std::uint8_t *const raw_publickey_xvk );

bool raw_child_privatekey( std::uint8_t const *const raw_parent_privatekey_xsk, std::uint32_t const index, std::uint8_t *const raw_child_privatekey_xsk );

bool raw_child_publickey( std::uint8_t const *const raw_parent_public_key_xvk, std::uint32_t const index, std::uint8_t *const raw_child_public_key_xvk );


#endif // BIP32_ED25519_HPP_INCLUDED
