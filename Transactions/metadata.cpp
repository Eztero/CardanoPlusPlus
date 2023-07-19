#include "metadata.hpp"
Metadatas::Metadatas(){
    metadata_count = 0;
    ptrvec = nullptr;
    }

void Metadatas::addMetadata(std::uint64_t const keytag, std::vector<std::uint8_t> const &CborMetadata){
// 8 (keytag) + 8(CborMetadata.size) + UINT16_MAX (CborMetadata) = 16bytes + UINT16_MAX bytes

if(metadata_count < UINT16_MAX && CborMetadata.size() < UINT16_MAX){
    addUint64toVector(metadata, keytag);
    addUint64toVector(metadata, CborMetadata.size());
    metadata.insert(metadata.end(),CborMetadata.begin(),CborMetadata.end());
    metadata_count++;
}
}

bool Metadatas::arethereMetadatas() const {
    return metadata_count > 0 ? true : false;
}

std::vector<std::uint8_t> const & Metadatas::getCborMetadatas(){

    if(metadata_count > 0){

        //CborSerialize cbor(&metadata_cbor);
        cbor.clearCbor();
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
    return cbor.getCbor();
}
