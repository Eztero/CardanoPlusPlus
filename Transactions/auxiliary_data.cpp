#include "auxiliary_data.hpp"

AuxiliaryData::AuxiliaryData(std::vector <std::uint8_t> * const auxiliary_data_cbor_out) : CborSerialize(auxiliary_data_cbor_out) {
    buff_sizet=0;
    auxiliarymapcountbit = 0;
    metadata_count = 0;
}

bool AuxiliaryData::arethereAuxiliaryData() const{
    return (auxiliarymapcountbit > 0) ? true : false;
}

void AuxiliaryData::Build(){
    ClearCbor();
    std::uint8_t contador = 0;

    if(auxiliarymapcountbit > 0){

        for(std::uint8_t x = 0; x < 4; x++ ){ //se realiza un conteo de los mapas que existen en auxiliary data
            contador += (auxiliarymapcountbit >> x ) & 0x01;
        }
        createMap(contador);   /// { }
        for(std::uint8_t a = 0; a < 4; a++ ){ //se asignan los datos

            if((auxiliarymapcountbit >> a ) & 0x01){  //revisa cada item del transaction witness
                switch(a){
                case 0:{};break;
                case 1:{};break;
                case 2:{};break;
                case 3:{};break;
                }
            }
        }
    }
    else{
        addNull();
    }

}
