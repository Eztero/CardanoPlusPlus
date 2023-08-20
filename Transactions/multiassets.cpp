#include "multiassets.hpp"
namespace Cardano{
Multiassets::Multiassets(){
//cbor = new CborSerialize( &buffer_cbor ); ///si no puede asignar memoria que lanze un error
}

Multiassets::~Multiassets(){
}

Multiassets &Multiassets::addAsset(std::uint8_t const *const policyID, std::uint8_t const *const assetname, std::size_t const &assetname_len, std::uint64_t const amount){
    if( capsula.size() == 0 ){ //si no se han creado datos en la capsula se crea el primer dato
                capsula.push_back(std::vector<std::uint8_t>(0));
                capsula[0].push_back(0); //se agrega el primer elemento del array
                capsula[0].reserve(30);
    }
    std::uint8_t igual = 0;  // indica si el policyid es el mismo
    std::size_t posicion = 0; //parte de la posicion cero
    std::vector<std::uint8_t>::iterator it = capsula[0].begin();

    for(int b = 0;b<capsula[0][0];b++){ //solo si existen policyID en capsula[0] se entra a comparar
        posicion += 1; // se pasa a la siguiente posicion (columna)
        igual = 0;
        for(int a = 0; a < 28 ;a++){  // compara en largos de 28bytes, revisa si se repiten los policyID
            ++it;  // inicia saltando a la primera posicion
            if(*it == policyID[a]){
                igual++;
            }
        }
        if(igual == 28){    //si encuentra similitud sale del bucle
            break;  // o it = capsula[0].end();
        }

    }
    if(assetname_len < 32){ // el largo del nombre no debe exceder los 32 bytes
        if(igual == 28){

            cbor.clearCbor();
            cbor.addBytesArray(assetname, assetname_len);
            cbor.addUint(amount);
            std::vector<std::uint8_t> const & buffer_cbor = cbor.getCbor();
            capsula[posicion][0] += 1;  ///aumenta la cantidad de elementos en el mapa ///PONER LIMITE A 254 elementos
            capsula[posicion].insert( capsula[posicion].end(), buffer_cbor.begin(), buffer_cbor.end() ); //inserta los datos en cbor

        }else{
            capsula[0][0] += 1;
            capsula[0].insert (capsula[0].end(), policyID, policyID+28 ); //agrego el nuevo policyID, a la primera columna
            capsula.push_back(std::vector<std::uint8_t>(0));  //se crea otra columna para almacenar los datos de esa posicion
            posicion += 1; // se pasa a la siguiente posicion (columna)
            capsula[posicion].push_back(0); //se agrega el primer elemento del array en esa posicion

            cbor.clearCbor();
            cbor.addBytesArray(assetname, assetname_len);
            cbor.addUint(amount);
            std::vector<std::uint8_t> const & buffer_cbor = cbor.getCbor();
            capsula[posicion][0] += 1;  //aumenta la cantidad de elementos en el mapa
            capsula[posicion].insert( capsula[posicion].end(), buffer_cbor.begin(), buffer_cbor.end() ); //inserta los datos en cbor

        }
    }
    return *this;
}

Multiassets &Multiassets::addAsset(std::uint8_t const *const policyID, std::string assetname, std::uint64_t const amount){

    std::uint8_t assetname_len = assetname.size();
    std::uint8_t asset_name[assetname_len] = {};
    for(std::uint8_t a = 0; a < assetname_len; a ++){
        asset_name[a] = static_cast<uint8_t>(assetname[a]);

    }
    addAsset(policyID, asset_name, assetname_len, amount);

    return *this;
}

std::vector<std::uint8_t> const &Multiassets::getCborMultiassets(){
    cbor.clearCbor();
    int policyID_count = static_cast<std::uint64_t>(capsula[0][0]);
    if(policyID_count != 0){ // si no esta vacio se procede con el resto
    std::uint8_t *ptr_policyID = capsula[0].data();
    ptr_policyID++; //salta la primera posicion
    cbor.createMap(capsula[0][0]);

    policyID_count += 1; //aumento en uno, para que concuerde la posicion de los datos en las otras columnas
    for(int a = 1; a < policyID_count;a++){
     cbor.addBytesArray(ptr_policyID,28);
     cbor.createMap(static_cast<std::uint64_t>(capsula[a][0]));
     cbor.bypassIteratorVectorCbor(capsula[a].begin()+1, capsula[a].end());
     ptr_policyID += 28;
    }
    }
    return cbor.getCbor();
}
}
