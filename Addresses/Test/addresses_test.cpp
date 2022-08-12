#include "../../Bip44-ed25519/bip44_ed25519.hpp"
#include "../addresses.hpp"
#include <string>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]){

    if (sodium_init() < 0) {
        std::cout << "panic! the library couldn't be initialized; it is not safe to use"<<std::endl;
        return 0;
    }

    std::uint8_t entropiaword15[32] = {0x17,0x54,0xae,0xf1,0x34,0x16,0xc7,0xe9,0x2b,0x2c,0xf7,0x1b,0x63,0xb5,0x12,0x10,0xcc,0x9c,0x53,0xf0,0x10,0x64,0x38,0xa7,0x96,0x7b,0xe4,0x50,0xe8,0x72,0xd1,0xd3};

    std::uint8_t output_keyhash_len = 0;
    std::uint32_t account = 0;
    std::uint32_t index_addr = 0;

    std::string address_bech32;
    std::string address_out;

    std::uint8_t privatekey_account[XVK_LENGTH];
    std::uint8_t extended_mastersecretkey[EXTENDED_MASTERSECRETKEY_LENGTH];
    std::uint8_t output_keyhash[KEYHASH_LENGTH_MAX];


    /// Root Key(Private Key)
    raw_masterkeys_generation( entropiaword15, sizeof(entropiaword15), nullptr, 0, extended_mastersecretkey );
    std::cout << "\nmasterkeys(private key): \n";
    for(uint8_t i = 0; i < EXTENDED_MASTERSECRETKEY_LENGTH; i++){
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(extended_mastersecretkey[i]);
    }
    std::cout << std::endl << std::endl;


    /// Enterprise Address from Public Account Key

    ///        RAW Acount Private Key

    if(!raw_derivekey(extended_mastersecretkey, InputKey::ExtendedMasterKey, Wallet::HD, OutputKey::Public, 0, Role::OnlyAccount, privatekey_account)){
        std::cout << "\n RAW Acount Private Key: Error \n" << std::endl;
    }



    ///        Enterprise Address from Raw Account Private Key

    if(createaddress(privatekey_account, InputKey::AccountKey_xvk, Network::Mainnet, Wallet::HD, Address::Enterprise,
                     account, index_addr, address_out)){
        std::cout << "\n Mainnet Enterprise address from account(public key): \n" << address_out << std::endl;
    }



    /// Enterprise Address from Master Key

    if(createaddress(extended_mastersecretkey, InputKey::ExtendedMasterKey, Network::Mainnet, Wallet::HD, Address::Enterprise,
                     account, index_addr, address_out)){
        std::cout << "\n Mainnet Enterprise address from master key: \n" << address_out << std::endl;
    }



    /// Enterprise Address Key Hash from Master Key

    if(raw_createaddress_keyhash(extended_mastersecretkey, InputKey::ExtendedMasterKey, Network::Mainnet, Wallet::HD, Address::Enterprise,
                                 account, index_addr, output_keyhash, &output_keyhash_len)){
        std::cout << "\n RAW Mainnet Enterprise keyhash from master key: \n";
        for(uint8_t i = 0; i < output_keyhash_len; i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(output_keyhash[i]);
        }
        std::cout << std::endl << std::endl;
    }

    return 0;
}
