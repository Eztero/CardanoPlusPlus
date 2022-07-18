#include "bip32_ed25519.hpp"


void hmac512_sodium( std::uint8_t const *const key, std::size_t const key_len, std::uint8_t const *const data, std::size_t const data_len, std::uint8_t *const out) //out[64]
{
    crypto_auth_hmacsha512_state hctx;
    crypto_auth_hmacsha512_init(&hctx, key, key_len);
    crypto_auth_hmacsha512_update(&hctx, data, data_len);
    crypto_auth_hmacsha512_final(&hctx, out);
};

inline void normalize_bytes_icarusmethod( std::uint8_t *const kl ){
    kl[0] &= 0xf8;  //0b1111_1000;
    kl[31] &= 0x1f; //0b0001_1111;
    kl[31] |= 0x40; //0b0100_0000;

}

inline void store32_le( std::uint32_t const index, std::uint8_t *const out ){ //store32 litleendian
    out[0] = static_cast<std::uint8_t>(index);
    out[1] = static_cast<std::uint8_t>(index >> 8);
    out[2] = static_cast<std::uint8_t>(index >> 16);
    out[3] = static_cast<std::uint8_t>(index >> 24);
}

/// kl_out = kparentl + 8 * trunc28(zl)
void add_28_mul8( std::uint8_t const *const kparentl, std::uint8_t const *const zl, std::uint8_t *const kl_out ){

    std::uint16_t carry = 0;
    std::uint16_t entero = 0;

    for(std::uint8_t i = 0; i < 28; i++){
        entero = static_cast<std::uint16_t>(kparentl[i]) + (static_cast<std::uint16_t>(zl[i]) << 3) + carry;
        kl_out[i] = static_cast<std::uint8_t>(entero & 0xff);
        carry = entero >> 8;
    }
    for(std::uint8_t i = 28; i < 32; i++){
        entero = static_cast<std::uint16_t>(kparentl[i]) + carry;
        kl_out[i] = static_cast<std::uint8_t>(entero & 0xff);
        carry = entero >> 8;
    }
}

/// kr_out = zr + kparentr
void add_256bits( std::uint8_t const *const kparentr, std::uint8_t const *const zr, std::uint8_t *const kr_out ){
    std::uint16_t carry = 0;
    std::uint16_t entero = 0;

    for(std::uint8_t i = 0; i<32; i++){
        entero = static_cast<std::uint16_t>(kparentr[i]) + static_cast<std::uint16_t>(zr[i]) + carry;
        kr_out[i] = static_cast<std::uint8_t>(entero & 0xff);
        carry = entero >> 8;
    }
}

/// ai_out = aparent + (8*trunc28(zl))*B
bool point_plus(std::uint8_t const *const aparent,std::uint8_t const *const zl,std::uint8_t *const ai_out){
    std::uint8_t cero[32]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    std::uint8_t zl_8[32];
    std::uint8_t azl_8[32];


    //8 * trunc28(zl) -> zl_8
    add_28_mul8(cero, zl, zl_8);

    //zl_8(private key) ; verifica si ultimo byte
    if(zl_8[31] > 127){
        return false;
    }

    //         azl_8 = zl_8 * B
    if(crypto_scalarmult_ed25519_base_noclamp(azl_8, zl_8) != 0 ){
        return false;
    }
    //ai_out = aparent + azl_8
    if(crypto_core_ed25519_add(ai_out, azl_8, aparent) != 0 ){
        return false;
    }

    //ai_out[31] ^= 0x80; //0b10000000
    return true;
}

/// root_privatekey/extended_mastersecretkey[96] = pbkdf2_hmac512(entropy)
bool raw_masterkeys_generation( std::uint8_t const *const entropy, std::size_t entropy_len, std::uint8_t const *const password, std::size_t password_len, std::uint8_t *const extended_mastersecretkey ){

    std::memset(extended_mastersecretkey, 0, EXTENDED_MASTERSECRETKEY_LENGTH);

    pbkdf2_hmac512_libsodium( password, password_len, entropy, entropy_len, 4096, EXTENDED_MASTERSECRETKEY_LENGTH, extended_mastersecretkey );

    normalize_bytes_icarusmethod(extended_mastersecretkey); // se establecen los bits segun la norma icarus

    return true;
}

/// (A/extended_publickey)[64] = kl[32] * B || k_chaincode[32]
bool raw_privatekey_to_publickey( std::uint8_t const *const raw_privatekey_xsk, std::uint8_t *const raw_publickey_xvk ){

    if(raw_privatekey_xsk[31] > 127){ //si su primer bits mas alto es 1 entonces no es valido
        return false;
    }

    if(crypto_scalarmult_ed25519_base_noclamp(raw_publickey_xvk, raw_privatekey_xsk) != 0){ // se obtiene la llave publica
        return false;
    }

    for(std::uint8_t i = 64 ; i < 96; i++){  //Copia el chain code del privatekey
    raw_publickey_xvk[i - 32] = raw_privatekey_xsk[i];  // Public Key (32 bytes) || Chain Code (32 bytes) = Extended Verification Keys
    }

    return true;
}

///IMPLEMENTAR un sistema para que compruebe el valor maximo de uint32_t index o lo limite a su valor maximo

/// extended_privatekey_child[96] = kli || kri || Ci
bool raw_child_privatekey( std::uint8_t const *const raw_parent_privatekey_xsk, std::uint32_t const index, std::uint8_t *const raw_child_privatekey_xsk ){

   std::uint8_t buffer_xvk[XVK_LENGTH]; // buffer de 64bytes (almacena sha512 y llaves xvk)

    if(index < 0x80000000){//de 0x00000000 a 0x7fffffff; de 0 a (2^31)-1

        std::uint8_t data_raw[37]; //1byte(0x02)+32byte(raw_public_key_littleendian)+4byte(index)
        std::uint8_t data_ci[64];


        //parent public key (Big-endian)
        raw_privatekey_to_publickey(raw_parent_privatekey_xsk,buffer_xvk);
        for(std::uint8_t i = 0; i < 32; i++){
        data_raw[i+1] = buffer_xvk[i];
        }
        //index (Little-endian)
        store32_le(index, &data_raw[33]);

        //Z = hmac512( parent chaincode, 0x02 || parent publickey || index )
        data_raw[0] = 0x02;
        hmac512_sodium(&raw_parent_privatekey_xsk[64], 32, data_raw, 37, buffer_xvk); //ahora hmac512=Z=raw_child_extended_private_key

        //Ci = hmac512( parent chaincode, 0x03 || parent publickey || index )
        data_raw[0] = 0x03;
        hmac512_sodium(&raw_parent_privatekey_xsk[64], 32, data_raw, 37, data_ci);

        //parent chain code
        for(std::uint8_t i = 32; i < 64; i++){
            raw_child_privatekey_xsk[64 + (i - 32)] = data_ci[i];
        }

        sodium_memzero(data_raw, 37);
        sodium_memzero(data_ci, 64);

    }
    else{ //if(index >= 0x80000000 && index < 0xffffffff){ //de 0x80000000 a 0xffffffff ; de 2^31 a (2^32)-1, se elimina el if else() ya que el valor de uint32_max = index maximo
        std::uint8_t data_raw[69]; //1byte(0x00)+64byte(kparent_littleendian)+4byte(index)
        std::uint8_t data_ci[64];

        //raw_parent_privatekey_xsk[64] = k = kparent_l + kparent_r
        for(std::uint8_t i = 0; i < 64; i++){
            data_raw[1+i] = raw_parent_privatekey_xsk[i];
        }
        //index (Little-endian)
        store32_le(index, &data_raw[65]);

        //Z = hmac512( parent chaincode, 0x00 || parent privatekey || index )
        data_raw[0] = 0x00;
        hmac512_sodium(&raw_parent_privatekey_xsk[64], 32, data_raw, 69, buffer_xvk); //ahora hmac512=Z=raw_child_extended_private_key(64bytes)

        //Ci = hmac512( parent chaincode, 0x01 || parent privatekey || index )
        data_raw[0] = 0x01;
        hmac512_sodium(&raw_parent_privatekey_xsk[64], 32, data_raw, 69, data_ci);

        for(std::uint8_t i = 32; i<64; i++){
            raw_child_privatekey_xsk[64 + (i - 32)] = data_ci[i];
        }

        sodium_memzero(data_raw, 69);
        sodium_memzero(data_ci, 64);

    }
    //else{
    //return false;
    //}

    std::uint8_t kl[32];
    std::uint8_t kr[32];


    add_28_mul8(&raw_parent_privatekey_xsk[0], &buffer_xvk[0], kl);
    add_256bits(&raw_parent_privatekey_xsk[32], &buffer_xvk[32], kr);

    for(std::uint8_t i = 0; i < 32; i++){
        raw_child_privatekey_xsk[i] = kl[i];
        raw_child_privatekey_xsk[32 + i] = kr[i];
    }

    sodium_memzero(kl, 32);
    sodium_memzero(kr, 32);
    sodium_memzero(buffer_xvk, XVK_LENGTH);

    return true;
};

/// extended_publickey_child[64] = Ai[32] || Ci[32]
bool raw_child_publickey( std::uint8_t const *const raw_parent_public_key_xvk, std::uint32_t const index, std::uint8_t *const raw_child_public_key_xvk ){
    if(index < 0x80000000){//de 0x00000000 a 0x7fffffff; de 0 a (2^31)-1
        std::uint8_t data_raw[37]; //1byte(0x02)+32byte(raw_public_key_littleendian)+4byte(index)
        std::uint8_t data_ci[64];
        std::uint8_t data_z[64];

        //parent public key (Big-endian)
        for(std::uint8_t i = 0; i < 32; i++){
            data_raw[1 + i] = raw_parent_public_key_xvk[i];
        }
        // index (Little-endian)
        store32_le(index, &data_raw[33]);

        //Z = hmac512( parent chaincode, 0x02 || parent publickey || index )
        data_raw[0] = 0x02;
        hmac512_sodium(&raw_parent_public_key_xvk[32], 32, data_raw, 37, data_z); //ahora Z=data_z

        //Ci = hmac512( parent chaincode, 0x03 || parent publickey || index )
        data_raw[0] = 0x03;
        hmac512_sodium(&raw_parent_public_key_xvk[32], 32, data_raw, 37, data_ci);
        for(std::uint8_t i = 32; i < 64; i++){
            raw_child_public_key_xvk[i] = data_ci[i];
        }
        sodium_memzero(data_raw, 37);
        sodium_memzero(data_ci, 64);
        //Ai = parent publickey + trunc28(Zl)B
        if(point_plus(raw_parent_public_key_xvk, data_z, raw_child_public_key_xvk) == false) {
        sodium_memzero(data_z, 64);
        return false;
        }
        sodium_memzero(data_z, 64);
    }
    else{
            return false;
    }

    return true;
}