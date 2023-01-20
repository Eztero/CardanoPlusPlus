#include "txutils.hpp"


void agregarUint64BytestoVector(std::vector <std::uint8_t> & bytesvector, std::uint64_t const & numero){
    bytesvector.push_back( ( numero >> 56 ) & 0xff );
    bytesvector.push_back( ( numero >> 48 ) & 0xff );
    bytesvector.push_back( ( numero >> 40 ) & 0xff );
    bytesvector.push_back( ( numero >> 32 ) & 0xff );
    bytesvector.push_back( ( numero >> 24 ) & 0xff );
    bytesvector.push_back( ( numero >> 16 ) & 0xff );
    bytesvector.push_back( ( numero >> 8 ) & 0xff );
    bytesvector.push_back( ( numero ) & 0xff );
}


std::uint64_t const Array8bytestoUint64(std::uint8_t const * const array8bytes){

return (( static_cast<std::uint64_t>(*array8bytes) << 56) | (static_cast<std::uint64_t>(*(array8bytes+1)) << 48) | (static_cast<std::uint64_t>(*(array8bytes+2)) << 40) | (static_cast<std::uint64_t>(*(array8bytes+3)) << 32) | (static_cast<std::uint64_t>(*(array8bytes+4)) << 24) | (static_cast<std::uint64_t>(*(array8bytes+5)) << 16) | (static_cast<std::uint64_t>(*(array8bytes+6)) << 8) | (static_cast<std::uint64_t>(*(array8bytes+7))) );

}

bool const existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
    std::uint16_t buff_sizet = 0;
    for(int e = 0 ;e < ciclos ; e++){
        data2 = data2 + salto*e;
        buff_sizet = 0;
        for(int u = 0; u < data_len; u++){
            if(data1[u] == data2[u]){
                buff_sizet++;
            }
        }
        if(buff_sizet == data_len){
            return true;
        }
    }

    return false;
}

bool const existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
    std::uint16_t buff_sizet = 0;
    std::uint16_t addr_keyhash_buffer_len = 0;
    for(int e = 0 ;e < ciclos ; e++){

        output = output + addr_keyhash_buffer_len;
        buff_sizet = 0;
        for(int u = 0; u < data_len; u++){
            if(data[u] == output[u+1]){
                buff_sizet++;
            }
        }
        if(buff_sizet == data_len){
            return true;
        }
        addr_keyhash_buffer_len = output[0] + salto ;
    }

    return false;
}

std::uint8_t const *const hexchararray2uint8array(std::string const & string_hex, std::size_t * const hexchararray2uint8array_len){
    //se crea una memoria dinamica para un nuevo array char_hexa[]
    std::size_t array_hex_len = static_cast<std::size_t>( string_hex.size() / 2 );
    if(hexchararray2uint8array_len != nullptr){
        *hexchararray2uint8array_len = array_hex_len;
    }
    std::uint8_t *array_hex = new std::uint8_t[array_hex_len];
    for(std::size_t ha = 0; ha < array_hex_len; ha++){
        array_hex[ha] = static_cast<std::uint8_t>(std::stoul(string_hex.substr(ha*2,2),nullptr,16));
    };

    return array_hex;
};
