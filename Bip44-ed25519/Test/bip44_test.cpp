#include "../bip44_ed25519.hpp"
#include <string>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]){

    if (sodium_init() < 0) {
        std::cout<<"panic! the library couldn't be initialized; it is not safe to use"<<std::endl;
        return 0;
    }

    std::uint8_t entropiaword15[32]={0x17,0x54,0xae,0xf1,0x34,0x16,0xc7,0xe9,0x2b,0x2c,0xf7,0x1b,0x63,0xb5,0x12,0x10,0xcc,0x9c,0x53,0xf0,0x10,0x64,0x38,0xa7,0x96,0x7b,0xe4,0x50,0xe8,0x72,0xd1,0xd3};

    std::uint16_t decode_len;
    std::uint32_t account = 0;
    std::uint32_t index_addr=1;

    std::string address_bech32;
    std::uint8_t child_xsk[XSK_LENGTH];
    std::uint8_t child_xvk[XVK_LENGTH];
    std::uint8_t account_child_xsk[XSK_LENGTH];
    std::uint8_t b32_decode[BECH32_MAX_LENGTH];
    std::uint8_t extended_mastersecretkey[EXTENDED_MASTERSECRETKEY_LENGTH];


    /**
--------------------   Bip44_ed25519 https://cips.cardano.org/cips/cip1852/  ------------------------

     m        /    1852' or 1854'     /    1815'     /    0'..(2^31)'-1     /      0..2       /    0...(2^31)-1 (only 0 for Role=2)
     :                :                      :                :                     :                      :
(master key)     (purpose/Wallet)        (coin type)       (account)              (Role)              (index address)
Array(uint8_t)    0=HD (1852)            (constant)        Number(uint32)         0=Extern             Number(uint32)
                  1=MultiSignHD (1854)                                            1=Intern
                                                                                  2=Staking
                                                                                  3=Only_Account
--------------------------------- Library functions --------------------------------------------------

    derive_key(master key, Wallet::HD,  Key::Private,  account, Role::Extern, index_addr, child_key_out)
raw_derive_key(master key, Wallet::HD,  Key::Private,  account, Role::Extern, index_addr, child_key_out)
                                             :
                                        0=Private(xsk)
                                        1=Public (xvk)

    derive_account_key(raw account key, AccountKeyType::Account_xvk, Wallet::HD, Key::Private, Role::Extern, index_addr, bech32_raw_child_key_out);
raw_derive_account_key(raw account key, AccountKeyType::Account_xvk, Wallet::HD, Key::Private, Role::Extern, index_addr, raw_child_key_out);
                                            :
                                        0=Account_xvk
                                        1=Account_xsk

**/


    /// Root Key(Private Key)
    raw_masterkeys_generation( entropiaword15, sizeof(entropiaword15), nullptr, 0, extended_mastersecretkey );
    std::cout<<"\nmasterkeys(privatekey): \n";
    for(uint8_t i=0;i<EXTENDED_MASTERSECRETKEY_LENGTH;i++){
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(extended_mastersecretkey[i]);
    }
    std::cout << std::endl << std::endl;




    /// RAW Private Key addr
    if(raw_derive_master_key(extended_mastersecretkey, Wallet::HD, Key::Private, account, Role::Extern, index_addr, child_xsk)){
        std::cout<<"raw_derive_key(privatekey): \n";
        for(uint8_t i=0;i<XSK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(child_xsk[i]);
        }
        std::cout << std::endl << std::endl;

    }else{
        std::cout <<"raw_derive_key(publickey): error \n" << std::endl;
    }

    /// RAW Public Key addr
    if(raw_derive_master_key(extended_mastersecretkey, Wallet::HD, Key::Public, account, Role::Extern, index_addr, child_xvk)){
        std::cout<<"raw_derive_key(publickey): \n";
        for(uint8_t i=0;i<XVK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(child_xvk[i]);
        }
        std::cout << std::endl << std::endl;
    }else{
        std::cout <<"raw_derive_key(publickey): error \n" << std::endl;
    }



    /// RAW Public key from Public Account Key

    ///        RAW Acount Private Key
    if(raw_derive_master_key(extended_mastersecretkey, Wallet::HD, Key::Private, account, Role::OnlyAccount, index_addr, account_child_xsk)){
        std::cout<<"\n\nraw_derive_key(account privatekey): \n";
        for(uint8_t i=0;i<XSK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(account_child_xsk[i]);
        }
        std::cout << std::endl << std::endl;
    }else{
        std::cout <<"raw_derive_key(account privatekey): error \n" << std::endl;
    }

    ///        RAW Account Private Key to RAW Private Key addr
    if(raw_derive_account_key(account_child_xsk, AccountKeyType::Account_xsk ,Wallet::HD,
                              Key::Private, Role::Extern, index_addr,
                              child_xsk)){

        std::cout<<"\traw_derive_account_key(privatekey): \n\t";
        for(uint8_t i=0;i<XSK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(child_xsk[i]);
        }
        std::cout << std::endl << std::endl;
    }else{
        std::cout <<"\traw_derive_account_key(privatekey): error \n" << std::endl;
    }

    ///        RAW Account Private Key to RAW Public Key addr
    if(raw_derive_account_key(account_child_xsk, AccountKeyType::Account_xsk ,Wallet::HD,
                              Key::Public, Role::Extern, index_addr,
                              child_xvk)){

        std::cout<<"\traw_derive_account_key(publickey): \n\t";
        for(uint8_t i=0;i<XVK_LENGTH;i++){
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(child_xvk[i]);
        }
        std::cout << std::endl << std::endl;
    }else{
        std::cout <<"\traw_derive_account_key(publickey): error \n" << std::endl;
    }





    /// Bech32 Private Key addr
    if(derive_master_key(extended_mastersecretkey, Wallet::HD, Key::Private, account, Role::Extern, index_addr, address_bech32)){
        std::cout <<"\n\nderive_key(privatekey): \n"<< address_bech32 << std::endl;

        /// Decode key to RAW
        if( bech32_decode(address_bech32.c_str(),b32_decode,&decode_len) ){
            std::cout<<"\n\tdecode derive_key(privatekey): \n\t";
            for(uint8_t i=0;i<decode_len;i++){
                std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b32_decode[i]);
            }
            std::cout << std::endl << std::endl;
        }else{
            std::cout <<"\n\tdecode (privatekey): error \n" << std::endl;
        }

    }

    /// Bech32 Public Key addr
    if(derive_master_key(extended_mastersecretkey, Wallet::HD, Key::Public, account, Role::Extern, index_addr, address_bech32)){
        std::cout <<"derive_key(publickey): \n"<< address_bech32 << std::endl;

        /// Decode key to RAW
        if( bech32_decode(address_bech32.c_str(),child_xsk,&decode_len) ){
            std::cout<<"\n\tdecode derive_key(publickey): \n\t";
            for(uint8_t i=0;i<decode_len;i++){
                std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(child_xsk[i]);
            }
            std::cout << std::endl << std::endl;
        }else{
            std::cout <<"\n\tdecode (publickey): error \n" << std::endl;
        }

    }



    return 0;
}
