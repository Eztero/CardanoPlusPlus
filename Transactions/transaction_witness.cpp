#include "transaction_witness.hpp"
namespace Cardano{

TransactionWitness::TransactionWitness(){
    ptrvec = nullptr;
    vkeywitness_count = 0;
    witnessmapcountbit = 0;
    buff_sizet = 0;
}

TransactionWitness::~TransactionWitness(){
    ptrvec = nullptr;
}

TransactionWitness &TransactionWitness::addVkeyWitness( std::uint8_t const * const public_key, std::uint8_t const * const signature_transactionbody ){
    //32 (public_key) + 64 (signature) = 96 bytes

    if(vkeywitness_count < UINT16_MAX && !Utils::existen_coincidencias(public_key,vkeywitness.data(),32,vkeywitness_count,96)){ //comprueba de que no se repitan los publickey
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

// ? 1: [* native_script ]
// esquema:
// para este caso no es necesario incorporar el largo de la cadena del cborNativeScript ya que viene seriealizado en cbor
// y todos ellos iran incluido en un array de datos por lo que solo se necesita la cantidad de elementos de este array
// |cantidad | nativescript1 |  nativescript2 | ... |
// Recibe los native_script de las transacciones input

//TransactionWitness &TransactionWitness::addNativeScript( std::uint8_t const *const cborNativeScript, std::size_t const cborNativeScript_len ){
//    cbor_native_script.assign(cborNativeScript, cborNativeScript + cborNativeScript_len );
//    return *this;
//}

TransactionWitness & TransactionWitness::addNativeScript( std::vector<std::uint8_t> const & cborNativeScript ){

    cbor_native_script = &cborNativeScript;
    witnessmapcountbit |= 0x02;

    return *this;
}

// ? 5: [*redeemer]
// redeemer = [ tag: redeemer_tag, index: uint, data: plutus_data, ex_units: ex_units ]
// esquema:  |cantidad | redeemer1 |  redeemer2 | ... |
// Recibe los redeemer de las transacciones input
TransactionWitness & TransactionWitness::addRedeemer(std::vector <std::uint8_t> const & cborRedeemers ){

    cbor_redeemers.assign(cborRedeemers.begin(), cborRedeemers.end());
    witnessmapcountbit |= 0X20;

    return *this;
}

//? 4: [* plutus_data ]
// esquema:  |cantidad | datum1 | datum2 | ... |
// Recibe los datum (en cbor , no hash32) de las transacciones input y output
TransactionWitness & TransactionWitness::addDatum(std::vector <std::uint8_t> const & cborDatums){

    cbor_datums.assign(cborDatums.begin(), cborDatums.end());
    witnessmapcountbit |= 0X10;

    return *this;
}

// ? 3: [* plutus_v1_script ]
// esquema:  |cantidad | plutus_v1_script1 | plutus_v1_script2 | ... |
// Recibe los plutus_v1_script de las transacciones input
TransactionWitness & TransactionWitness::addPlutusV1Script(std::vector <std::uint8_t> const & cborPlutusV1Scripts){
    cbor_plutusv1scripts = &cborPlutusV1Scripts;
    //cbor_plutusv1scripts.assign(cborPlutusV1Scripts.begin(), cborPlutusV1Scripts.end());
    witnessmapcountbit |= 0x08;
    return *this;
}

// ? 6: [* plutus_v2_script ]
// esquema:  |cantidad | plutus_v2_script1 | plutus_v2_script2 | ... |
// Recibe los plutus_v2_script de las transacciones input
TransactionWitness & TransactionWitness::addPlutusV2Script(std::vector <std::uint8_t> const & cborPlutusV2Scripts){
    cbor_plutusv2scripts = &cborPlutusV2Scripts;
    //cbor_plutusv2scripts.assign(cborPlutusV2Scripts.begin(), cborPlutusV2Scripts.end());
    witnessmapcountbit |= 0x40;
    return *this;
}

std::vector<std::uint8_t> const & TransactionWitness::Build(){
    //CborSerialize cbor(&cbor_TransactionWitness);
    cbor.clearCbor();
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
                case 1:{
                    cbor.addIndexMap(static_cast<uint64_t>(1));                 /// ? 1:
                    cbor.createArray(Utils::extract2bytestoUint16(cbor_native_script->data()));
                    cbor.bypassIteratorVectorCbor(cbor_native_script->begin() + 2,cbor_native_script->end());
                };break;
                case 2:{

                };break;
                case 3:{
                    cbor.addIndexMap(static_cast<uint64_t>(3));                 /// ? 3:
                    cbor.createArray(Utils::extract2bytestoUint16(cbor_plutusv1scripts->data()));
                    cbor.bypassIteratorVectorCbor(cbor_plutusv1scripts->begin() + 2, cbor_plutusv1scripts->end());
                };break;
                case 4:{
                    cbor.addIndexMap(static_cast<uint64_t>(4));                 /// ? 4:
                    cbor.bypassVectorCbor(cbor_datums);
                };break;
                case 5:{
                    cbor.addIndexMap(static_cast<uint64_t>(5));                 /// ? 5:
                    cbor.bypassVectorCbor(cbor_redeemers);
                };break;
                case 6:{
                    cbor.addIndexMap(static_cast<uint64_t>(6));                 /// ? 6:
                    cbor.createArray(Utils::extract2bytestoUint16(cbor_plutusv2scripts->data()));
                    cbor.bypassIteratorVectorCbor(cbor_plutusv2scripts->begin() + 2, cbor_plutusv2scripts->end());
                };break;
                }
            }
        }

    }else{
        cbor.createMap(0);
    }
    return cbor.getCbor();
}

}
