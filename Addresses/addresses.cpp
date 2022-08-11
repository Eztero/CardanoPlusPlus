#include "addresses.hpp"

static bool set_address(Address const *const address_type, std::uint8_t const *const input_key, InputKey const *const input_key_type, Wallet const * const wallet_type,
                        std::uint32_t const *const account_path, std::uint32_t const *const address_index_path, std::uint8_t *const header,
                        std::uint8_t *const payload_left, std::uint8_t *const payload_right,
                        std::uint8_t *const buff_xvk){

    ///Set Address
    switch(*address_type){             ///bech32(hrp, (header || payload_left || payload_right) )

    case Address::Base:{              /// Role = External Address

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Extern, *address_index_path, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Staking, STAKE_INDEX, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Base_Change:{       /// Role = Internal Address

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Intern, *address_index_path, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Staking, STAKE_INDEX, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);


    };break;

    case Address::Enterprise:{        /// Role = External Address

        *header |= 0x60; //b0110_0000

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Extern, *address_index_path, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Enterprise_Change:{ /// Role = Internal Address

        *header |= 0x60; //b0110_0000

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Intern, *address_index_path, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Stake:{             /// Role = Staking Address = External Address

        *header |= 0xE0; //b1110_0000

        if(!raw_derivekey(input_key, *input_key_type, *wallet_type, OutputKey::Public,
                          *account_path, Role::Staking, *address_index_path, buff_xvk)){
            return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    }

    return true;
}


bool createaddress(std::uint8_t const *const input_key, InputKey const input_key_type, Network const network_id, Wallet const wallet_type, Address const address_type,
                   std::uint32_t const account_path, std::uint32_t const address_index_path,
                   std::string& address_out){

    char hrp[11];
    std::uint8_t header;
    std::uint8_t buff_xvk[XVK_LENGTH];
    std::uint8_t payload_left[28];
    std::uint8_t payload_right[28];

    ///Network::Mainnet=1, Network::Testnet=0
    header=static_cast<std::uint8_t>(network_id);

    ///Set Human Readable Part
    if(address_type != Address::Stake){
        std::strcpy(hrp, "addr");
    }else{
        std::strcpy(hrp, "stake");
    }
    if(network_id == Network::Testnet){
        std::strcat(hrp, "_test");
    }

    ///Set Address
    if(!set_address(&address_type, input_key, &input_key_type, &wallet_type,
                    &account_path, &address_index_path, &header,
                    payload_left, payload_right, buff_xvk)){
        return false;
    }

    buff_xvk[0] = header;
    if(address_type == Address::Base || address_type == Address::Base_Change ){
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1+i] = payload_left[i];
            buff_xvk[29+i] = payload_right[i];
        }
        bech32_encode(hrp, buff_xvk, 57, address_out);
    }else{
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1 + i] = payload_left[i];
        }
        bech32_encode(hrp, buff_xvk, 29, address_out);
    }

    return true;
}


bool raw_createaddress_keyhash(std::uint8_t const *const input_key, InputKey const input_key_type, Network const network_id, Wallet const wallet_type, Address const address_type,
                               std::uint32_t const account_path, std::uint32_t const address_index_path,
                               std::uint8_t *const output_keyhash,std::uint8_t *const output_keyhash_len){

    std::uint8_t header;
    std::uint8_t buff_xvk[XVK_LENGTH];
    std::uint8_t payload_left[28];
    std::uint8_t payload_right[28];

    ///Network::Mainnet=1, Network::Testnet=0
    header=static_cast<std::uint8_t>(network_id);


    ///Set Address
    if(!set_address(&address_type, input_key, &input_key_type, &wallet_type,
                    &account_path, &address_index_path, &header,
                    payload_left, payload_right, buff_xvk)){
        return false;
    }

    output_keyhash[0] = header;
    if(address_type == Address::Base || address_type == Address::Base_Change ){
        for(std::uint8_t i = 0; i < 28; i++){
            output_keyhash[1+i] = payload_left[i];
            output_keyhash[29+i] = payload_right[i];
            if(output_keyhash_len != nullptr){
                *output_keyhash_len = 57;
            }
        }
    }else{
        for(std::uint8_t i = 0; i < 28; i++){
            output_keyhash[1 + i] = payload_left[i];
        }
        if(output_keyhash_len != nullptr){
            *output_keyhash_len = 29;
        }
    }


    return true;
}

