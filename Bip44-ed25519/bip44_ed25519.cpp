#include "bip44_ed25519.hpp"


static bool from_masterkey(std::uint8_t const *const extended_master_secret_key, Wallet const *const wallet_type, OutputKey const *const output_key_type, Role const *const role_path,
                           std::uint32_t const *const index_acc, int *const keysize, std::uint32_t const *const address_index_path,
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
    default:{
        return false;
    };break;
    }

    if(!raw_child_privatekey(buff_xsk, H1815, buff_xsk)){                     /// m/1852'/1815'
        return false;
    }

    if(!raw_child_privatekey(buff_xsk, *index_acc, buff_xsk)){                 /// m/1852'/1815'/account'
        return false;
    }

    switch (*output_key_type){

    case OutputKey::Private:{
        *keysize = XSK_LENGTH;
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
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(!raw_child_privatekey(buff_xsk, *address_index_path, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                    return false;
                }
                if(!valid_ed25519_sk(buff_xsk)){
                    return false;
                }

            }else{
                return false;
            }
        }

    };break;

    case OutputKey::Public:{
        *keysize = XVK_LENGTH;
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
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(!raw_child_publickey(buff_xsk, *address_index_path, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    return false;
                }

            }else {
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


static bool from_accountkey(std::uint8_t const *const account_key, InputKey const *const account_key_type, Wallet const *const wallet_type, OutputKey const *const output_key_type, Role const *const role_path,
                            int *const keysize, std::uint32_t const *const address_index_path,
                            std::uint8_t *const buff_xsk){

    switch (*output_key_type){

    case OutputKey::Private:{
        if(*account_key_type == InputKey::AccountKey_xsk){
            *keysize = XSK_LENGTH;
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

                    if(!raw_child_privatekey(buff_xsk, *address_index_path, buff_xsk)){                     /// m/1852'/1815'/account'/role_path/address
                        return false;
                    }
                    if(!valid_ed25519_sk(buff_xsk)){
                        return false;
                    }

                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    };break;

    case OutputKey::Public:{
        *keysize = XVK_LENGTH;

        if(*account_key_type == InputKey::AccountKey_xsk){
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
                    return false;
                }
            };break;

            }

            if(*address_index_path < 2147483648U){

                if(!raw_child_publickey(buff_xsk, *address_index_path, buff_xsk)){                  /// m/1852'/1815'/account'/role_path/address
                    return false;
                }

            }else {
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


bool raw_derivekey(std::uint8_t const *const input_key ,InputKey input_key_type, Wallet wallet_type, OutputKey output_key_type,
                   std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
                   std::uint8_t *const output_key){

    std::memset(output_key,0,XVK_LENGTH); //se deja a cero los primeros 64 bytes, asi en caso de un error su sk o xvk seran cero

    int keysize = 0;
    std::uint32_t index_acc = 0;
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves

    //------ Derivacion----

    switch(input_key_type){
    case InputKey::ExtendedMasterKey:{
        if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
            return false;
        }
        index_acc = account_path + H0;
        if(!from_masterkey(input_key, &wallet_type, &output_key_type, &role_path,
                           &index_acc, &keysize, &address_index_path, buff_xsk)){
            sodium_memzero(buff_xsk, XSK_LENGTH);
            return false;
        }
    };break;
    default:{
        if(!from_accountkey(input_key, &input_key_type, &wallet_type, &output_key_type, &role_path,
                            &keysize, &address_index_path, buff_xsk)){
            sodium_memzero(buff_xsk, XSK_LENGTH);
            return false;
        }
    };break;
    }

    //----------------

    for(std::uint8_t i = 0; i<keysize; i++){
        output_key[i] = buff_xsk[i];
    }
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}

//for generate only account
bool raw_derivekey(std::uint8_t const *const input_key ,InputKey input_key_type, Wallet wallet_type, OutputKey output_key_type,
                   std::uint32_t const account_path, Role role_path, std::uint8_t *const output_key){

    std::memset(output_key,0,XVK_LENGTH); //se deja a cero los primeros 64 bytes, asi en caso de un error su sk o xvk seran cero

    int keysize = 0;
    std::uint32_t index_acc = 0;
    std::uint32_t address_index_path = 0;
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves

    //------ Derivacion----
    if(role_path == Role::OnlyAccount){
        if(input_key_type == InputKey::ExtendedMasterKey){
            if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
                return false;
            }
            index_acc = account_path + H0;
            if(from_masterkey(input_key, &wallet_type, &output_key_type, &role_path,
                              &index_acc, &keysize, &address_index_path, buff_xsk)){

                for(std::uint8_t i = 0; i<keysize; i++){
                    output_key[i] = buff_xsk[i];
                }
                sodium_memzero(buff_xsk, XSK_LENGTH);

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
    //----------------

    return true;
}



bool derivekey(std::uint8_t const *const input_key ,InputKey input_key_type, Wallet wallet_type, OutputKey output_key_type,
               std::uint32_t const account_path, Role role_path, std::uint32_t const address_index_path,
               std::string& bech32_output_key){

    bech32_output_key.clear(); //se deja en cero el string

    int keysize = 0;
    std::uint32_t index_acc = 0;
    char hrp[17]; //soporta el prefijo mas largo
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves

    //------ Derivacion----

    switch(input_key_type){
    case InputKey::ExtendedMasterKey:{
        if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
            return false;
        }
        index_acc = account_path + H0;
        if(!from_masterkey(input_key, &wallet_type, &output_key_type, &role_path,
                           &index_acc, &keysize, &address_index_path, buff_xsk)){
            sodium_memzero(buff_xsk, XSK_LENGTH);
            return false;
        }
    };break;
    default:{
        if(!from_accountkey(input_key, &input_key_type, &wallet_type, &output_key_type, &role_path,
                            &keysize, &address_index_path, buff_xsk)){
            sodium_memzero(buff_xsk, XSK_LENGTH);
            return false;
        }
    };break;
    }

    //------- Hrp ------
    if(role_path != Role::OnlyAccount){
        if(role_path != Role::Staking){
            std::strcpy (hrp,"addr");   ///prefix
        }else{
            std::strcpy (hrp,"stake");  ///prefix
        }
    }
    else{
        std::strcpy (hrp,"acct");       ///prefix
    }
    if(wallet_type == Wallet::MultiSignHD){
        std::strcat (hrp,"_shared");    ///prefix
    }
    if(keysize == XVK_LENGTH){
        std::strcat (hrp,"_xvk");       ///prefix

    }else{
        std::strcat (hrp,"_xsk");       ///prefix
    }
    //---------------

    bech32_encode(hrp,buff_xsk,keysize,bech32_output_key);
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}

//for generate only account
bool derivekey(std::uint8_t const *const input_key ,InputKey input_key_type, Wallet wallet_type, OutputKey output_key_type,
               std::uint32_t const account_path, Role role_path, std::string& bech32_output_key){

    bech32_output_key.clear(); //se deja en cero el string

    int keysize = 0;
    std::uint32_t index_acc = 0;
    std::uint32_t address_index_path = 0;
    char hrp[17]; //soporta el prefijo mas largo
    std::uint8_t buff_xsk[XSK_LENGTH];  //Se crea un buffer que pueda contener los dos tipos de llaves

    //------ Derivacion----

    if(role_path == Role::OnlyAccount){
        if(input_key_type == InputKey::ExtendedMasterKey){
            if(account_path > 2147483647U){ // y si no excede el limite maximo ((2^32) - 1) - 2^31 = (2^31) - 1 = 2147483647
                return false;
            }
            index_acc = account_path + H0;
            if(!from_masterkey(input_key, &wallet_type, &output_key_type, &role_path,
                               &index_acc, &keysize, &address_index_path, buff_xsk)){
                sodium_memzero(buff_xsk, XSK_LENGTH);
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }

    //------- Hrp ------
    std::strcpy (hrp,"acct");       ///prefix

    if(wallet_type == Wallet::MultiSignHD){
        std::strcat (hrp,"_shared");    ///prefix
    }
    if(keysize == XVK_LENGTH){
        std::strcat (hrp,"_xvk");       ///prefix

    }else{
        std::strcat (hrp,"_xsk");       ///prefix
    }
    //---------------

    bech32_encode(hrp,buff_xsk,keysize,bech32_output_key);
    sodium_memzero(buff_xsk, XSK_LENGTH);

    return true;
}

