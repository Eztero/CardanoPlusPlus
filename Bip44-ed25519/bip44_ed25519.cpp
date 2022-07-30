#include "bip44_ed25519.hpp"

static bool from_masterkey(std::uint8_t const *const extended_master_secret_key, Wallet const *const wallet_type, Key const *const key_type, Role const *const role_path,
                    std::uint32_t const *const index_acc, int *const l_key, std::uint32_t const *const address_index_path,
                    std::uint8_t *const buff_xsk){

    switch(*wallet_type){
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

    if(!raw_child_privatekey(buff_xsk, *index_acc, buff_xsk)){                 /// m/1852'/1815'/account'
        return false;
    }

    switch (*key_type){

    case Key::Private:{
        *l_key = XSK_LENGTH;
        if(*role_path != Role::OnlyAccount){

            switch(*wallet_type){

            case Wallet::HD:{
                if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(*role_path !=Role::Intern){
                    if(!raw_child_privatekey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(*role_path == Role::Staking && address_index_path > 0){ //En caso de detectar una direccion stake el index_addr=0
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }

                if(!raw_child_privatekey(buff_xsk, *address_index_path, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                    sodium_memzero(buff_xsk, XSK_LENGTH);
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
        *l_key = XVK_LENGTH;
        if(!raw_privatekey_to_publickey(buff_xsk, buff_xsk)){
            return false;
        }

        if(*role_path != Role::OnlyAccount){

            switch(*wallet_type){

            case Wallet::HD:{
                if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(*role_path !=Role::Intern){
                    if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(*role_path == Role::Staking && *address_index_path > 0){ //En caso de detectar una direccion stake el index_addr=0
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
                if(!raw_child_publickey(buff_xsk, *address_index_path, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    sodium_memzero(buff_xsk, XSK_LENGTH);
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

    return true;
}


static bool from_accountkey(std::uint8_t const *const account_key, AccountKeyType const *const account_key_type, Wallet const *const wallet_type,Key const *const key_type, Role const *const role_path,
                     int *const l_key, std::uint32_t const *const address_index_path,
                     std::uint8_t *const buff_xsk){

    switch (*key_type){

    case Key::Private:{
        if(*account_key_type == AccountKeyType::Account_xsk){
            *l_key = XSK_LENGTH;
            if(*role_path != Role::OnlyAccount){

                switch(*wallet_type){

                case Wallet::HD:{
                    if(!raw_child_privatekey(account_key, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                };break;

                case Wallet::MultiSignHD:{
                    if(*role_path !=Role::Intern){
                        if(!raw_child_privatekey(account_key, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                            return false;
                        }
                    }else{
                        return false;
                    }
                };break;

                }

                if(*address_index_path < 2147483648U){

                    if(*role_path == Role::Staking && *address_index_path > 0){ //En caso de detectar una direccion stake el index_addr=0
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                    }
                    if(!raw_child_privatekey(buff_xsk, *address_index_path, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                        sodium_memzero(buff_xsk, XSK_LENGTH);
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
            }else{
                return false;
            }
        }else{
            return false;
        }
    };break;


    case Key::Public:{
        *l_key = XVK_LENGTH;

        if(*account_key_type == AccountKeyType::Account_xsk){
            if(!raw_privatekey_to_publickey(account_key, buff_xsk)){
                return false;
            }
        }else{
            std::memcpy(buff_xsk, account_key, XVK_LENGTH);
        }

        if(*role_path != Role::OnlyAccount){

            switch(*wallet_type){

            case Wallet::HD:{
                if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                    return false;
                }
            };break;

            case Wallet::MultiSignHD:{
                if(*role_path !=Role::Intern){
                    if(!raw_child_publickey(buff_xsk, static_cast<std::uint32_t>(*role_path), buff_xsk)){  /// m/1852'/1815'/account'/role_path
                        return false;
                    }
                }else{
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(*role_path == Role::Staking && *address_index_path > 0){ //En caso de detectar una direccion stake el index_addr=0
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }
                if(!raw_child_publickey(buff_xsk, *address_index_path, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    sodium_memzero(buff_xsk, XSK_LENGTH);
                    return false;
                }

            }else {
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }else{
            return false;
        }
    };break;

    default:{
        return false;
    };break;

    }

 return true;
}


bool raw_derive_master_key(std::uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                           std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                           std::uint8_t *const child_key_out){

    std::memset(child_key_out,0,XVK_LENGTH); //se deja a cero los primeros 64 bytes, asi en caso de un error su sk o xvk seran cero

    int l_key=0;
    if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
        return false;
    }

    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves
    std::uint32_t index_acc = account_path + H0;

    //------ Derivacion----

    if(!from_masterkey(extended_master_secret_key, &wallet_type, &key_type, &role_path, &index_acc, &l_key, &address_index_path, buff_xsk)){
    return false;
    }
    //----------------

    for(std::uint8_t i = 0; i<l_key; i++){
        child_key_out[i] = buff_xsk[i];
    }
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}


bool derive_master_key(std::uint8_t const *const extended_master_secret_key, Wallet wallet_type, Key key_type,
                std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                std::string& bech32_child_key_out){

    bech32_child_key_out.clear(); //se deja en cero el string

    if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
        return false;
    }

    int l_key=0;
    char hrp[17]; //soporta el prefijo mas largo
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves
    std::uint32_t index_acc = account_path + H0;

    //------ Derivacion----

  if(!from_masterkey(extended_master_secret_key, &wallet_type, &key_type, &role_path, &index_acc, &l_key, &address_index_path, buff_xsk)){
    return false;
    }
    //------- Hrp ------
    if(role_path != Role::OnlyAccount){
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

    bech32_encode(hrp,buff_xsk,l_key,bech32_child_key_out);
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}


bool raw_derive_account_key(std::uint8_t const *const account_key, AccountKeyType account_key_type, Wallet wallet_type,
                            Key key_type, Role role_path, std::uint32_t const address_index_path,
                            std::uint8_t *const child_key_out){

    std::memset(child_key_out,0,XVK_LENGTH); //se deja a cero los primeros 64 bytes, asi en caso de un error su sk o xvk seran cero

    int l_key=0;
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves


    //------ Derivacion----
  if(!from_accountkey(account_key, &account_key_type, &wallet_type, &key_type, &role_path, &l_key, &address_index_path, buff_xsk)){
        return false;
    }
    //----------------

    for(std::uint8_t i = 0; i<l_key; i++){
        child_key_out[i] = buff_xsk[i];
    }
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}


bool derive_account_key(std::uint8_t const *const account_key, AccountKeyType account_key_type, Wallet wallet_type,
                        Key key_type, Role role_path, std::uint32_t const address_index_path,
                        std::string& bech32_child_key_out){

    bech32_child_key_out.clear(); //se deja en cero el string

    int l_key=0;
    char hrp[17]; //soporta el prefijo mas largo
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves

    //------ Derivacion----
  if(!from_accountkey(account_key, &account_key_type, &wallet_type, &key_type, &role_path, &l_key, &address_index_path, buff_xsk)){
        return false;
    }
    //------- Hrp ------
    if(role_path != Role::OnlyAccount){
        if(role_path != Role::Staking){
            std::strcpy (hrp,"addr"); ///prefix
        }else{
            std::strcpy (hrp,"stake"); ///prefix
        }
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

    bech32_encode(hrp,buff_xsk,l_key,bech32_child_key_out);
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}
