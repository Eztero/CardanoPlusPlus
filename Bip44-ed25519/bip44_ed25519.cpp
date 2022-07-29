#include "bip44_ed25519.hpp"

bool raw_derive_key(std::uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                    std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                    std::uint8_t *const child_key_out){

    std::memset(child_key_out,0,XVK_LENGTH); //se deja a cero los primeros 64 bytes, asi en caso de un error su sk o xvk seran cero
    int l_key=0;

    if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
        return false;
    }

    std::uint32_t index_addr = 0;
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves
    std::uint32_t index_acc = account_path + H0;

    //------ Derivacion----

    switch(wallet_type){
    case Wallet::HD:{
        if (!raw_child_privatekey(extended_master_secret_key, H1852, buff_xsk)){  /// m/1852'
            return false;
        }
    };break;
    case Wallet::MultiSignHD:{
        if (!raw_child_privatekey(extended_master_secret_key, H1854, buff_xsk)){  /// m/1854'
            return false;
        }
    };break;

    }

    if(!raw_child_privatekey(buff_xsk, H1815, buff_xsk)){                     /// m/1852'/1815'
        return false;
    }

    if(!raw_child_privatekey(buff_xsk, index_acc, buff_xsk)){                 /// m/1852'/1815'/account'
        return false;
    }

    switch (key_type){

    case Key::Private:{
        l_key = XSK_LENGTH;
        if(role_path != Role::Only_Account){

            switch(wallet_type){

            case Wallet::HD:{
                if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(role_path !=Role::Intern){
                    if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(address_index_path < 2147483648U){

                if(role_path != Role::Staking){ //En caso de detectar una direccion stake el index_addr=0
                    index_addr = address_index_path;
                }
                else{
                    index_addr = 0;
                }
                if(!raw_child_privatekey(buff_xsk, index_addr, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                    return false;
                }
                if(!valid_ed25519_sk(buff_xsk)){
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }

            }else{
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }

    };break;


    case Key::Public:{
        l_key = XVK_LENGTH;
        if(!raw_privatekey_to_publickey(buff_xsk, buff_xsk)){
            return false;
        }

        if(role_path != Role::Only_Account){

            switch(wallet_type){

            case Wallet::HD:{
                if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(role_path !=Role::Intern){
                    if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(address_index_path < 2147483648U){

                if(role_path != Role::Staking){   //En caso de detectar una direccion stake el index_addr=0
                    index_addr = address_index_path;
                }
                else{
                    index_addr = 0;
                }
                if(!raw_child_publickey(buff_xsk, index_addr, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    return false;
                }

            }else {
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }
    };break;

    default:{
        return false;
    };break;

    }

    //----------------

    for(std::uint8_t i = 0; i<l_key; i++){
        child_key_out[i] = buff_xsk[i];
    }
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}


bool derive_key(std::uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                std::string& child_key_address_out){

    child_key_address_out.clear(); //se deja en cero el string

    if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
        return false;
    }

    int l_key=0;
    char hrp[17]; //soporta el prefijo mas largo
    std::uint32_t index_addr = 0;
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves
    std::uint32_t index_acc = account_path + H0;

    //------ Derivacion----

    switch(wallet_type){
    case Wallet::HD:{
        if (!raw_child_privatekey(extended_master_secret_key, H1852, buff_xsk)){  /// m/1852'
            return false;
        }
    };break;
    case Wallet::MultiSignHD:{
        if (!raw_child_privatekey(extended_master_secret_key, H1854, buff_xsk)){  /// m/1854'
            return false;
        }
    };break;

    }

    if(!raw_child_privatekey(buff_xsk, H1815, buff_xsk)){                     /// m/1852'/1815'
        return false;
    }

    if(!raw_child_privatekey(buff_xsk, index_acc, buff_xsk)){                 /// m/1852'/1815'/account'
        return false;
    }

    switch (key_type){

    case Key::Private:{
        l_key = XSK_LENGTH;
        if(role_path != Role::Only_Account){

            switch(wallet_type){

            case Wallet::HD:{
                if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(role_path !=Role::Intern){
                    if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(address_index_path < 2147483648U){

                if(role_path != Role::Staking){ //En caso de detectar una direccion stake el index_addr=0
                    index_addr = address_index_path;
                }
                else{
                    index_addr = 0;
                }
                if(!raw_child_privatekey(buff_xsk, index_addr, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                    return false;
                }
                if(!valid_ed25519_sk(buff_xsk)){
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }

            }else{
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }

    };break;


    case Key::Public:{
        l_key = XVK_LENGTH;
        if(!raw_privatekey_to_publickey(buff_xsk, buff_xsk)){
            return false;
        }

        if(role_path != Role::Only_Account){

            switch(wallet_type){

            case Wallet::HD:{
                if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(role_path !=Role::Intern){
                    if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(address_index_path < 2147483648U){

                if(role_path != Role::Staking){   //En caso de detectar una direccion stake el index_addr=0
                    index_addr = address_index_path;
                }
                else{
                    index_addr = 0;
                }
                if(!raw_child_publickey(buff_xsk, index_addr, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    return false;
                }

            }else {
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }
    };break;

    default:{
        return false;
    };break;

    }

    //------- Hrp ------
    if(role_path != Role::Only_Account){
        if(role_path != Role::Staking){
            std::strcpy (hrp,"addr"); ///prefix
        }else{
            std::strcpy (hrp,"stake"); ///prefix
        }
    }
    else{
        std::strcpy (hrp,"acct"); ///prefix
    }
    if(wallet_type == Wallet::MultiSignHD){
        std::strcat (hrp,"_shared"); ///prefix
    }
    if(l_key == XVK_LENGTH){
        std::strcat (hrp,"_xvk"); ///prefix

    }else{
        std::strcat (hrp,"_xsk"); ///prefix
    }
    //---------------

    bech32_encode(hrp,buff_xsk,l_key,child_key_address_out);
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}
