#include "transaction_witness.hpp"

TransactionWitness::TransactionWitness(){
    ptrvec = nullptr;
    vkeywitness_count = 0;
    witnessmapcountbit = 0;
    buff_sizet = 0;
}

TransactionWitness &TransactionWitness::addVkeyWitness(std::uint8_t const *const public_key, std::uint8_t const * const signature_transactionbody ){
 //32 (public_key) + 64 (signature) = 96 bytes

 if(vkeywitness_count < UINT16_MAX && !existen_coincidencias(public_key,vkeywitness.data(),32,vkeywitness_count,96)){
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

std::vector<std::uint8_t> const &TransactionWitness::Build(){
    CborSerialize cbor(&cbor_TransactionWitness);
    cbor.ClearCbor();
    std::uint8_t contador = 0;

    if(witnessmapcountbit > 0){

    for(std::uint8_t x = 0; x < 7; x++ ){ //se realiza un conteo de los mapas que existen en el transaccion witness
        contador += (witnessmapcountbit >> x ) & 0x01;
    }

    cbor.createMap(contador);                                          /// { }
    for(std::uint8_t x = 0; x < 7; x++ ){ //se asignan los datos

    if((witnessmapcountbit >> x ) & 0x01){  //revisa cada item del transaction witness
    switch(x){
        case 0:{
            ptrvec = vkeywitness.data();
            cbor.addIndexMap(static_cast<uint64_t>(0));                 /// ? 0:
            cbor.createArray(vkeywitness_count);                        /// [ ]
            for(std::uint16_t v = 0; v < vkeywitness_count;v++){
                cbor.createArray(2);                                    /// [ , ]
                cbor.addBytesArray(&ptrvec[0], 32);                     /// [vkey, ]
                cbor.addBytesArray(&ptrvec[32], 64);                    /// [vkey,signature]
                ptrvec += 96;
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
        cbor.createArray(0);
    }
return cbor_TransactionWitness;
}
