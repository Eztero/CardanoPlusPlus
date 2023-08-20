#include "native_scripts.hpp"
namespace Cardano{
NativeScripts::NativeScripts(){
    eoc = 0; //espacio ocupado en capsula
    pec = 0; // posicion en capsula
    condiciones.reserve(100);
    ppec.reserve(100);
    //vector_buffer.reserve(42);
}

NativeScripts & NativeScripts::requireAllOf(){
    if(eoc != 0){
        capsula[pec][0] += 1;
        ppec.push_back(pec);
        capsula.push_back( std::vector<std::uint8_t>(0) );
    }else{
        ppec.push_back(0);
        ppec.push_back(pec); // comienza del 1
        capsula.push_back( std::vector<std::uint8_t>(0) );
        capsula.push_back( std::vector<std::uint8_t>(0) );// comienza del 1
    }
    eoc ++;
    pec = eoc;
    condiciones.push_back(0x11);
    condiciones.push_back(eoc);
    capsula[eoc].reserve(224); //reserva 224 bytes para trabajar
    capsula[eoc].push_back(0); //inicia el array de la capsula en cero, el primer array indica la cantidad de elementos de la capsula

    return *this;
}

NativeScripts & NativeScripts::requireAnyOf(){
    if(eoc != 0){
        capsula[pec][0] += 1;
        ppec.push_back(pec);
        capsula.push_back( std::vector<std::uint8_t>(0) );
    }else{
        ppec.push_back(0);
        ppec.push_back(pec); // comienza del 1
        capsula.push_back( std::vector<std::uint8_t>(0) );
        capsula.push_back( std::vector<std::uint8_t>(0) );// comienza del 1
    }
    eoc ++;
    pec = eoc;
    condiciones.push_back(0x12);
    condiciones.push_back(eoc);
    capsula[eoc].reserve(224); //reserva 224 bytes para trabajar
    capsula[eoc].push_back(0); //inicia el array de la capsula en cero, el primer array indica la cantidad de elementos de la capsula

    return *this;
}

NativeScripts & NativeScripts::requireAtLeastNOf(std::uint64_t const n){
    if(eoc != 0){
        capsula[pec][0] += 1;
        ppec.push_back(pec);
        capsula.push_back( std::vector<std::uint8_t>(0) );
    }else{
        ppec.push_back(0);
        ppec.push_back(pec); // comienza del 1
        capsula.push_back( std::vector<std::uint8_t>(0) );
        capsula.push_back( std::vector<std::uint8_t>(0) );// comienza del 1
    }
    eoc ++;
    pec = eoc;
    condiciones.push_back(0x13);
    Utils::addUint64toVector(condiciones, n); //agrega 8 bytes referente a la cantidad K de "N de K"
    condiciones.push_back( eoc );
    capsula[eoc].reserve(224); //reserva 224 bytes para trabajar
    capsula[eoc].push_back(0); //inicia el array de la capsula en cero, el primer array indica la cantidad de elementos de la capsula

    return *this;
}

NativeScripts & NativeScripts::thisIsValidBeforeSlot(std::uint64_t const slot){
    capsula[pec][0] += 1;  // Agrega +1 a la cantidad de elementos
    //CborSerialize cbor( &vector_buffer );
    cbor.createArray(2).addUint(4).addUint( slot );
    std::vector<std::uint8_t> const & vector_buffer = cbor.getCbor();
    capsula[pec].insert(capsula[pec].end(), vector_buffer.begin(), vector_buffer.end()); // Agrega el elemento serializado en cbor a la capsula
    //vector_buffer.clear();
    cbor.clearCbor();

    return *this;
}

NativeScripts & NativeScripts::thisIsValidAfterSlot(std::uint64_t const slot){
    capsula[pec][0] += 1;  // Agrega +1 a la cantidad de elementos
    //CborSerialize cbor( &vector_buffer );
    cbor.createArray(2).addUint(5).addUint( slot );
    std::vector<std::uint8_t> const & vector_buffer = cbor.getCbor();
    capsula[pec].insert(capsula[pec].end(), vector_buffer.begin(), vector_buffer.end()); // Agrega el elemento serializado en cbor a la capsula
    //vector_buffer.clear();
    cbor.clearCbor();

    return *this;
}

NativeScripts & NativeScripts::SignatureOf(std::uint8_t const *const vk_hash){
    capsula[pec][0] += 1;  // Agrega +1 a la cantidad de elementos
    //CborSerialize cbor( &vector_buffer );
    cbor.createArray(2).addUint( static_cast<std::uint8_t>(0) ).addBytesArray(vk_hash, 28);
    std::vector<std::uint8_t> const & vector_buffer = cbor.getCbor();
    capsula[pec].insert(capsula[pec].end(), vector_buffer.begin(), vector_buffer.end());  // Agrega el elemento serializado en cbor a la capsula
    //vector_buffer.clear();
    cbor.clearCbor();

    return *this;
}

NativeScripts & NativeScripts::SignatureOf(std::string const payment_address){
    std::uint16_t bufferbech32_len = 0;
    if(Hash::bech32_decode(payment_address.c_str(), bufferbech32, &bufferbech32_len) && bufferbech32_len == 29){
        SignatureOf( &bufferbech32[1] );
    }

    return *this;
}

NativeScripts & NativeScripts::Endrequire(){
    pec = ppec[pec];

    return *this;
}

std::vector<std::uint8_t> const &NativeScripts::getCborNativeScripts(){
    //CborSerialize cbor( &cbornativescript );
    cbor.clearCbor();
    std::vector<std::uint8_t>::iterator it = condiciones.begin();
    while(it != condiciones.end()){
        switch(*it){
        case 0x11:{
            ++it; //pasa a donde muestra la posicion de los datos de la capsula
            cbor.createArray(2);
            cbor.addUint(1);
            cbor.createArray( capsula[*it][0] ); //agrega al array la cantidad de elementos que tiene la capsula
            cbor.bypassIteratorVectorCbor( capsula[*it].begin()+1, capsula[*it].end() );
        };break;
        case 0x12:{
            ++it;
            cbor.createArray(2);
            cbor.addUint(2);
            cbor.createArray( capsula[*it][0] ); //agrega al array la cantidad de elementos que tiene la capsula
            cbor.bypassIteratorVectorCbor( capsula[*it].begin()+1, capsula[*it].end() );
        };break;
        case 0x13:{
            ++it;
            cbor.createArray(3);
            cbor.addUint(3);
            cbor.addUint(&(*it)); //Crea una referencia uint8t al valor entregado por It, segun la funcion solo leera los primeros 8 bytes del vector
            it+=8;
            cbor.createArray( capsula[*it][0] ); //agrega al array la cantidad de elementos que tiene la capsula
            cbor.bypassIteratorVectorCbor( capsula[*it].begin()+1, capsula[*it].end() );
        };break;
        default:{
            ++it;
        };break;
        }
    }
    return cbor.getCbor();
    //return cbornativescript;
}
}

