#include "transaction_body.hpp"
//https://www.rfc-editor.org/rfc/rfc8610


TransaccionBody::TransaccionBody(std::vector <std::uint8_t> * const transactionbody_cbor_out) : CborSerialize(transactionbody_cbor_out) {
    fee = 0;
    buff_sizet = 0;
    ttl = 0;
    bodymapcountbit = 0;
    output_count = 0;
    input_count = 0;
    withdrawals_count = 0;
    ptr_data = nullptr;

}

TransaccionBody::~TransaccionBody(){
    ptr_data=nullptr;
}

void TransaccionBody::addUint64BytestoVector(std::vector <std::uint8_t> & bytesvector, std::uint64_t const & numero){
    bytesvector.push_back( ( numero >> 56 ) & 0xff );
    bytesvector.push_back( ( numero >> 48 ) & 0xff );
    bytesvector.push_back( ( numero >> 40 ) & 0xff );
    bytesvector.push_back( ( numero >> 32 ) & 0xff );
    bytesvector.push_back( ( numero >> 24 ) & 0xff );
    bytesvector.push_back( ( numero >> 16 ) & 0xff );
    bytesvector.push_back( ( numero >> 8 ) & 0xff );
    bytesvector.push_back( ( numero ) & 0xff );

}



TransaccionBody &TransaccionBody::addTransactionsInput(std::uint8_t const *const TxHash, std::uint64_t const TxIx){   //0 : set<transaction_input> --> transaction_input = [ transaction_id : hash32, index : uint]
    /// 32(TxHash) + 8(TxIx) = 40 bytes de largo cada input

    if(!existen_coincidencias(TxHash,input.data(),32,input_count,40)){
        buff_sizet = static_cast<std::uint64_t>( input.capacity() ) - static_cast<std::uint64_t>( input.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 40){
            input.reserve(input.size() + 40);
        }

        input_count++;
        input.insert(input.end(), TxHash, TxHash + 32);
        addUint64BytestoVector(input, TxIx);

        bodymapcountbit |= 0x0001;
    }


    return *this;
}

TransaccionBody &TransaccionBody::addTransactionsOutput(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount){ // 1 : [* transaction_output] --> transaction_output = [ address , amount : value , ? datum_hash : $hash32 ; New]
    ///  1(address_keyhash_len) + 57(address_keyhash) + 8(amount) = 65 bytes maximo de largo cada output
    if(address_keyhash_len == 29 || address_keyhash_len == 57){

        if(!existen_coincidencias_output(address_keyhash, output.data(), address_keyhash_len,output_count , 9)){

            buff_sizet = static_cast<std::uint64_t>( output.capacity() ) - static_cast<std::uint64_t>( output.size() );
            addr_keyhash_buffer_len = address_keyhash_len + 9;

            // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
            if(buff_sizet < addr_keyhash_buffer_len ) {
                input.reserve(input.size() + addr_keyhash_buffer_len);
            }

            output_count++;
            output.push_back(static_cast<std::uint8_t>(address_keyhash_len)); //Agrega el address_keyhash_len en un bytes antes del address_keyhash
            output.insert(output.end(), address_keyhash, address_keyhash + address_keyhash_len); //Agrega el array address_keyhash
            addUint64BytestoVector(output, amount); //Agrega el amount en 8 bytes

            bodymapcountbit |= 0x0002;
        }

    }

    return *this;
}

TransaccionBody &TransaccionBody::addTransactionsOutput(std::string &payment_address, std::uint64_t const amount){ // 1 : [* transaction_output] --> transaction_output = [ address , amount : value , ? datum_hash : $hash32 ; New]
    ///  1(addr_keyhash_buffer_len) + 57(addr_keyhash_buffer) + 8(amount) = 65 bytes maximo de largo cada output

    if(bech32_decode(payment_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){

        addTransactionsOutput(addr_keyhash_buffer, addr_keyhash_buffer_len, amount);
        //buff_sizet = static_cast<std::uint64_t>( output.capacity() ) - static_cast<std::uint64_t>( output.size() );
        //std::size_t size_data = addr_keyhash_buffer_len + 9;

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        //if(buff_sizet < size_data ) {
        //    input.reserve(input.size() + size_data);
        //}

        //output_count++;
        //output.push_back(static_cast<std::uint8_t>(addr_keyhash_buffer_len)); //Agrega el address_keyhash_len en un bytes antes del address_keyhash
        //output.insert(output.end(), addr_keyhash_buffer, addr_keyhash_buffer + addr_keyhash_buffer_len); //Agrega el array address_keyhash
        //addUint64BytestoVector(output, amount); //Agrega el amount en 8 bytes

        //bodymapcountbit |= 0x0002;

    }



    return *this;
}

TransaccionBody &TransaccionBody::addCertificates(Certificates *cert){
    cert_ = cert;
    bodymapcountbit |= 0x0010;
    return *this;
}

TransaccionBody &TransaccionBody::addFee(std::uint64_t const amount){ // 2 : coin; fee   --> (uint)amount lovelance
    fee = amount;
    bodymapcountbit |= 0x0004;
    return *this;
}

TransaccionBody &TransaccionBody::addInvalidAfter(std::uint64_t const number){ // ? 3 : uint; time to live --> numberslot+200
    ttl = number;
    bodymapcountbit |= 0x0008;
    return *this;
}

TransaccionBody &TransaccionBody::addInvalidBefore(std::uint64_t const number){ // ? 8: uint; validity interval start
    vis = number;
    bodymapcountbit |= 0x0100;
    return *this;
}

bool TransaccionBody::existen_coincidencias(std::uint8_t const * data1, std::uint8_t const * data2, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
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

bool TransaccionBody::existen_coincidencias_output(std::uint8_t const * data, std::uint8_t const * output, std::uint16_t const data_len, std::uint16_t const ciclos ,std::uint16_t const salto ){
    buff_sizet = 0;
    addr_keyhash_buffer_len = 0;
    for(int e = 0 ;e < ciclos ; e++){

        output = output + addr_keyhash_buffer_len;
        buff_sizet = 0;
        for(int u = 0; u < data_len; u++){
            if(data[u] == output[u+1]){
                buff_sizet++;
            }
        }
        if(buff_sizet == data_len){
            return true;
        }
        addr_keyhash_buffer_len = output[0] + salto ;
    }

    return false;
}

TransaccionBody &TransaccionBody::addWithdrawals(std::uint8_t const *const stake_address_keyhash, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 29 (stake addr keyhash) + 8 (amount) = 37
    if(!existen_coincidencias(stake_address_keyhash,withdrawals.data(),29,withdrawals_count, 37)){ // Comprueba de que no se repitan las direcciones, si hay coincidencia se omite la direccion

        buff_sizet = static_cast<std::uint64_t>( withdrawals.capacity() ) - static_cast<std::uint64_t>( withdrawals.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 37){
            withdrawals.reserve(withdrawals.size() + 37);
        }

        withdrawals_count++;
        withdrawals.insert(withdrawals.end(), stake_address_keyhash, stake_address_keyhash + 29);
        addUint64BytestoVector(withdrawals, amount);

        bodymapcountbit |= 0x0020;

    }
    return *this;
}

TransaccionBody &TransaccionBody::addWithdrawals(std::string &stake_address, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 29 (stake addr keyhash) + 8 (amount) = 37

    if(bech32_decode(stake_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){
        if(addr_keyhash_buffer_len == 29){

            addWithdrawals(addr_keyhash_buffer, amount);

            //buff_sizet = static_cast<std::uint64_t>( withdrawals.capacity() ) - static_cast<std::uint64_t>( withdrawals.size() );

            // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
            //if(buff_sizet < 37){
            //    withdrawals.reserve(withdrawals.size() + 37);
            //}

            //withdrawals_count++;
            //withdrawals.insert(withdrawals.end(), addr_keyhash_buffer, addr_keyhash_buffer + 29);
            //addUint64BytestoVector(withdrawals, amount);

            //bodymapcountbit |= 0x0020;

        }

    }

    return *this;
}



TransaccionBody &TransaccionBody::addAuxiliaryDataHash(std::uint8_t const *const hash_32bytes){ // ? 7 : byte array ; auxiliary_data_hash --> blake2b256(auxiliary_data)
    // Reserva 32 bytes para contener el hash blake2b256
    auxiliary_data_hash.reserve(32);

    // Inserta el hash_32bytes en auxiliary_data_hash
    auxiliary_data_hash.assign(hash_32bytes, hash_32bytes + 32);

    bodymapcountbit |= 0x0080;
    return *this;
}

//-------
void TransaccionBody::Build(){
    //borra todos los datos del vector antes de serializar
    ClearCbor();
    std::uint64_t contador = 0;

    if(bodymapcountbit > 0){ //Condicion que salta el proceso en caso de que no hayan datos

    //Aca agregar al bodymapcountbit, la existencia de certificados (bodymapcountbit |= 0x0010;)
    for(std::uint8_t x = 0; x < 19; x++ ){ //se realiza un conteo de los mapas que existen en el transaccion body
        contador += (bodymapcountbit >> x ) & 0x01;
    }

        createMap(contador);                                     /// { }

        for(std::uint8_t x = 0; x < 19; x++ ){ //se asignan los datos

            if((bodymapcountbit >> x ) & 0x01){  //revisa cada item del transaction body

                switch(x){
                case 0 :{
                    contador = 0; // Se encarga de guardaar las posiciones del array
                    ptr_data = input.data();

                    addIndexMap(0);                               /// 0:
                    createArray(input_count);                     /// [  ]
                    for(std::uint8_t i = 0; i < input_count; i++){   /// [ TxHash , TxIx ],
                        contador = i*40;  ///32+8=40
                        createArray(2);
                        addBytesArray(&ptr_data[contador], 32);
                        addUint(&ptr_data[32 + contador]);
                    }
                };break;
                case 1 :{
                    contador = 0; // Se encarga de guardaar las posiciones del array
                    ptr_data = output.data();

                    addIndexMap(1);                                 /// 1:
                    createArray(output_count);                      /// [  ]
                    for(std::uint8_t i = 0; i < output_count; i++){ /// { 0: address_keyhash, 1: amount, ?2: datum_hash },

                        ///FALTA AGREGAR DATUMHASH
                        createMap(2);

                        addIndexMap(0);
                        addBytesArray(&ptr_data[contador+1], ptr_data[contador]);

                        contador += ptr_data[contador] + 9;

                        addIndexMap(1);
                        addUint(&ptr_data[contador-8]);



                    }

                };break;
                case 2 :{
                    addIndexMap(2);                                    /// 2:
                    addUint(fee);                                      /// fee
                };break;
                case 3 :{
                    addIndexMap(3);                                    /// 3:
                    addUint(ttl);                                      /// ttl
                };break;
                case 4 :{
                    addIndexMap(4);                                    /// 4:
                    addVectorUint8(cert_->Build());                    /// certificates
                };break;
                case 5 :{
                    ptr_data = withdrawals.data();
                    contador=0;
                    addIndexMap(5);                                 /// 5:
                    createMap(withdrawals_count);                   /// {  }
                    for(std::uint8_t i = 0; i < withdrawals_count; i++){ /// stake_address_keyhash : amount
                        contador= i * 37; //29 +8 = 37
                        addBytesArray(&ptr_data[contador], 29);
                        addUint(&ptr_data[29 + contador]);
                    }
                };break;
                case 6 :{};break;
                case 7 :{
                    addIndexMap(7);
                    addBytesArray(auxiliary_data_hash.data(),32);
                };break;
                case 8 :{
                    addIndexMap(8);                                    /// 3:
                    addUint(vis);                                      /// validity interval start
                };break;
                case 9 :{};break;
                case 10 :{};break;
                case 11 :{};break;
                case 12 :{};break;
                case 13 :{};break;
                case 14 :{};break;
                case 15 :{};break;
                case 16 :{};break;
                case 18 :{};break;
                case 19 :{};break;
                }
            }
        }
    }
}

//-------

//TransaccionBody TransaccionBody::AddCertificate(cert, cert_length){ // ? 4 : [* certificate] --> certificate =[ stake_registration // stake_deregistration // stake_delegation // pool_registration // genesis_key_delegation // move_instantaneous_rewards_cert]
//                                                                            //  stake_registration = (0, stake_credential)   ----> stake_credential =  [  0, addr_keyhash // 1, scripthash ]
//}                                                                //  stake_deregistration = (1, stake_credential)
//  stake_delegation = (2, stake_credential, pool_keyhash)
//  pool_registration = (3, pool_params)
