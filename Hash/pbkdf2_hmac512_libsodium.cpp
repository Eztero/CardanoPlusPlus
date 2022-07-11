#include "pbkdf2_hmac512_libsodium.hpp"

inline void store32_be( std::uint8_t *const dst, std::uint32_t w ) // store32 bigendian
{
    dst[3] = static_cast<std::uint8_t>(w);
    dst[2] = static_cast<std::uint8_t>(w >> 8);
    dst[1] = static_cast<std::uint8_t>(w >> 16);
    dst[0] = static_cast<std::uint8_t>(w >> 24);
}

bool pbkdf2_hmac512_libsodium( std::uint8_t const *const key, std::uint8_t const key_len,
                               std::uint8_t const *const salt, std::uint8_t const salt_len,
                               std::uint32_t const iterations,
                               std::uint32_t const out_len , std::uint8_t *const out ){

    sodium_memzero(out, out_len);

    if(out_len > 0xfffffe) {  //se limita a una longitud de 16777214 bytes, pero su limite real es, out_len < (2^32 - 1) * 64
        return false;
    }

    std::uint64_t T_max = static_cast<std::uint64_t>(std::ceil(static_cast<float>(out_len) / 64));  // indica el numero de bloques que se generaran

    if(T_max >= 255){
        return false;
    }

    crypto_auth_hmacsha512_state init_hctx, hctx;
    uint8_t T_last_bytes= static_cast<std::uint8_t>(out_len - ((T_max - 1) * 64));   // indica cuantos bytes se tomaran del ultimo bloque
    std::uint8_t T_index[4];          // el indice de bloque
    std::uint8_t U[64];               // el PRF, U_1 = hmac512(key, salt || T_index) y U_(n-1) = hmac512(key, U_(n-1))
    std::uint8_t T[64];               // el Bloque, T = U_1 ^ U_2 ^ ... U_(n-1)
    std::uint8_t bytes_len = 0;       // indica la cantidad de bytes que se tomaran del bloque
    std::uint32_t c = 0;              // indica las iteraciones
    std::uint8_t x = 0;

    crypto_auth_hmacsha512_init(&init_hctx, key, key_len);          // se inicia init_hctx con el key  //(llave)
    crypto_auth_hmacsha512_update(&init_hctx, salt, salt_len);      // se agrega el fragmento del salt  //(mensaje o dato)


    for (std::uint8_t i = 0; i < T_max; i++) {                            // (Ciclo Bloques) ; T_(i)
        store32_be(T_index, static_cast<std::uint32_t>(i + 1));           // i=1 pasa a ivec[4]; T_index[0]=MSB...T_index[3]=LSB
        std::memcpy(&hctx, &init_hctx, sizeof(init_hctx));                // se pasa a el contenido de init_hctx a hctx
        crypto_auth_hmacsha512_update(&hctx, T_index, 4);                 // se concatena T_i a salt (salt || T_i )
        crypto_auth_hmacsha512_final(&hctx, U);                           // se genera un U_1

        memcpy(T, U, 64);                                                 // se copia U a T

        for(c = 2; c <= iterations; c++) {                                // (Ciclo PRF), inicia con c=2 , c < iteraciones
            crypto_auth_hmacsha512_init(&hctx, key, key_len);             // se inicia hctx con el key
            crypto_auth_hmacsha512_update(&hctx, U, 64);                  // se agrega el fragmento U
            crypto_auth_hmacsha512_final(&hctx, U);                       // se genera U_(c), PRF( HmacSha512(key, U_(c-1)) )

            for(x = 0; x < 64; x++) {                                     // (Ciclo XOR) va del byte 0 al 63, sha512
                T[x] ^= U[x];                                             // realiza un xor en paralelo por cada byte T[k]=T[k] ^ U[K] , si T=U_1 ; T= U_(j-1) ^ U_(j)
            }
        }

        if(i != (T_max - 1)){                                             // si es el ultimo byte, se copia la cantidad indicada en T_last_bytes
            bytes_len = 64;
        }
        else{
            bytes_len = T_last_bytes;
        }
        std::memcpy(&out[i * 64], T, static_cast<size_t>(bytes_len));          //  Los byte de cada nuevo bloque los concatenan al anterior hasta completar los bytes requeridos (outlen),
                                                                               //  asi un out[96] = T_1[64] || T_2[32]
    }
    sodium_memzero(&init_hctx, sizeof(init_hctx));
    return true;
}
