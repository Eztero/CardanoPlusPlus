
#ifndef ADDRESSES_HPP
#define ADDRESSES_HPP

#define BLAKE224_LENGTH 28U
#define BLAKE256_LENGTH 32U
#define STAKE_INDEX 0
#define KEYHASH_LENGTH_MAX 57U

#include "../Bip44-ed25519/bip44_ed25519.hpp"

enum class Network{
Testnet,
Mainnet
};

enum class Address{
Base,
Base_Change,
Enterprise,
Enterprise_Change,
Stake
};

bool createaddress(std::uint8_t const *const input_key, InputKey const input_key_type, Network const network_id, Wallet const wallet_type, Address const address_type,
                   std::uint32_t const account_path, std::uint32_t const address_index_path,
                   std::string& address_out);

bool raw_createaddress_keyhash(std::uint8_t const *const input_key, InputKey const input_key_type, Network const network_id, Wallet const wallet_type, Address const address_type,
                               std::uint32_t const account_path, std::uint32_t const address_index_path,
                               std::uint8_t *const output_keyhash,std::uint8_t *const output_keyhash_len);

#endif // ADDRESSES_HPP
