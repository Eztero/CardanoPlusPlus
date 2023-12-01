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

    script_a[0] = static_cast<std::uint8_t>(script_type);

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

    case Address::Base_Extern:{              /// Role = External Address

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Extern, *address_index_path, buff_xvk ) ){
            throw std::invalid_argument("setting address error,  could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Staking, STAKE_INDEX, buff_xvk ) ){
            throw std::invalid_argument("setting address error,  could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Base_Intern:{       /// Role = Internal Address

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Intern, *address_index_path, buff_xvk ) ){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Staking, STAKE_INDEX, buff_xvk ) ){
            throw std::invalid_argument("setting address error, could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_right, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);


    };break;

    case Address::Enterprise_Extern:{        /// Role = External Address

        *header |= 0x60; //b0110_0000

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Extern, *address_index_path, buff_xvk ) ){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Enterprise_Intern:{ /// Role = Internal Address

        *header |= 0x60; //b0110_0000

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Intern, *address_index_path, buff_xvk ) ){
            throw std::invalid_argument("setting address error, could not get a valid public key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    case Address::Stake:{             /// Role = Staking Address = External Address

        *header |= 0xE0; //b1110_0000

        if(!getRawKey( input_key_type, input_key, wallet_type, OutputKey::Public,
                       *account_path, Role::Staking, *address_index_path, buff_xvk ) ){
            throw std::invalid_argument("setting address error, could not get a valid public stake key");
            //return false;
        }

        crypto_generichash_blake2b(payload_left, BLAKE224_LENGTH, buff_xvk, 32, nullptr, 0);

    };break;

    }

    //return true;
}

void getBech32Address( Cardano::InputKey const input_key_type, std::uint8_t const * const input_key, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
                       std::uint32_t const account_path, std::uint32_t const address_index_path,
                       std::string & address_output){

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
    if(address_type == Address::Base_Extern || address_type == Address::Base_Intern){
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1+i] = payload_left[i];
            buff_xvk[29+i] = payload_right[i];
        }
        Hash::bech32_encode(hrp, buff_xvk, 57, address_output);
    }else{
        for(std::uint8_t i = 0; i < 28; i++){
            buff_xvk[1 + i] = payload_left[i];
        }
        Hash::bech32_encode(hrp, buff_xvk, 29, address_output);
    }

}

void getRawAddress( Cardano::InputKey const input_key_type, std::uint8_t const * const input_key, Cardano::Network const network_id, Cardano::Wallet const wallet_type, Cardano::Address const address_type,
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
    if(address_type == Address::Base_Extern || address_type == Address::Base_Intern ){
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

void getRawAddressKeyHash( Cardano::InputKey const input_key_type, std::uint8_t const * const input_key,
                           std::uint32_t const account_path,std::uint8_t * const addresskeyhash_output, std::uint8_t * const addresskeyhash_len ){

    std::uint8_t header=0;
    std::uint8_t buff_xvk[XVK_LENGTH]{};
    std::uint8_t payload_left[28]{};
    std::uint8_t payload_right[28]{};

    std::uint32_t const address_index_path = 0;
    Address const address_type = Address::Stake;
    Wallet const wallet_type = Wallet::HD;

    set_address(&address_type, input_key, input_key_type, wallet_type,
                &account_path, &address_index_path, &header,
                payload_left, payload_right, buff_xvk);



    addresskeyhash_output[0] = header;

    for(std::uint8_t i = 0; i < 28; i++){
        addresskeyhash_output[i] = payload_left[i];
    }
    if(addresskeyhash_len != nullptr){
        *addresskeyhash_len = 28;
    }
}

void getBech32ScriptHash( Cardano::ScriptType const script_type, std::string const &script, std::string & address_output ){

    std::size_t script_uint8_t_len = 0;
    std::uint8_t const * const script_uint8_t = (script_type != ScriptType::None)?Utils::hexchararray2uint8array(script, &script_uint8_t_len):nullptr;

    if(script_uint8_t != nullptr){
        std::uint8_t blake_28bytes[BLAKE224_LENGTH]{};
        set_script(script_uint8_t, &script_uint8_t_len, script_type,blake_28bytes);
        delete[] script_uint8_t;
        char hrp[11]{};
        std::strcpy(hrp, "script");
        if(!Hash::bech32_encode(hrp, blake_28bytes, BLAKE224_LENGTH, address_output)){
            throw std::invalid_argument("getBech32ScriptHash error , could not encode in bech32 ");
        }

    }else{
        throw std::invalid_argument("getBech32ScriptHash error , ScriptType not supported ");
    }

}

// hash 28 bytes
void getRawScriptHash( Cardano::ScriptType const script_type, std::string const &script,
                       std::uint8_t * const scripthash_output, std::uint8_t * const scripthash_len ){

    std::size_t script_uint8_t_len = 0;
    std::uint8_t const * const script_uint8_t = (script_type != ScriptType::None)?Utils::hexchararray2uint8array(script, &script_uint8_t_len):nullptr;

    if(script_uint8_t != nullptr){
        set_script(script_uint8_t, &script_uint8_t_len, script_type,scripthash_output);
        if(scripthash_len != nullptr){
            *scripthash_len = BLAKE224_LENGTH;
        }
        delete[] script_uint8_t;
    }else{
        throw std::invalid_argument("getRawScriptHash error , ScriptType not supported ");
    }

}

void getBech32AddressfromScript( Cardano::ScriptType const script_type, std::string const &script,  Cardano::Network const network_id,
                                 Cardano::ScriptAddress const address_output_type, std::string & address_output ){
    char hrp[11]{};
    std::uint8_t payload[29]{};
    std::uint8_t output_28bytesraw[BLAKE224_LENGTH]{};
    std::size_t script_uint8_t_len = 0;
    std::uint8_t const * const script_uint8_t = (script_type != ScriptType::None)?Utils::hexchararray2uint8array(script, &script_uint8_t_len):nullptr;

    if(script_uint8_t != nullptr){
        set_script(script_uint8_t, &script_uint8_t_len, script_type, output_28bytesraw);
        delete[] script_uint8_t;
    }else{
        throw std::invalid_argument("getBech32AddressfromScript error , ScriptType not supported ");
    }

    switch(address_output_type){
    case ScriptAddress::Payment:{
        std::strcpy(hrp, "addr");
        payload[0] = 0x70 | static_cast<std::uint8_t>(network_id);; // enterprise address
    };break;
    case ScriptAddress::Stake:{
        std::strcpy(hrp, "stake");
        payload[0] = 0xf0 | static_cast<std::uint8_t>(network_id);; // stake address
    };break;
    }

    if(network_id == Network::Testnet){
        std::strcat(hrp, "_test");
    }

    std::memcpy ( &payload[1], output_28bytesraw, 28 );
    Hash::bech32_encode(hrp, payload, 29, address_output);
}

void getBech32AddressfromAddresses( std::string const &payment_address, std::string const &stake_address,
                                    std::string & address_output ){

    std::uint8_t payload_payment[BECH32_MAX_LENGTH]{};
    std::uint8_t payload_stake[BECH32_MAX_LENGTH]{};
    std::uint8_t payload[57]{};
    char hrp[11]{};

    std::uint16_t payload_payment_len = 0;
    std::uint16_t payload_stake_len = 0;

    if(payment_address.find("addr")){
        throw std::invalid_argument("getBech32AddressfromAddresses error , payment address does not contain a valid address");
    }
    if(stake_address.find("stake")){
        throw std::invalid_argument("getBech32AddressfromAddresses error , stake address does not contain a valid address");
    }

    if(!Hash::bech32_decode(payment_address.c_str(),payload_payment,&payload_payment_len)){
        throw std::invalid_argument("getBech32AddressfromAddresses error , could not decode payment address");
    }

    if(!Hash::bech32_decode(stake_address.c_str(),payload_stake,&payload_stake_len)){
        throw std::invalid_argument("getBech32AddressfromAddresses error , could not decode stake address");
    }

    //verifica las cabezeras de las direcciones
    if(payload_payment[0] >> 7){
        throw std::invalid_argument("getBech32AddressfromAddresses error , payment address does not contain a valid payment header");
    }

    if(!(payload_stake[0] >> 7)){
        throw std::invalid_argument("getBech32AddressfromAddresses error , stake address does not contain a valid payment header");
    }

    // se usa la direccion de pago para obtener la red en la que se trabaja mainnet/testnet
    payload[0] = payload_payment[0] & 0x0f;
    switch(payload[0]){
    case 0x01:{
        std::strcpy(hrp, "addr");
    };break;
    case 0x00:{
        std::strcpy(hrp, "addr_test");
    };break;
    }

    // Se crea el nuevo header a partir de las dos direcciones pago y stake
    payload[0] |= ( payload_payment[0] & 0x10 ); // payload_payment[0] & b0001_0000
    payload[0] |= ( (payload_stake[0] & 0x10) << 1 );

    for(std::uint8_t i = 0; i < 28; i++){
        payload[1+i] = payload_payment[i+1];
        payload[29+i] = payload_stake[i+1];
    }

    Hash::bech32_encode(hrp, payload, 57, address_output);

}

}
