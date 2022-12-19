#include "auxiliary_data.hpp"

AuxiliaryData::AuxiliaryData() : Metadatas(){
    ptrvec = nullptr;
    auxiliarymapcountbit = 0;
}


bool AuxiliaryData::arethereAuxiliaryData() const{
    return (arethereMetadatas()) ? true : false;
}

std::vector<std::uint8_t> const &AuxiliaryData::Build(){
    if(arethereMetadatas()){
        auxiliarymapcountbit |= 0x01;
    }

    CborSerialize cbor(&cborAuxiliaryData);
    cbor.ClearCbor();
    std::uint8_t contador = 0;

    if(auxiliarymapcountbit > 0){

        for(std::uint8_t x = 0; x < 4; x++ ){ //se realiza un conteo de los mapas que existen en auxiliary data
            contador += (auxiliarymapcountbit >> x ) & 0x01;
        }
        cbor.addTag(259);
        cbor.createMap(contador);   /// { }
        for(std::uint8_t a = 0; a < 4; a++ ){ //se asignan los datos

            if((auxiliarymapcountbit >> a ) & 0x01){  //revisa cada item del transaction witness
                switch(a){
                case 0:{
                cbor.addIndexMap(static_cast<uint64_t>(0));    /// 0 :
                cbor.bypassVectorCbor(getCborMetadatas());       /// Metadata
                };break;
                case 1:{};break;
                case 2:{};break;
                case 3:{};break;
                }
            }
        }
    }
    else{
        cbor.addNull();
    }
return cborAuxiliaryData;
}
