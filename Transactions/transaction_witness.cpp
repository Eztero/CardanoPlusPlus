#include "transaction_witness.hpp"

TransactionWitness::TransactionWitness(std::vector <std::uint8_t> * const witness_cbor_out) : CborSerialize(witness_cbor_out){
    vkeywitness_count = 0;
    witnessmapcountbit = 0;
    buff_sizet = 0;
}

bool TransactionWitness::existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
    buff_sizet = 0;
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

TransactionWitness &TransactionWitness::addVkeyWitness(std::uint8_t const *const public_key, std::uint8_t const * const signature_transactionbody ){
 //32 (public_key) + 64 (signature) = 96 bytes

 if(!existen_coincidencias(public_key,vkeywitness.data(),32,vkeywitness_count,96)){
    buff_sizet = static_cast<std::size_t>( vkeywitness.capacity() ) - static_cast<std::size_t>( vkeywitness.size() );
        if(buff_sizet < 28){
            vkeywitness.reserve(vkeywitness.size() + 96);
        }
    vkeywitness.insert(vkeywitness.end(),public_key,public_key+32);
    vkeywitness.insert(vkeywitness.end(),signature_transactionbody,signature_transactionbody+64);
    vkeywitness_count++;

    witnessmapcountbit |= 0x01;
 }
    return *this;
}

void TransactionWitness::Build(){
    ClearCbor();
    std::uint8_t contador = 0;

    if(witnessmapcountbit > 0){

    for(std::uint8_t x = 0; x < 7; x++ ){ //se realiza un conteo de los mapas que existen en el transaccion witness
        contador += (witnessmapcountbit >> x ) & 0x01;
    }

    createMap(contador);                                          /// { }
    for(std::uint8_t x = 0; x < 7; x++ ){ //se asignan los datos

    if((witnessmapcountbit >> x ) & 0x01){  //revisa cada item del transaction witness
    switch(x){
        case 0:{
            ptrvec = vkeywitness.data();
            addIndexMap(0);                                        /// ? 0:
            createArray(vkeywitness_count);                        /// [ ]
            for(std::uint16_t v = 0; v < vkeywitness_count;v++){
                contador = v*96;
                createArray(2);                                    /// [ , ]
                addBytesArray(&ptrvec[contador], 32);              /// [vkey, ]
                addBytesArray(&ptrvec[32+contador],64);            /// [vkey,signature]
            }

        };break;
        case 1:{};break;
        case 2:{};break;
        case 3:{};break;
        case 4:{};break;
        case 5:{};break;
        case 6:{};break;
    }
            }
    }

    }else{
        createArray(0);
    }

}
