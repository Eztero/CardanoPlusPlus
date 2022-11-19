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

bool existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
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

bool existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
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

