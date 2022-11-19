#include "bech32.hpp"

char const B32Chars_encode[33] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

std::int8_t const B32Chars_decode[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    15, -1, 10, 17, 21, 20, 26, 30,  7,  5, -1, -1, -1, -1, -1, -1,
    -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
     1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1,
    -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
     1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1
};

///concat_data= data1 || data2 || add_bytes
static std::uint8_t *concat_data(std::uint8_t const *const data1,std::uint16_t const *const data1_len, std::uint8_t const *const data2, std::uint16_t const *const data2_len, uint16_t const add_bytes, std::uint16_t *const data_out_len){
    *data_out_len = (*data2_len) + (*data1_len) + add_bytes;  // se espera que el valor no supere los 65535 bytes de largo
    std::uint8_t *data_out = static_cast<std::uint8_t*>(std::calloc(*data_out_len, sizeof(std::uint8_t)));
    if(data_out != nullptr){
        for(std::uint16_t i = 0; i < (*data_out_len); i++){
            if(i < (*data1_len)){
                data_out[i] = data1[i];
            }
            else{
                data_out[i] = data2[i - (*data1_len)];
            }
        }
    }
    return data_out;
}

/// Encode
static std::uint8_t *convert_bits(std::uint8_t const *const data,std::uint16_t const *const data_len , std::uint16_t const fromBits, std::uint16_t const toBits, std::uint16_t *const convert_bits_len){
    std::uint16_t acc = 0;
    std::uint16_t bits = 0;
    std::uint16_t maxv = (1 << toBits) - 1;
    std::uint16_t maxacc = (1 << (fromBits + toBits - 1)) - 1;
    std::uint16_t cv_bits_len = (*data_len) * 2;
    std::uint8_t *cv_bits = static_cast<std::uint8_t*>(std::calloc(cv_bits_len, sizeof(std::uint8_t)));
    if(cv_bits != nullptr){
        *convert_bits_len = 0;

        for(std::uint16_t i = 0; i < *data_len; i++){
            if ((data[i] >> fromBits) > 0)
            {
                *convert_bits_len = 0;
                std::memset(cv_bits, 0, cv_bits_len);
                std::free(cv_bits);
                return nullptr;
            }
            acc = ((acc << fromBits) | data[i]) & maxacc;
            bits += fromBits;
            while (bits >= toBits)
            {
                bits -= toBits;
                cv_bits[*convert_bits_len] = static_cast<std::uint8_t>((acc >> bits) & maxv);
                *convert_bits_len += 1;
            }
        }
        if (bits > 0)
        {
            cv_bits[*convert_bits_len] = static_cast<std::uint8_t>((acc << (toBits - bits)) & maxv);
            *convert_bits_len += 1;
        }
    }
    return cv_bits;

}

///Decode cardano addr
static bool decode_bits( std::uint8_t const *const data,std::uint16_t const data_len, std::uint16_t const fromBits, std::uint16_t const toBits, std::uint8_t *const bits_out ,std::uint16_t *const bits_out_len ){
    std::uint16_t blen = 0;
    std::uint16_t acc = 0;
    std::uint16_t bits = 0;
    std::uint16_t maxv = (1 << toBits) - 1;
    std::uint16_t maxacc = (1 << (fromBits + toBits - 1)) - 1;

    if(bits_out_len != nullptr){
        *bits_out_len=0;
    }
    for(std::uint16_t i = 0; i < data_len; i++){
        if ((data[i] >> fromBits) > 0)
        {
            return false;
        }
        acc = ((acc << fromBits) | data[i]) & maxacc;
        bits += fromBits;
        while (bits >= toBits)
        {
            if(blen <= 57){ //si esta fuera del rango de la llave mas larga (hashkey) genera un false
                bits -= toBits;
                bits_out[blen] = static_cast<std::uint8_t>((acc >> bits) & maxv);
                blen += 1;
            }else{
                return false;
            }
        }
    }
    if (bits >= fromBits || static_cast<std::uint8_t>((acc << (toBits - bits)) & maxv) != 0 || blen < 28) //si blen es menor a la llave mas corta (hashkey) genera un false
    {
        return false;
    }
    if(bits_out_len != nullptr){
        *bits_out_len = blen;
    }

    return true;
}

static bool IsValidHrp(char const *const hrp, std::uint8_t *const hrp_len){
    *hrp_len = 0;
    //comprueba si el largo de la cadena esta entre 1 a 83
    while(hrp[*hrp_len] != '\0'){
        *hrp_len += 1;
        if(*hrp_len > 83){ // no debe pasarse de los 83 bytes
            return false;
        }
    }
    if(*hrp_len < 1){
        return false;
    }

    //comprueba si el valor de cada byte esta en el rango de 33 a 126
    for (uint8_t a = 0; a < *hrp_len; a++){
        if(static_cast<int>(hrp[a]) < 33 || static_cast<int>(hrp[a]) > 126){
            return false;
        }
    }
    return true;
}

static bool IsValidStringBench32(char const *const bech32_code, std::uint16_t *const bech32_code_pos_separator, std::uint16_t *const bech32_code_lenght){
    bool lower = false;
    bool upper = false;
    std::uint8_t c = 0;
    std::uint16_t len = 0;
    std::uint16_t pos = 0;

    while(bech32_code[len] != '\0'){
        if(bech32_code[len] == SEPARATOR_BECH32){ // siempre detecta el ultimo separador ('1')
            pos = len;
        }
        len++;
        if(len > 65533){  // evita el desbordamiento de memoria para len y pos
            return false;
        }
    }

    *bech32_code_pos_separator = pos;
    *bech32_code_lenght = len;

    for (std::uint16_t i = 0; i < len; ++i) {
        c = static_cast<std::uint8_t>(bech32_code[i]);
        if (c >= 'a' && c <= 'z'){lower = true;}
        else if (c >= 'A' && c <= 'Z'){upper = true;}
        else if (c < 33 || c > 126) {return false;}  //no es valido si contiene otros caracteres
    }
    if(lower && upper){return false;}  //solo es valido si solo posee letras mayusculas o minusculas no ambas

    if (pos == len|| pos == 0 || pos + 7 > len || (len-pos) < 6) {
        return false;
    }
    return true;
}

static void bech32_polymod(std::uint8_t const *const data,std::uint16_t const *const data_len, std::uint32_t *const polymod_out){
    std::uint8_t b = 0;
    *polymod_out = 1;
    for(std::uint16_t i = 0; i < *data_len; i++){
        b = *polymod_out >> 25;
        *polymod_out = ((*polymod_out & 0x1ffffff) << 5) ^ static_cast<std::uint32_t>(data[i]);

        if (b & 1){*polymod_out ^= 0x3b6a57b2;};
        if (b & 2){*polymod_out ^= 0x26508e6d;};
        if (b & 4){*polymod_out ^= 0x1ea119fa;};
        if (b & 8){*polymod_out ^= 0x3d4233dd;};
        if (b & 16){*polymod_out ^= 0x2a1462b3;};
    }
}


static std::uint8_t *bech32_hrp_expand(char const *const hrp, std::uint8_t const *const hrp_len, std::uint16_t *const bech32_hrp_expand_len){

    std::uint16_t len = ((*hrp_len) * 2) + 1;
    *bech32_hrp_expand_len = len;

    std::uint8_t *hrp_e = static_cast<std::uint8_t*>(std::calloc(len, sizeof(std::uint8_t)));
    if(hrp_e != nullptr){
        for (std::uint16_t i = 0; i < *hrp_len; ++i) {
            hrp_e[i] = static_cast<std::uint8_t>(hrp[i]) >> 5;
            hrp_e[i + *hrp_len + 1] = static_cast<std::uint8_t>(hrp[i]) & 0x1f;
        }
    }

    return hrp_e;
}

static bool bech32_verify_checksum(char const *const hrp, std::uint8_t const *const hrp_len, std::uint8_t const *const data, std::uint16_t const data_len){
    std::uint16_t hrp_expand_len;
    std::uint16_t v_len = 0;
    std::uint32_t pmod;

    std::uint8_t *hrp_expand = bech32_hrp_expand(hrp, hrp_len, &hrp_expand_len);
    if(hrp_expand == nullptr){
        return false;
    }
    std::uint8_t *v = concat_data(hrp_expand, &hrp_expand_len, data, &data_len, 0, &v_len);
    if(v == nullptr){
        std::free(hrp_expand);
        return false;
    }

    bech32_polymod(v, &v_len, &pmod);

    //se borra v[] y hrp_expand[]
    std::memset(v, 0, v_len);
    std::free(hrp_expand);
    std::free(v);

    if(pmod == 1){
        return true;
    }
    return false;

}

static bool bech32_create_checksum(char const *const hrp, std::uint8_t const *const hrp_len, std::uint8_t const *const data, std::uint16_t const *const data_len,std::uint8_t *const checksum) {
    std::uint16_t hrp_expand_len;
    std::uint16_t v_len = 0;
    //std::uint16_t enc_len = 0;
    std::uint32_t polymod;

    std::uint8_t *hrp_expand=bech32_hrp_expand(hrp, hrp_len, &hrp_expand_len);
    if(hrp_expand == nullptr){
        return false;
    }
    std::uint8_t *v = concat_data(hrp_expand, &hrp_expand_len, data, data_len, 6, &v_len); //se agregan 6 bytes adicionales al array para el calculo del checksum en bech32_polymod
    if(v == nullptr){
        std::free(hrp_expand);
        return false;
    }

    bech32_polymod(v, &v_len, &polymod);
    polymod = polymod ^ 1;
    for(std::uint8_t i = 0; i < 6; i++){
        checksum[i] = (polymod >> (5 * (5 - i))) & 0x1f;
    }

    //borrar hrp_expand[] y v[]
    std::memset(v, 0, v_len);
    std::free(hrp_expand);
    std::free(v);

    return true;
}

bool bech32_encode(char const *const hrp, std::uint8_t const *const data, std::uint16_t const data_len, std::string &encode_out){

    encode_out.clear();

    std::uint8_t hrp_len = 0;
    std::uint16_t c_bit_len = 0;
    std::uint16_t cdata_len = 0;
    std::uint16_t chk_len = 6;
    std::uint8_t chk[chk_len];

    if(!IsValidHrp(hrp, &hrp_len)){
        return false;
    }

    std::uint8_t *c_bit = convert_bits(data, &data_len, 8, 5, &c_bit_len);
    if(c_bit == nullptr){
        return false;
    }

    if(!bech32_create_checksum(hrp, &hrp_len, c_bit, &c_bit_len, chk)){
        std::memset(c_bit, 0, c_bit_len);
        std::free(c_bit);
        return false;
    }

    std::uint8_t *cdata=concat_data(c_bit,&c_bit_len, chk, &chk_len, 0, &cdata_len);
    if(cdata == nullptr){
        std::memset(c_bit, 0, c_bit_len);
        std::free(c_bit);
        return false;
    }


    encode_out += hrp; //hrp
    encode_out += SEPARATOR_BECH32; //hrp + separador
    encode_out.reserve(encode_out.length() + cdata_len);

    for (std::uint16_t i = 0; i<cdata_len; i++) {
        encode_out += B32Chars_encode[ cdata[i] ]; //hrp + separador + data
    }

    //Borrar cdata c_bit
    std::memset(c_bit, 0, c_bit_len);
    std::memset(cdata, 0, cdata_len);
    std::free(cdata);
    std::free(c_bit);

    return true;
}

bool bech32_decode(char const *const bech32_code,std::uint8_t *const data,std::uint16_t *const data_len){
    std::uint16_t bech32_code_lenght = 0;
    std::uint16_t pos_separator = 0;
    std::uint8_t hrp_len = 0;

    if(!IsValidStringBench32(bech32_code, &pos_separator, &bech32_code_lenght)){
        return false;
    }

    char hrp[pos_separator + 1];
    std::strncpy ( hrp, bech32_code, pos_separator);
    hrp[pos_separator] = '\0';

    if(!IsValidHrp(hrp, &hrp_len)){
        return false;
    }

    std::uint16_t c_bit_len = bech32_code_lenght-(pos_separator+1);
    std::uint8_t c_bit[c_bit_len];
    std::memset(c_bit,0,c_bit_len);

    for (std::uint16_t i = 0; i < c_bit_len; i++)
    {
        c_bit[i] = B32Chars_decode[ static_cast<std::uint8_t>( bech32_code[ (pos_separator + 1) + i] ) ];
    }

    if (!bech32_verify_checksum(hrp, &hrp_len, c_bit, c_bit_len)){
        std::memset(c_bit, 0, c_bit_len);
        return false;
    }

    if(!decode_bits(c_bit, (c_bit_len - 6), 5, 8, data, data_len)){
        std::memset(c_bit, 0, c_bit_len);
        return false;
    }

    std::memset(c_bit, 0, c_bit_len);
    return true;

}
