#include "cbor_lite.hpp"

CborSerialize::CborSerialize(std::vector <std::uint8_t> * const cbor_data_out)
{
    bytes_cbor_data = cbor_data_out;
    //cbor_data_out->clear();
    //if(bytes_cbor_data->capacity() < 150 ){
    //   bytes_cbor_data->reserve(150);
    //}

};

CborSerialize::~CborSerialize()
{
    bytes_cbor_data = nullptr;
};

void CborSerialize::AddSize2Vector(std::uint64_t const &size_array, Pos_hex const &pos){
    switch(pos){
    case Pos_hex::hff : { // < 0x100
        bytes_cbor_data->push_back( size_array );
    };break;
    case Pos_hex::hff2 : { // < 0x10000
        bytes_cbor_data->push_back( ( size_array >> 8 ) & 0xff);
        bytes_cbor_data->push_back( ( size_array ) & 0xff);
    };break;
    case Pos_hex::hff4 : { // < 0x100000000
        bytes_cbor_data->push_back( ( size_array >> 24 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 16 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 8 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array ) & 0xff );
    };break;
    case Pos_hex::hff8 : { // < std::UINT64_MAX
        bytes_cbor_data->push_back( ( size_array >> 56 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 48 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 40 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 32 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 24 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 16 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array >> 8 ) & 0xff );
        bytes_cbor_data->push_back( ( size_array ) & 0xff );
    };break;
    }
};

CborSerialize &CborSerialize::addBytesArray(std::uint8_t const *const bytes, std::uint64_t bytes_length){

    if(bytes_length < 0x18){//0...23
        bytes_cbor_data->push_back( bytes_length + 0x40 );
    }

    else if(bytes_length < 0x100){ //24...255
        bytes_cbor_data->push_back( 0x58 );
        AddSize2Vector(bytes_length, Pos_hex::hff);
    }

    else if(bytes_length < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0x59);
        AddSize2Vector(bytes_length, Pos_hex::hff2);
    }

    else if(bytes_length < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0x5a );
        AddSize2Vector(bytes_length, Pos_hex::hff4);

    }

    else if(bytes_length < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0x5b );
        AddSize2Vector(bytes_length, Pos_hex::hff8);

    }

    bytes_cbor_data->insert(bytes_cbor_data->end(), bytes, bytes + bytes_length);

    return *this;
};

CborSerialize &CborSerialize::bypassVectorCbor(std::vector<std::uint8_t> const &vectorCbor){
    bytes_cbor_data->insert(bytes_cbor_data->end(), vectorCbor.begin(), vectorCbor.end());
    return *this;
}

CborSerialize &CborSerialize::bypassPtrUint8Cbor( std::uint8_t const * const ptrArrayCbor, std::uint16_t ptrArrayCbor_len ){
    bytes_cbor_data->insert(bytes_cbor_data->end(), ptrArrayCbor, ptrArrayCbor + ptrArrayCbor_len);
    return *this;
}



CborSerialize &CborSerialize::addUint(std::uint64_t const number){ //dicierne entre numero negativo y positivo
    if(number < 0x18){//0...23
        bytes_cbor_data->push_back( number + 0x00 );
    }

    else if(number < 0x100){ //24...255
        bytes_cbor_data->push_back( 0x18 );
        AddSize2Vector(number, Pos_hex::hff);
    }

    else if(number < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0x19);
        AddSize2Vector(number, Pos_hex::hff2);
    }

    else if(number < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0x1a );
        AddSize2Vector(number, Pos_hex::hff4);

    }

    else if(number < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0x1b );
        AddSize2Vector(number, Pos_hex::hff8);
    }
    return *this;
};

CborSerialize &CborSerialize::addUint(std::uint8_t const * const arraynumbe8byteshex){
    std::uint64_t const number = ( static_cast<std::uint64_t>(arraynumbe8byteshex[0]) << 56 ) | (static_cast<std::uint64_t>(arraynumbe8byteshex[1]) << 48) | (static_cast<std::uint64_t>(arraynumbe8byteshex[2]) << 40) |
            ( static_cast<std::uint64_t>(arraynumbe8byteshex[3]) << 32 ) | (static_cast<std::uint64_t>(arraynumbe8byteshex[4]) << 24) | (static_cast<std::uint64_t>(arraynumbe8byteshex[5]) << 16) |
            ( static_cast<std::uint64_t>(arraynumbe8byteshex[6]) << 8  ) | static_cast<std::uint64_t>(arraynumbe8byteshex[7]);


    if(number < 0x18){//0...23
        bytes_cbor_data->insert(bytes_cbor_data->end(),arraynumbe8byteshex+7,arraynumbe8byteshex+8);
    }

    else if(number < 0x100){ //24...255
        bytes_cbor_data->push_back( 0x18 );
        bytes_cbor_data->insert(bytes_cbor_data->end(),arraynumbe8byteshex+7,arraynumbe8byteshex+8);
    }

    else if(number < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0x19);
        bytes_cbor_data->insert(bytes_cbor_data->end(),arraynumbe8byteshex+6,arraynumbe8byteshex+8);
    }

    else if(number < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0x1a );
        bytes_cbor_data->insert(bytes_cbor_data->end(),arraynumbe8byteshex+4,arraynumbe8byteshex+8);
    }

    else if(number < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)
        bytes_cbor_data->push_back( 0x1b );
        bytes_cbor_data->insert(bytes_cbor_data->end(),arraynumbe8byteshex,arraynumbe8byteshex+8);
    }
    return *this;
}

CborSerialize &CborSerialize::createArray(std::uint64_t const size_array){
    if(size_array < 0x18){//0...23
        bytes_cbor_data->push_back( size_array + 0x80 );
    }

    else if(size_array < 0x100){ //24...255
        bytes_cbor_data->push_back( 0x98 );
        AddSize2Vector(size_array, Pos_hex::hff);
    }

    else if(size_array < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0x99);
        AddSize2Vector(size_array, Pos_hex::hff2);
    }

    else if(size_array < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0x9a );
        AddSize2Vector(size_array, Pos_hex::hff4);
    }

    else if(size_array < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0x9b );
        AddSize2Vector(size_array, Pos_hex::hff8);


    }
    return *this;
};

CborSerialize &CborSerialize::createMap(std::uint64_t const size_array){
    if(size_array < 0x18){//0...23
        bytes_cbor_data->push_back( size_array + 0xa0 );
    }

    else if(size_array < 0x100){ //24...255
        bytes_cbor_data->push_back( 0xb8 );
        AddSize2Vector(size_array, Pos_hex::hff);
    }

    else if(size_array < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0xb9);
        AddSize2Vector(size_array, Pos_hex::hff2);
    }

    else if(size_array < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0xba );
        AddSize2Vector(size_array, Pos_hex::hff4);

    }

    else if(size_array < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0xbb );
        AddSize2Vector(size_array, Pos_hex::hff8);

    }

    return *this;
};

CborSerialize &CborSerialize::addBool(bool const b){
    switch(b){
    case true :{bytes_cbor_data->push_back( 0xf5 );};break;
    case false :{bytes_cbor_data->push_back( 0xf4 );};break;
    }
    return *this;
};

CborSerialize &CborSerialize::addNull(){
    bytes_cbor_data->push_back( 0xf6 );
    return *this;
};

CborSerialize &CborSerialize::addString(std::string const &text){
    std::uint64_t size_array = text.size();
    if(size_array < 0x18){//0...23
        bytes_cbor_data->push_back( size_array + 0x60 );
    }

    else if(size_array < 0x100){ //24...255
        bytes_cbor_data->push_back( 0x78 );
        AddSize2Vector(size_array, Pos_hex::hff);
    }

    else if(size_array < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0x79);
        AddSize2Vector(size_array, Pos_hex::hff2);
    }

    else if(size_array < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0x7a );
        AddSize2Vector(size_array, Pos_hex::hff4);

    }

    else if(size_array < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0x7b );
        AddSize2Vector(size_array, Pos_hex::hff8);

    }

    bytes_cbor_data->insert(bytes_cbor_data->end(), text.begin(), text.end());

    return *this;
};

CborSerialize &CborSerialize::addIndexMap(std::uint64_t const index){
    addUint(index);
    return *this;
};

CborSerialize &CborSerialize::addIndexMap(std::string const &text){
    addString(text);
    return *this;
};

CborSerialize &CborSerialize::addIndexMap(std::uint8_t const *const bytesarray, std::uint64_t bytesarray_length){
    addBytesArray(bytesarray, bytesarray_length);
    return *this;
};

CborSerialize &CborSerialize::addIndexMap(std::uint8_t const * const arraynumbe8byteshex){
    addUint(arraynumbe8byteshex);
    return *this;
};

CborSerialize &CborSerialize::addTag(std::uint64_t const number){
    if(number < 0x18){//0...23
        bytes_cbor_data->push_back( number + 0xc0 );
    }

    else if(number < 0x100){ //24...255
        bytes_cbor_data->push_back( 0xd8 );
        AddSize2Vector(number, Pos_hex::hff);
    }

    else if(number < 0x10000){//256...65535  (uint16)
        bytes_cbor_data->push_back(0xd9);
        AddSize2Vector(number, Pos_hex::hff2);
    }

    else if(number < 0x100000000){// 65536...4294967295 (uint32)
        bytes_cbor_data->push_back( 0xda );
        AddSize2Vector(number, Pos_hex::hff4);

    }

    else if(number < UINT64_MAX){ // 4294967296...18446744073709551615 (uint64)

        bytes_cbor_data->push_back( 0xdb );
        AddSize2Vector(number, Pos_hex::hff8);
    }

    return *this;
};

void CborSerialize::ClearCbor(){
    bytes_cbor_data->clear();
    if(bytes_cbor_data->capacity() < 150 ){
        bytes_cbor_data->reserve(150);
    }
};
