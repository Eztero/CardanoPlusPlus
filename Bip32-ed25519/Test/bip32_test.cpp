
#include "../../Hash/pbkdf2_hmac512_libsodium.hpp"
#include "../bip32_ed25519.hpp"
#include <string>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]){

    if (sodium_init() < 0) {
        std::cout<<"panic! the library couldn't be initialized; it is not safe to use"<<std::endl;
        return 0;
    }

    std::uint8_t entropiaword15[32]={0x17,0x54,0xae,0xf1,0x34,0x16,0xc7,0xe9,0x2b,0x2c,0xf7,0x1b,0x63,0xb5,0x12,0x10,0xcc,0x9c,0x53,0xf0,0x10,0x64,0x38,0xa7,0x96,0x7b,0xe4,0x50,0xe8,0x72,0xd1,0xd3};
    std::uint8_t extended_mastersecretkey[EXTENDED_MASTERSECRETKEY_LENGTH];
    std::uint8_t skey_out[XSK_LENGTH];
    std::uint8_t vkey_out[XVK_LENGTH];
    std::uint8_t vkey_out1[XVK_LENGTH];
    std::uint64_t index_n = 2147483647;


    /// ROOT KEY(PRIVATE KEY)

    raw_masterkeys_generation( entropiaword15, 20, nullptr, 0, extended_mastersecretkey );
    std::cout<<"\nmasterkeys(privatekey): \n";
    for(uint8_t i=0;i<EXTENDED_MASTERSECRETKEY_LENGTH;i++){
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(extended_mastersecretkey[i]);
    }
    std::cout << std::endl << std::endl;

    /// PRIVATE KEY TO PUBLIC KEY

    if( raw_privatekey_to_publickey( extended_mastersecretkey, vkey_out ) ){
        std::cout << "masterkey(privatekey) to masterkey(publickey): \n";
        for(uint8_t i = 0 ; i < XVK_LENGTH; i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(vkey_out[i]);
        }
        std::cout << std::endl << std::endl;
    }
    else {
        std::cout <<"masterkey(privatekey) to masterkey(publickey): error \n" << std::endl;
    }

    /// CHILD PRIVATE KEY

    if( raw_child_privatekey( extended_mastersecretkey,index_n, skey_out ) ){

        std::cout << "masterkey(privatekey) to child privatekey (m/" << std::dec << index_n << "): \n";
        for(uint8_t i = 0; i < XSK_LENGTH; i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(skey_out[i]);
        }
        std::cout << std::endl << std::endl;

        /// CONVERT TO PUBLIC

        if( raw_privatekey_to_publickey( skey_out, vkey_out1 ) ){
            std::cout << "child privatekey (m/" << std::dec << index_n << ") to publickey: \n";
            for(uint8_t i = 0 ; i < XVK_LENGTH; i++){
                std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(vkey_out1[i]);
            }
            std::cout << std::endl << std::endl;
        }
        else {
            std::cout <<"child privatekey (m/" << std::dec << index_n << ") to publickey: error \n" << std::endl;
        }

    }
    else {
        std::cout <<"masterkey(privatekey) to child privatekey(m/" << std::dec << index_n << "): error \n" << std::endl;
    }

    /// CHILD PUBLIC KEY

    if (raw_child_publickey(vkey_out, index_n, vkey_out1)){
        std::cout << "masterkey(publickey) to child publickey (m/" << std::dec << index_n << "): \n";
        for(uint8_t i=0;i<XVK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(vkey_out1[i]);
        }
        std::cout << std::endl << std::endl;
    }else {
        std::cout <<"masterkey(publickey) to child publickey(m/" << std::dec << index_n << "): error\n" << std::endl;
    }

    /// SIGNATURE

    std::uint8_t mensage[]{"hola que hace"};
    std::uint8_t sign[SIGNATURE_LENGTH];
    signature(skey_out, mensage , sizeof(mensage)-1, sign);
    std::cout << "signature(m = "<<mensage<<", len = "<< std::dec <<sizeof(mensage)-1<<") with child privatekey (m/" << std::dec << index_n << "):\n";
    for(std::uint8_t i = 0; i<64; i++){
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(sign[i]);
    }
    std::cout << std::endl << std::endl;

    /// VERIFY

    std::cout << "verifying signature with child publickey (m/" << std::dec << index_n << "):\n";
    if(verify(vkey_out1, mensage , sizeof(mensage)-1, sign)){
        std::cout << "valid signature"<<std::endl;
    }
    else{
        std::cout << "invalid  signature"<<std::endl;
    }

    return 0;
}
