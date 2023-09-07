#include "../../include/cardanoplusplus/addresses/addresses.hpp"
#include "../../include/cardanoplusplus/bip44_ed25519/bip44_ed25519.hpp"
#include "../../include/cardanoplusplus/utils/txutils.hpp"
#include "../../include/cardanoplusplus/utils/cmacros.hpp"
#include "../../include/cardanoplusplus/hash/bech32.hpp"

namespace Cardano{

static int searchByteArraysofScript(ScriptType const script_type, std::uint8_t const * const input_script, std::size_t const * const input_script_len){
    int a = 0;
    int b = 0;
    bool ciclo = true;
    do{
        switch(script_type){
        case ScriptType::Native_Script:{
            switch(input_script[a]){

            case 0x98:{
                b = a;
                a += 2;
            };break;
            case 0x99:{
                b = a;
                a += 3;
            };break;
            case 0x9a:{
                b = a;
                a += 5;
            };break;
            case 0x9b:{
                b = a;
                a += 9;
            };break;
            default:{
                if(input_script[a] > 0x80 && input_script[a] < 0x97){
                    b = a;
                    a += 1;
                }
                else if(a > 0){
                    ciclo = false;
                }
                else {
                    throw std::invalid_argument("script error,  does not contain a array in cbor");
                }
            };break;

            }
        };break;
        case ScriptType::Plutus_Script_V1:
        case ScriptType::Plutus_Script_V2:{

            switch(input_script[a]){
            case 0x58:{
                b = a;
                a += 2;
            };break;
            case 0x59:{
                b = a;
                a += 3;
            };break;
            case 0x5a:{
                b = a;
                a += 5;
            };break;
            case 0x5b:{
                b = a;
                a += 9;
            };break;
            default:{
                if(input_script[a] > 0x40 && input_script[a] < 0x57){
                    b = a;
                    a += 1;
                }
                else if(a > 0){
                    ciclo = false;
                }
                else {
                    throw std::invalid_argument("script error,  does not contain a byte array in cbor");
                }
            };break;

            }

        };break;
        case ScriptType::None:{throw std::invalid_argument("script error, option ScriptType::None not valid");};break;
        }

    }while(ciclo);

    return b;
}

static void set_script(std::uint8_t const * const input_script, std::size_t const * const input_script_len, ScriptType const script_type,
                       std::uint8_t * const output_scripthash){
    int pos_arrayofscript = searchByteArraysofScript(script_type, input_script, input_script_len);
    int script_a_len = *input_script_len - pos_arrayofscript + 1;
    std::uint8_t script_a[script_a_len]{};

    if(script_type != ScriptType::None){
     script_a[0] = static_cast<std::uint8_t>(script_type);
    }else{throw std::invalid_argument("Set Script error, ScriptType not supported ");}

    std::memcpy ( &script_a[1], &input_script[pos_arrayofscript], (*input_script_len - pos_arrayofscript));
    crypto_generichash_blake2b(output_scripthash, BLAKE224_LENGTH, script_a, script_a_len, nullptr, 0);
    std::memset(script_a, 0x00, script_a_len);
}

static void set_address(Address const * const address_type, std::uint8_t const * const input_key, InputKey const input_key_type, Wallet const wallet_type,
                        std::uint32_t const * const account_path, std::uint32_t const * const address_index_path, std::uint8_t * const header,
                        std::uint8_t * const payload_left, std::uint8_t *const payload_right,
                        std::uint8_t * const buff_xvk){

    ///Set Address
    switch(*address_type){             ///bech32(hrp, (header || payload_left || payload_right) )

    case Address::Base:{              /// Role = External Address

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Extern, *address_index_path, buff_xvk)){
            throw std::invalid_argument("setting address error,  could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Staking, STAKE_INDEX, buff_xvk)){
            throw std::invalid_argument("setting address error,  could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Base_Change:{       /// Role = Internal Address

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Intern, *address_index_path, buff_xvk)){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Staking, STAKE_INDEX, buff_xvk)){
            throw std::invalid_argument("setting address error, could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);


    };break;

    case Address::Enterprise:{        /// Role = External Address

        *header |= 0x60; //b0110_0000

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Extern, *address_index_path, buff_xvk)){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Enterprise_Change:{ /// Role = Internal Address

        *header |= 0x60; //b0110_0000

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Intern, *address_index_path, buff_xvk)){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Stake:{             /// Role = Staking Address = External Address

        *header |= 0xE0; //b1110_0000

        if(!getRawKey(input_key, input_key_type, wallet_type, OutputKey::Public,
                      *account_path, Role::Staking, *address_index_path, buff_xvk)){
            throw std::invalid_argument("setting address error, could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    }

    //return true;
}

void getBech32Address(std::uint8_t const * const input_key, Cardano::InputKey const input_key_type, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
                      std::uint32_t const account_path, std::uint32_t const address_index_path,
                      std::string & address_out){

    char hrp[11]{};
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
    if(wallet_type == Wallet::MultiSignHD){
        throw std::invalid_argument("getBech32Address error, Wallet::MultiSignHD not supported ");
        //return false;
    } // las direcciones de pago y stake multifirma (shared) se crean de dos o mas  direcciones keyhash

    set_address(&address_type, input_key, input_key_type, wallet_type,
                &account_path, &address_index_path, &header,
                payload_left, payload_right, buff_xvk);



    buff_xvk[0] = header;
    if(address_type == Address::Base || address_type == Address::Base_Change ){
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1+i] = payload_left[i];
            buff_xvk[29+i] = payload_right[i];
        }
        Hash::bech32_encode(hrp, buff_xvk, 57, address_out);
    }else{
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1 + i] = payload_left[i];
        }
        Hash::bech32_encode(hrp, buff_xvk, 29, address_out);
    }

}

void getRawAddress(std::uint8_t const * const input_key, Cardano::InputKey const input_key_type, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
                   std::uint32_t const account_path, std::uint32_t const address_index_path,
                   std::uint8_t * const output_raw,std::uint8_t * const output_raw_len){

    std::uint8_t header;
    std::uint8_t buff_xvk[XVK_LENGTH]{};
    std::uint8_t payload_left[28]{};
    std::uint8_t payload_right[28]{};

    ///Network::Mainnet=1, Network::Testnet=0
    header=static_cast<std::uint8_t>(network_id);


    ///Set Address
    if(wallet_type == Wallet::MultiSignHD){
        throw std::invalid_argument("getRawAddress error, Wallet::MultiSignHD not supported ");
        //return false;
    } // las direcciones de pago y stake multifirma (shared) se crean de dos o mas  direcciones keyhash

    set_address(&address_type, input_key, input_key_type, wallet_type,
                &account_path, &address_index_path, &header,
                payload_left, payload_right, buff_xvk);



    output_raw[0] = header;
    if(address_type == Address::Base || address_type == Address::Base_Change ){
        for(std::uint8_t i = 0; i < 28; i++){
            output_raw[1+i] = payload_left[i];
            output_raw[29+i] = payload_right[i];
            if(output_raw_len != nullptr){
                *output_raw_len = 57;
            }
        }
    }else{
        for(std::uint8_t i = 0; i < 28; i++){
            output_raw[1 + i] = payload_left[i];
        }
        if(output_raw_len != nullptr){
            *output_raw_len = 29;
        }
    }

}

void getBech32ScriptHash(std::uint8_t const * const input_script, std::size_t input_script_len, Cardano::ScriptType const script_type, std::string & address_out ){

    std::uint8_t blake_28bytes[BLAKE224_LENGTH]{};
    set_script(input_script, &input_script_len, script_type,blake_28bytes);

    char hrp[11]{};
    std::strcpy(hrp, "script");
    Cardano::Hash::bech32_encode(hrp, blake_28bytes, BLAKE224_LENGTH, address_out);

}

// hash 28 bytes
void getRawScriptHash(std::uint8_t const * const input_script, std::size_t input_script_len, Cardano::ScriptType const script_type,
                      std::uint8_t * const output_28bytesraw, std::uint8_t * const output_28bytesraw_len ){

    set_script(input_script, &input_script_len, script_type,output_28bytesraw);
    if(output_28bytesraw_len != nullptr){
        *output_28bytesraw_len = BLAKE224_LENGTH;
    }

}

void getBech32AddressfromScript(std::string const &script, Cardano::ScriptType const script_type,  Cardano::Network const network_id,
                                Cardano::ScriptAddress const address_type, std::string & address_out ){
    char hrp[11]{};
    std::uint8_t output_29bytesraw[29]{};
    std::uint8_t output_28bytesraw[BLAKE224_LENGTH]{};
    std::size_t script_uint8_t_len = 0;
    std::uint8_t const * const script_uint8_t = Utils::hexchararray2uint8array(script, &script_uint8_t_len);
    try{
        set_script(script_uint8_t, &script_uint8_t_len, script_type, output_28bytesraw);
    }catch(const std::invalid_argument& erl){
        delete[] script_uint8_t;
        throw;
    }
    delete[] script_uint8_t;
    output_29bytesraw[0] = 0x70; // enterprise address
    output_29bytesraw[0] |= static_cast<std::uint8_t>(network_id);

    std::memcpy ( &output_29bytesraw[1], output_28bytesraw, BLAKE224_LENGTH );

    switch(address_type){
    case ScriptAddress::Payment:{ std::strcpy(hrp, "addr"); };break;
    case ScriptAddress::Stake:{ std::strcpy(hrp, "stake"); };break;
    }
    if(network_id == Network::Testnet){
        std::strcat(hrp, "_test");
    }
    Hash::bech32_encode(hrp, output_29bytesraw, 29, address_out);
}

}
