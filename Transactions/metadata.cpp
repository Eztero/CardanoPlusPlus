#include "metadata.hpp"
Metadatas::Metadatas(){
    metadata_count = 0;
    }

Metadatas &Metadatas::addMetadata(std::uint64_t const keytag, std::vector<std::uint8_t> const &metadataincbor){
// 8 (keytag) + 8(metadataincbor.size) + UINT16_MAX (metadataincbor) = 16bytes + UINT16_MAX bytes

if(metadata_count < UINT16_MAX && metadataincbor.size() < UINT16_MAX){
    agregarUint64BytestoVector(metadata, keytag);
    agregarUint64BytestoVector(metadata, metadataincbor.size());
    metadata.insert(metadata.end(),metadataincbor.begin(),metadataincbor.end());
    metadata_count++;
}
return *this;
}

bool Metadatas::arethereMetadatas() const {
    return metadata_count > 0 ? true : false;
}

std::vector<std::uint8_t> const &Metadatas::getMetadatas(){

    if(metadata_count > 0){

        CborSerialize cbor(&metadata_cbor);
        ptrvec = metadata.data();
        std::uint16_t metadataLen = 0;

        cbor.createMap(metadata_count);                              /// { }
        for(std::uint16_t m = 0 ; m < metadata_count; m++){

            cbor.addIndexMap(ptrvec);                                /// transaction_metadatum_label :
            metadataLen = (
                        static_cast<std::uint16_t>(ptrvec[14]) << 8  ) |
                        static_cast<std::uint16_t>(ptrvec[15]
                          ); // se obtiene solo los 16bytes
            cbor.bypassPtrUint8Cbor( &ptrvec[16], metadataLen );     /// transaction_metadatum_label : transaction_metadatum

            ptrvec += 15 + metadataLen; //16 + metadataLen - 1 = 15 + metadataLen, Se resta uno menos en puntero

        }
    }
    return metadata_cbor;
}
