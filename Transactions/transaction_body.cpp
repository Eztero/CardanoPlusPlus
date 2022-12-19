#include "transaction_body.hpp"
//https://www.rfc-editor.org/rfc/rfc8610


TransaccionBody::TransaccionBody() : Certificates() {
    //cert_ = nullptr;
    ptrvec = nullptr;
    buff_sizet = 0;
    addr_keyhash_buffer_len = 0;
    bodymapcountbit = 0;
    output_count = 0;
    input_count = 0;
    withdrawals_count = 0;
    fee = 0;
    ttl = 0;
    vis = 0;

}

TransaccionBody::~TransaccionBody(){
    ptrvec=nullptr;
}

TransaccionBody &TransaccionBody::addTransactionsInput(std::uint8_t const *const TxHash,std::uint64_t const TxIx){   //0 : set<transaction_input> --> transaction_input = [ transaction_id : hash32, index : uint]
    /// 32(TxHash) + 8(TxIx) = 40 bytes de largo cada input

    if( ( input_count < UINT16_MAX ) && ( !existen_coincidencias(TxHash, input.data(), 32, input_count, 40) ) ){
        buff_sizet = static_cast<std::size_t>( input.capacity() ) - static_cast<std::size_t>( input.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 40){
            input.reserve(input.size() + 40);
        }

        input_count++;
        input.insert(input.end(), TxHash, TxHash + 32);
        agregarUint64BytestoVector(input, TxIx);

        bodymapcountbit |= 0x0001;
    }


    return *this;
}

TransaccionBody &TransaccionBody::addTransactionsInput(std::string &TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const *const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
    addTransactionsInput(TxHash_uint8t, TxIx);
    }
    delete[] TxHash_uint8t;

return *this;
}

TransaccionBody &TransaccionBody::addTransactionsOutput(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount){ // 1 : [* transaction_output] --> transaction_output = [ address , amount : value , ? datum_hash : $hash32 ; New]
    ///  1(address_keyhash_len) + 57(address_keyhash) + 8(amount) = 65 bytes maximo de largo cada output
    if(address_keyhash_len == 29 || address_keyhash_len == 57){

        if(output_count < UINT16_MAX && !existen_coincidencias_output(address_keyhash, output.data(), address_keyhash_len,output_count , 9) ){

            buff_sizet = static_cast<std::size_t>( output.capacity() ) - static_cast<std::size_t>( output.size() );
            addr_keyhash_buffer_len = address_keyhash_len + 9;

            // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
            if(buff_sizet < addr_keyhash_buffer_len ) {
                input.reserve(input.size() + addr_keyhash_buffer_len);
            }

            output_count++;
            output.push_back(static_cast<std::uint8_t>(address_keyhash_len)); //Agrega el address_keyhash_len en un bytes antes del address_keyhash
            output.insert(output.end(), address_keyhash, address_keyhash + address_keyhash_len); //Agrega el array address_keyhash
            agregarUint64BytestoVector(output, amount); //Agrega el amount en 8 bytes

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

TransaccionBody &TransaccionBody::addWithdrawals(std::uint8_t const *const stake_address_keyhash, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 29 (stake addr keyhash) + 8 (amount) = 37
    if(withdrawals_count < UINT16_MAX && !existen_coincidencias(stake_address_keyhash,withdrawals.data(),29,withdrawals_count, 37) ){ // Comprueba de que no se repitan las direcciones, si hay coincidencia se omite la direccion

        buff_sizet = static_cast<std::size_t>( withdrawals.capacity() ) - static_cast<std::size_t>( withdrawals.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 37){
            withdrawals.reserve(withdrawals.size() + 37);
        }

        withdrawals_count++;
        withdrawals.insert(withdrawals.end(), stake_address_keyhash, stake_address_keyhash + 29);
        agregarUint64BytestoVector(withdrawals, amount);

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
std::vector<std::uint8_t> const &TransaccionBody::Build(){
    CborSerialize cbor(&cborTransactionBody);
    cbor.ClearCbor();
    std::uint64_t contador = 0;

    if(arethereCertificates()){
        bodymapcountbit |= 0x0010;
    }

    if(bodymapcountbit > 0){ //Condicion que salta el proceso en caso de que no hayan datos

    //Aca agregar al bodymapcountbit, la existencia de certificados (bodymapcountbit |= 0x0010;)
    for(std::uint8_t x = 0; x < 19; x++ ){ //se realiza un conteo de los mapas que existen en el transaccion body
        contador += (bodymapcountbit >> x ) & 0x01;
    }

        cbor.createMap(contador);                                      /// { }

        for(std::uint8_t x = 0; x < 19; x++ ){ //se asignan los datos

            if((bodymapcountbit >> x ) & 0x01){  //revisa cada item del transaction body

                switch(x){
                case 0 :{
                    ptrvec = input.data();

                    cbor.addIndexMap(static_cast<std::uint64_t>(0));   /// 0:
                    cbor.createArray(input_count);                     /// [  ]
                    for(std::uint8_t i = 0; i < input_count; i++){
                        cbor.createArray(2);                           /// [ , ],
                        cbor.addBytesArray(&ptrvec[0], 32);            /// [ TxHash , ],
                        cbor.addUint(&ptrvec[32]);                     /// [ TxHash , TxIx ],
                        ptrvec += 40;
                    }
                };break;
                case 1 :{
                    addr_keyhash_buffer_len = 0; // Se encarga de guardaar las posiciones del array
                    ptrvec = output.data();

                    cbor.addIndexMap(1);                                                /// 1:
                    cbor.createArray(output_count);                                     /// [  ]
                    for(std::uint8_t i = 0; i < output_count; i++){ // { 0: address_keyhash, 1: amount, ?2: datum_hash },

                        ///FALTA AGREGAR DATUMHASH
                        addr_keyhash_buffer_len = static_cast<std::uint16_t>(ptrvec[0]);
                        ptrvec += 1;

                        cbor.createMap(2);                                              /// { , }

                        cbor.addIndexMap(static_cast<std::uint64_t>(0));                /// { 0: , }
                        cbor.addBytesArray(&ptrvec[0], addr_keyhash_buffer_len);        /// { 0: address_keyhash, }

                        cbor.addIndexMap(1);                                            /// { 0: address_keyhash, 1: }
                        cbor.addUint(&ptrvec[addr_keyhash_buffer_len]);                 /// { 0: address_keyhash, 1: amount }

                        ptrvec += addr_keyhash_buffer_len + 8;  //  addr_keyhash_buffer_len + 9 - 1 = addr_keyhash_buffer_len + 8 Se resta uno menos en puntero



                    }

                };break;
                case 2 :{
                    cbor.addIndexMap(2);                                 /// 2:
                    cbor.addUint(fee);                                   /// fee
                };break;
                case 3 :{
                    cbor.addIndexMap(3);                                 /// 3:
                    cbor.addUint(ttl);                                   /// ttl
                };break;
                case 4 :{
                    cbor.addIndexMap(4);                                 /// 4:
                    cbor.bypassVectorCbor(getCborCertificates());                 /// certificates
                };break;
                case 5 :{
                    ptrvec = withdrawals.data();
                    cbor.addIndexMap(5);                                 /// 5:
                    cbor.createMap(withdrawals_count);                   /// {  }
                    for(std::uint8_t i = 0; i < withdrawals_count; i++){
                        cbor.addIndexMap(&ptrvec[0], 29);                /// stake_address_keyhash :
                        cbor.addUint(&ptrvec[29]);                       /// stake_address_keyhash : amount
                        ptrvec += 37;
                    }
                };break;
                case 6 :{};break;
                case 7 :{
                    cbor.addIndexMap(7);                                 /// 7:
                    cbor.addBytesArray(auxiliary_data_hash.data(),32);   /// auxiliary_data_hash
                };break;
                case 8 :{
                    cbor.addIndexMap(8);                                 /// 8:
                    cbor.addUint(vis);                                   /// validity interval start
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
    else{
        cbor.createArray(0);
    }
    return cborTransactionBody;
}

//-------

//TransaccionBody TransaccionBody::AddCertificate(cert, cert_length){ // ? 4 : [* certificate] --> certificate =[ stake_registration // stake_deregistration // stake_delegation // pool_registration // genesis_key_delegation // move_instantaneous_rewards_cert]
//                                                                            //  stake_registration = (0, stake_credential)   ----> stake_credential =  [  0, addr_keyhash // 1, scripthash ]
//}                                                                //  stake_deregistration = (1, stake_credential)
//  stake_delegation = (2, stake_credential, pool_keyhash)
//  pool_registration = (3, pool_params)
