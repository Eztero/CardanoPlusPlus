#include "transaction_body.hpp"
//https://www.rfc-editor.org/rfc/rfc8610
#include <iomanip>
namespace Cardano{
TransactionBody::TransactionBody() : Multiassets(),
    V1language_views{
        0xa1,0x41,0x00,0x59,0x01,0xb6,0x9f,0x1a,0x00,0x03,0x23,0x61,0x19,0x03,0x2c,0x01,0x01,0x19,0x03,0xe8,0x19,0x02,0x3b,0x00,
        0x01,0x19,0x03,0xe8,0x19,0x5e,0x71,0x04,0x01,0x19,0x03,0xe8,0x18,0x20,0x1a,0x00,0x01,0xca,0x76,0x19,0x28,0xeb,0x04,0x19,
        0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,
        0x19,0x59,0xd8,0x18,0x64,0x18,0x64,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x4c,0x51,0x18,0x20,0x1a,0x00,0x02,0xac,0xfa,
        0x18,0x20,0x19,0xb5,0x51,0x04,0x1a,0x00,0x03,0x63,0x15,0x19,0x01,0xff,0x00,0x01,0x1a,0x00,0x01,0x5c,0x35,0x18,0x20,0x1a,
        0x00,0x07,0x97,0x75,0x19,0x36,0xf4,0x04,0x02,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,
        0x19,0x03,0xe8,0x19,0x6f,0xf6,0x04,0x02,0x1a,0x00,0x03,0xbd,0x08,0x1a,0x00,0x03,0x4e,0xc5,0x18,0x3e,0x01,0x1a,0x00,0x10,
        0x2e,0x0f,0x19,0x31,0x2a,0x01,0x1a,0x00,0x03,0x2e,0x80,0x19,0x01,0xa5,0x01,0x1a,0x00,0x02,0xda,0x78,0x19,0x03,0xe8,0x19,
        0xcf,0x06,0x01,0x1a,0x00,0x01,0x3a,0x34,0x18,0x20,0x19,0xa8,0xf1,0x18,0x20,0x19,0x03,0xe8,0x18,0x20,0x1a,0x00,0x01,0x3a,
        0xac,0x01,0x19,0xe1,0x43,0x04,0x19,0x03,0xe8,0x0a,0x1a,0x00,0x03,0x02,0x19,0x18,0x9c,0x01,0x1a,0x00,0x03,0x02,0x19,0x18,
        0x9c,0x01,0x1a,0x00,0x03,0x20,0x7c,0x19,0x01,0xd9,0x01,0x1a,0x00,0x03,0x30,0x00,0x19,0x01,0xff,0x01,0x19,0xcc,0xf3,0x18,
        0x20,0x19,0xfd,0x40,0x18,0x20,0x19,0xff,0xd5,0x18,0x20,0x19,0x58,0x1e,0x18,0x20,0x19,0x40,0xb3,0x18,0x20,0x1a,0x00,0x01,
        0x2a,0xdf,0x18,0x20,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x1a,0x00,0x01,0x0f,0x92,
        0x19,0x2d,0xa7,0x00,0x01,0x19,0xea,0xbb,0x18,0x20,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,
        0x01,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x1a,0x00,0x0c,0x50,0x4e,0x19,0x77,0x12,
        0x04,0x1a,0x00,0x1d,0x6a,0xf6,0x1a,0x00,0x01,0x42,0x5b,0x04,0x1a,0x00,0x04,0x0c,0x66,0x00,0x04,0x00,0x1a,0x00,0x01,0x4f,
        0xab,0x18,0x20,0x1a,0x00,0x03,0x23,0x61,0x19,0x03,0x2c,0x01,0x01,0x19,0xa0,0xde,0x18,0x20,0x1a,0x00,0x03,0x3d,0x76,0x18,
        0x20,0x19,0x79,0xf4,0x18,0x20,0x19,0x7f,0xb8,0x18,0x20,0x19,0xa9,0x5d,0x18,0x20,0x19,0x7d,0xf7,0x18,0x20,0x19,0x95,0xaa,
        0x18,0x20,0x1a,0x03,0x74,0xf6,0x93,0x19,0x4a,0x1f,0x0a,0xff
        }, V2language_views{
               0xa1,0x01,0x98,0xaf,0x1a,0x00,0x03,0x23,0x61,0x19,0x03,0x2c,0x01,0x01,0x19,0x03,0xe8,0x19,0x02,0x3b,0x00,0x01,0x19,0x03,
               0xe8,0x19,0x5e,0x71,0x04,0x01,0x19,0x03,0xe8,0x18,0x20,0x1a,0x00,0x01,0xca,0x76,0x19,0x28,0xeb,0x04,0x19,0x59,0xd8,0x18,
               0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x59,0xd8,
               0x18,0x64,0x18,0x64,0x18,0x64,0x19,0x59,0xd8,0x18,0x64,0x19,0x4c,0x51,0x18,0x20,0x1a,0x00,0x02,0xac,0xfa,0x18,0x20,0x19,
               0xb5,0x51,0x04,0x1a,0x00,0x03,0x63,0x15,0x19,0x01,0xff,0x00,0x01,0x1a,0x00,0x01,0x5c,0x35,0x18,0x20,0x1a,0x00,0x07,0x97,
               0x75,0x19,0x36,0xf4,0x04,0x02,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x19,0x03,0xe8,
               0x19,0x6f,0xf6,0x04,0x02,0x1a,0x00,0x03,0xbd,0x08,0x1a,0x00,0x03,0x4e,0xc5,0x18,0x3e,0x01,0x1a,0x00,0x10,0x2e,0x0f,0x19,
               0x31,0x2a,0x01,0x1a,0x00,0x03,0x2e,0x80,0x19,0x01,0xa5,0x01,0x1a,0x00,0x02,0xda,0x78,0x19,0x03,0xe8,0x19,0xcf,0x06,0x01,
               0x1a,0x00,0x01,0x3a,0x34,0x18,0x20,0x19,0xa8,0xf1,0x18,0x20,0x19,0x03,0xe8,0x18,0x20,0x1a,0x00,0x01,0x3a,0xac,0x01,0x19,
               0xe1,0x43,0x04,0x19,0x03,0xe8,0x0a,0x1a,0x00,0x03,0x02,0x19,0x18,0x9c,0x01,0x1a,0x00,0x03,0x02,0x19,0x18,0x9c,0x01,0x1a,
               0x00,0x03,0x20,0x7c,0x19,0x01,0xd9,0x01,0x1a,0x00,0x03,0x30,0x00,0x19,0x01,0xff,0x01,0x19,0xcc,0xf3,0x18,0x20,0x19,0xfd,
               0x40,0x18,0x20,0x19,0xff,0xd5,0x18,0x20,0x19,0x58,0x1e,0x18,0x20,0x19,0x40,0xb3,0x18,0x20,0x1a,0x00,0x01,0x2a,0xdf,0x18,
               0x20,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x1a,0x00,0x01,0x0f,0x92,0x19,0x2d,0xa7,
               0x00,0x01,0x19,0xea,0xbb,0x18,0x20,0x1a,0x00,0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x1a,0x00,
               0x02,0xff,0x94,0x1a,0x00,0x06,0xea,0x78,0x18,0xdc,0x00,0x01,0x01,0x1a,0x00,0x11,0xb2,0x2c,0x1a,0x00,0x05,0xfd,0xde,0x00,
               0x02,0x1a,0x00,0x0c,0x50,0x4e,0x19,0x77,0x12,0x04,0x1a,0x00,0x1d,0x6a,0xf6,0x1a,0x00,0x01,0x42,0x5b,0x04,0x1a,0x00,0x04,
               0x0c,0x66,0x00,0x04,0x00,0x1a,0x00,0x01,0x4f,0xab,0x18,0x20,0x1a,0x00,0x03,0x23,0x61,0x19,0x03,0x2c,0x01,0x01,0x19,0xa0,
               0xde,0x18,0x20,0x1a,0x00,0x03,0x3d,0x76,0x18,0x20,0x19,0x79,0xf4,0x18,0x20,0x19,0x7f,0xb8,0x18,0x20,0x19,0xa9,0x5d,0x18,
               0x20,0x19,0x7d,0xf7,0x18,0x20,0x19,0x95,0xaa,0x18,0x20,0x1a,0x02,0x23,0xac,0xcc,0x0a,0x1a,0x03,0x74,0xf6,0x93,0x19,0x4a,
               0x1f,0x0a,0x1a,0x02,0x51,0x5e,0x84,0x19,0x80,0xb3,0x0a
               } {
    ptrvec = nullptr;
    buff_sizet = 0;
    buff_uint32t = 0;
    addr_keyhash_buffer_len = 0;
    bodymapcountbit = 0;
    witnessmapcountbit = 0;
    //withdrawals_count = 0;
    totalcollateral = 0;
    fee = 0;
    ttl = 0;
    vis = 0;

}

TransactionBody::~TransactionBody(){
    ptrvec=nullptr;
}

TransactionBody &TransactionBody::addFee(std::uint64_t const amount){ // 2 : coin; fee   --> (uint)amount lovelance
    fee = amount;
    bodymapcountbit |= 0x0004;
    return *this;
}

TransactionBody &TransactionBody::addInvalidAfter(std::uint64_t const number){ // ? 3 : uint; time to live --> numberslot+200
    ttl = number;
    bodymapcountbit |= 0x0008;
    return *this;
}

TransactionBody &TransactionBody::addInvalidBefore(std::uint64_t const number){ // ? 8: uint; validity interval start
    vis = number;
    bodymapcountbit |= 0x0100;
    return *this;
}


/**
TransactionBody &TransactionBody::addWithdrawals(std::uint8_t const *const raw_stake_address, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 29 (stake addr keyhash) + 8 (amount) = 37
    if(withdrawals_count < UINT16_MAX && !existen_coincidencias(raw_stake_address, withdrawals.data(), 29, withdrawals_count, 37) ){ // Comprueba de que no se repitan las direcciones, si hay coincidencia se omite la direccion

        buff_sizet = static_cast<std::size_t>( withdrawals.capacity() ) - static_cast<std::size_t>( withdrawals.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 37){
            withdrawals.reserve(withdrawals.size() + 37);
        }

        withdrawals_count++;
        withdrawals.insert(withdrawals.end(), raw_stake_address, raw_stake_address + 29);
        addUint64toVector(withdrawals, amount);

        bodymapcountbit |= 0x0020;

    }
    return *this;
}

TransactionBody &TransactionBody::addWithdrawals(std::string &stake_address, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 29 (stake addr keyhash) + 8 (amount) = 37

    if(bech32_decode(stake_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){
        if(addr_keyhash_buffer_len == 29){

            addWithdrawals(addr_keyhash_buffer, amount);

        }

    }

    return *this;
}

**/

TransactionBody &TransactionBody::addTotalCollateral(std::uint64_t const amount){
    totalcollateral = amount;
    bodymapcountbit |= 0x20000;
    return *this;
}

TransactionBody &TransactionBody::addAuxiliaryDataHash(std::uint8_t const *const hash_32bytes){ // ? 7 : byte array ; auxiliary_data_hash --> blake2b256(auxiliary_data)
    // Reserva 32 bytes para contener el hash blake2b256
    auxiliary_data_hash.reserve(32);

    // Inserta el hash_32bytes en auxiliary_data_hash
    auxiliary_data_hash.assign(hash_32bytes, hash_32bytes + 32);

    bodymapcountbit |= 0x0080;
    return *this;
}

//-------
std::vector<std::uint8_t> const &TransactionBody::Build(){
    //CborSerialize cbor(&cborTransactionBody);
    cbor.clearCbor();
    std::uint64_t contador = 0;
    witnessmapcountbit = TransactionInput.getWitnessMapcountbit();
    witnessmapcountbit |= Certificate.getWitnessMapcountbit();
    witnessmapcountbit |= Withdrawal.getWitnessMapcountbit();
    bodymapcountbit |= TransactionOutput.getBodyMapcountbit();
    bodymapcountbit |= TransactionInput.getBodyMapcountbit();
    bodymapcountbit |= Certificate.getBodyMapcountbit();
    bodymapcountbit |= Withdrawal.getBodyMapcountbit();

    TransactionInput.alphanumeric_organization();
    Withdrawal.alphanumeric_organization();


    if(bodymapcountbit > 0){ //Condicion que salta el proceso en caso de que no hayan datos

        //Aca agregar al bodymapcountbit, la existencia de certificados (bodymapcountbit |= 0x0010;)
        for(std::uint8_t x = 0; x < 19; x++ ){ //se realiza un conteo de los mapas que existen en el transaccion body 0 - 18
            contador += (bodymapcountbit >> x ) & 0x01;
        }

        cbor.createMap(contador);                                      /// { }

        for(std::uint8_t x = 0; x < 19; x++ ){ //se asignan los datos

            if((bodymapcountbit >> x ) & 0x01){  //revisa cada item del transaction body

                switch(x){
                case 0 :{
                    ptrvec = TransactionInput.getInputs().data();
                    std::uint16_t const & input_count = TransactionInput.getInputsCount();

                    cbor.addIndexMap(static_cast<std::uint64_t>(0));   /// 0:
                    cbor.createArray(input_count);                     /// [  ]
                    for(std::uint16_t t = 0; t < input_count; t++){
                        cbor.createArray(2);                           /// [ , ],
                        cbor.addBytesArray(&ptrvec[2], 32);            /// [ TxHash , ],
                        cbor.addUint(&ptrvec[34]);                     /// [ TxHash , TxIx ],
                        ptrvec += 42;
                    }


                    std::uint16_t const & collateralinput_count = TransactionInput.getCollateralCount();
                    if(collateralinput_count > 0){
                        ptrvec = TransactionInput.getCollateral().data();

                        cbor.addIndexMap(13);                              /// 13:
                        cbor.createArray(collateralinput_count);           /// [  ]
                        for(std::uint16_t t = 0; t < collateralinput_count; t++){
                            cbor.createArray(2);                           /// [ , ],
                            cbor.addBytesArray(&ptrvec[2], 32);            /// [ TxHash , ],
                            cbor.addUint(&ptrvec[34]);                     /// [ TxHash , TxIx ],
                            ptrvec += 42;
                        }


                    }
                    std::uint16_t const & referenceinput_count = TransactionInput.getInputsReferencesCount();
                    if(referenceinput_count > 0){
                        ptrvec = TransactionInput.getInputsReferences().data();
                        cbor.addIndexMap(18);                              /// 18:
                        cbor.createArray(referenceinput_count);           /// [  ]
                        for(std::uint16_t t = 0; t < referenceinput_count; t++){
                            cbor.createArray(2);                           /// [ , ],
                            cbor.addBytesArray(&ptrvec[2], 32);            /// [ TxHash , ],
                            cbor.addUint(&ptrvec[34]);                     /// [ TxHash , TxIx ],
                            ptrvec += 42;
                        }

                    }
                };break;
                case 1 :{
                    buff_uint32t = 0; // Se encargara de guardar el tamaño del array , max
                    ptrvec = TransactionOutput.getTransactionsOutputs().data();
                    std::uint16_t const &output_count = TransactionOutput.getAmountTransactionsOutputs(); // se obtiene la cantidad de direcciones de salida
                    std::uint8_t const *pos_map_transaccion_output[6]{};  // la ultima esta reservada para collateral return y no se borra
                    std::uint8_t count_map_transaccion = 0;

                    cbor.addIndexMap(1);
                    // en caso de existir un returncolateral se resta 1 ya que no pertenece a este array,
                    // return colateral se agregara posteriormente                                         /// 1:
                    cbor.createArray(output_count - ((TransactionOutput.getBodyMapcountbit() & 0x10000) >> 16));   /// [{q},{w},{n},..]

                    for(std::uint8_t i = 0; i < output_count; i++){ // { 0: address_keyhash, 1: amount, ? 2: datum_option, ? 3: script_ref},
                        // Se borran los punteros previos de 0 - 4
                        for(int c = 0; c < 5; c++){
                            pos_map_transaccion_output[c] = nullptr;
                        }
                        // Se extraen la cantidad de elementos del mapa,  ¿usar switch?
                        if(*ptrvec == 0x00){  // en caso de detectar este separador
                            count_map_transaccion = *(ptrvec+1);
                            // {a,b,c,d}  "el addOutput vale por 2 (0: address y 1:amount) y por 1 en 1:[amount, assets]
                            cbor.createMap(static_cast<std::uint64_t>(count_map_transaccion + *(ptrvec+2)));

                        }else if(*ptrvec == 0x05){ //en caso de detectar el separador de collateral return
                            count_map_transaccion = 1;
                        }
                        //Se ordenan los datos en un vector de puntero (pos_map_transaccion_output)
                        /// Se podria facilmente escribir los datos aqui, pero para una mejor lectura en cbor se respetaran las posiciones antes de escribir los datos.
                        /// Funcionalmente el orden no deberia afectar.
                        for(std::uint8_t c = 0; c < count_map_transaccion; c++){
                            switch (*ptrvec){
                            case 0x00:{    //"addOutput"
                                ptrvec += 3;
                                pos_map_transaccion_output[0] = ptrvec;
                                ptrvec += *ptrvec + 8 + 1;
                            };break;
                            case 0x01:{   //"addAsset
                                ptrvec += 1;
                                pos_map_transaccion_output[1] = ptrvec;
                                ptrvec += Utils::extract8bytestoUint64(ptrvec) + 8; //el numero 8, es un buffer de 8 bytes que almacena el largo en bytes del la variable
                            };break;
                            case 0x02:{   //addDatumHash
                                ptrvec += 1;
                                pos_map_transaccion_output[2] = ptrvec;
                                ptrvec += Utils::extract8bytestoUint64(ptrvec) + 8;
                            };break;
                            case 0x03:{   //addReferenceDatum
                                ptrvec += 1;
                                pos_map_transaccion_output[3] = ptrvec;
                                ptrvec += Utils::extract8bytestoUint64(ptrvec) + 8;
                            };break;
                            case 0x04:{  //addReferenceScript
                                ptrvec += 1;
                                pos_map_transaccion_output[4] = ptrvec;
                                ptrvec += Utils::extract8bytestoUint64(ptrvec) + 8;
                            };break;
                            case 0x05:{  //addReturnColateral
                                ptrvec += 1;
                                pos_map_transaccion_output[5] = ptrvec;
                                ptrvec += *ptrvec + 8 + 1;
                            };break;
                            }
                        }

                        // Se escriben los datos de manera ordenada, se excluye a addReturnColateral de este paso
                        for(int c = 0 ;c < 5; c++){
                            if(pos_map_transaccion_output[c] != nullptr){
                                switch(c){
                                case 0:{                                                                        /// 0: address_keyhash, 1: amount,
                                    cbor.addIndexMap(static_cast<std::uint64_t>(0));
                                    buff_uint32t = static_cast<std::uint16_t>(*pos_map_transaccion_output[c]);
                                    cbor.addBytesArray(pos_map_transaccion_output[c]+1, buff_uint32t);  //pos_map_transaccion_output[c][n]
                                    cbor.addIndexMap(1);
                                    if(pos_map_transaccion_output[1] != nullptr){
                                        cbor.createArray(2);
                                    }
                                    cbor.addUint(pos_map_transaccion_output[c]+1+buff_uint32t);

                                };break;
                                case 1:{                                                                        ///  1: [amount, ? assets] ,
                                    buff_uint32t = Utils::extract8bytestoUint64(pos_map_transaccion_output[c]);
                                    cbor.bypassPtrUint8Cbor(pos_map_transaccion_output[c]+8, buff_uint32t);
                                };break;
                                case 2:{                                                                        /// ? 2: [0,hash32] ,
                                    cbor.addIndexMap(2).createArray(2).addUint(static_cast<std::uint64_t>(0));

                                    buff_uint32t = Utils::extract8bytestoUint64(pos_map_transaccion_output[c]);
                                    cbor.bypassPtrUint8Cbor(pos_map_transaccion_output[c]+8, buff_uint32t);

                                };break;
                                case 3:{                                                                        /// ? 2: [1,data] ,
                                    cbor.addIndexMap(2).createArray(2).addUint(1);
                                    buff_uint32t = Utils::extract8bytestoUint64(pos_map_transaccion_output[c]);
                                    cbor.bypassPtrUint8Cbor(pos_map_transaccion_output[c]+8, buff_uint32t);
                                };break;
                                case 4:{                                                                        /// ? 3: script_ref,
                                    cbor.addIndexMap(3);
                                    buff_uint32t = Utils::extract8bytestoUint64(pos_map_transaccion_output[c]);
                                    cbor.bypassPtrUint8Cbor(pos_map_transaccion_output[c]+8, buff_uint32t);};break;
                                }
                            }
                        }
                    }

                    /// ? 16 : transaction_output ;collateral return
                    if(pos_map_transaccion_output[5] != nullptr){
                        cbor.addIndexMap(16).createMap(2).addIndexMap(static_cast<std::uint64_t>(0));;
                        buff_uint32t = static_cast<std::uint16_t>(*pos_map_transaccion_output[5]);
                        cbor.addBytesArray(pos_map_transaccion_output[5]+1, buff_uint32t);  //pos_map_transaccion_output[c][n]
                        cbor.addIndexMap(1);
                        cbor.addUint(pos_map_transaccion_output[5]+1+buff_uint32t);
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
                    cbor.addIndexMap(4);                                       /// 4:
                    cbor.createArray(static_cast<std::uint64_t>(Certificate.getCborCertificatesCount()));
                    cbor.bypassVectorCbor(Certificate.getCborCertificates());  /// certificates
                };break;
                case 5 :{
                    std::uint16_t const & withdrawals_count = Withdrawal.getWithdrawalsCount();
                    ptrvec = Withdrawal.getWithdrawals().data();
                    cbor.addIndexMap(5);                                 /// 5:
                    cbor.createMap(withdrawals_count);                   /// {  }
                    for(std::uint8_t i = 0; i < withdrawals_count; i++){
                        cbor.addIndexMap(&ptrvec[2], 29);                /// stake_address_keyhash :
                        cbor.addUint(&ptrvec[31]);                       /// stake_address_keyhash : amount
                        ptrvec += 39;
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
                case 11 :{   // redeemers | datums | laguage views

                    std::unique_ptr<Utils::CborSerialize> script_data(new Utils::CborSerialize);
                    std::uint16_t const & datum_data_count = TransactionInput.getSpendingDatumsCount();
                    std::uint16_t const & spendredeemer_data_count = TransactionInput.getSpendingRedeemersCount();
                    std::uint16_t const & certredeemer_data_count = Certificate.getCertificateRedeemersCount();
                    std::uint16_t const & rewardredeemer_data_count = Withdrawal.getWithdrawalRedeemersCount();

                    switch((witnessmapcountbit & 0x30)){
                    case 0x10:{

                        ptrvec = TransactionInput.getSpendingDatums().data();
                        script_data->createArray(0);                                                       //  80
                        script_data->createArray(static_cast<std::uint64_t>(datum_data_count));            //  [datums]
                        for(std::uint16_t t = 0; t < datum_data_count;t++){
                            script_data->bypassPtrUint8Cbor( ptrvec+10, Utils::extract8bytestoUint64(ptrvec+2) );
                            ptrvec += Utils::extract8bytestoUint64(ptrvec+2) + 10;
                        }
                        script_data->createMap(0);                                                          //  A0

                        std::vector<std::uint8_t> const &buff_getCbor = script_data->getCbor();  // se agrega la serializacion a la variable cbor_datums (se usa en witnnes)
                        cbor_datums.assign(buff_getCbor.begin()+1, buff_getCbor.end()-1 );

                    };break;
                    case 0x30:
                    case 0x20:{
                        std::uint16_t const redeemer_data_count = spendredeemer_data_count + certredeemer_data_count + rewardredeemer_data_count;

                        script_data->createArray(static_cast<std::uint64_t>(redeemer_data_count));                // [ ]

                        if(spendredeemer_data_count){
                            ptrvec = TransactionInput.getSpendingRedeemers().data();
                            for(std::uint16_t t = 0; t < spendredeemer_data_count;t++){
                                script_data->createArray(4);                                                           // [ , , , ]
                                script_data->addUint( *( ptrvec + 2 ) );                                               // tag
                                script_data->addUint(Utils::extract2bytestoUint16( ptrvec ));                                 // index
                                script_data->bypassPtrUint8Cbor( ptrvec + 11, Utils::extract8bytestoUint64( ptrvec + 3 ) );   // plutus_data
                                ptrvec += Utils::extract8bytestoUint64( ptrvec + 3 ) + 11;                                    // cambio al posicion de ptrvec
                                script_data->bypassPtrUint8Cbor(ptrvec + 8, Utils::extract8bytestoUint64( ptrvec ));          // ex_units
                                ptrvec += Utils::extract8bytestoUint64( ptrvec ) + 8;
                            }

                        }

                        if(certredeemer_data_count){
                            ptrvec = Certificate.getCertificateRedeemers().data();
                            for(std::uint16_t t = 0; t < certredeemer_data_count;t++){
                                script_data->createArray(4);                                                           // [ , , , ]
                                script_data->addUint( *( ptrvec + 2 ) );                                               // tag
                                script_data->addUint(Utils::extract2bytestoUint16( ptrvec ));                                 // index
                                script_data->bypassPtrUint8Cbor( ptrvec + 11, Utils::extract8bytestoUint64( ptrvec + 3 ) );   // plutus_data
                                ptrvec += Utils::extract8bytestoUint64( ptrvec + 3 ) + 11;                                    // cambio al posicion de ptrvec
                                script_data->bypassPtrUint8Cbor(ptrvec + 8, Utils::extract8bytestoUint64( ptrvec ));          // ex_units
                                ptrvec += Utils::extract8bytestoUint64( ptrvec ) + 8;
                            }

                        }

                        if(rewardredeemer_data_count){
                            ptrvec = Withdrawal.getWithdrawalRedeemers().data();
                            for(std::uint16_t t = 0; t < certredeemer_data_count;t++){
                                script_data->createArray(4);                                                           // [ , , , ]
                                script_data->addUint( *( ptrvec + 2 ) );                                               // tag
                                script_data->addUint(Utils::extract2bytestoUint16( ptrvec ));                                 // index
                                script_data->bypassPtrUint8Cbor( ptrvec + 11, Utils::extract8bytestoUint64( ptrvec + 3 ) );   // plutus_data
                                ptrvec += Utils::extract8bytestoUint64( ptrvec + 3 ) + 11;                                    // cambio al posicion de ptrvec
                                script_data->bypassPtrUint8Cbor(ptrvec + 8, Utils::extract8bytestoUint64( ptrvec ));          // ex_units
                                ptrvec += Utils::extract8bytestoUint64( ptrvec ) + 8;
                            }

                        }


                        std::vector<std::uint8_t> const &buff_getCbor = script_data->getCbor();  // se agrega la serializacion a la variable cbor_redeemers (se usa en witnnes)
                        cbor_redeemers.assign(buff_getCbor.begin(), buff_getCbor.end());

                        if(datum_data_count){
                            ptrvec = TransactionInput.getSpendingDatums().data();
                            script_data->createArray(static_cast<std::uint64_t>(datum_data_count));                 //  [datums]
                            for(std::uint16_t t = 0; t < datum_data_count;t++){
                                script_data->bypassPtrUint8Cbor( ptrvec+10, Utils::extract8bytestoUint64(ptrvec+2) );
                                ptrvec += Utils::extract8bytestoUint64(ptrvec+2) + 10;

                            }
                        }

                        cbor_datums.assign(buff_getCbor.begin()+cbor_redeemers.size(), buff_getCbor.end() );   // se agrega la serializacion a la variable cbor_datums (se usa en witnnes)

                        switch(TransactionInput.getGlobalReferencesScriptsType()){                                //  language views
                        case ScriptType::Plutus_Script_V1: {script_data->bypassPtrUint8Cbor(V1language_views,444);};break;
                        case ScriptType::Plutus_Script_V2: {script_data->bypassPtrUint8Cbor(V2language_views,467);};break;
                        default:{throw std::invalid_argument("error in ScriptType: type not valid , for input references, set script with setGlobalReferencesStriptsType()");};break;
                            /// poner un error en el default si no se selecciono un tipo
                        }


                    };break;
                    }

                    std::vector<std::uint8_t> const  & cbor_script_data = script_data->getCbor();
                    std::uint8_t script_data_hash[32];

                    /**
                    std::cout <<"\n cbor_script_data: ";
                    for (std::uint8_t i : cbor_script_data){
                        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(i);
                    }
                    std::cout <<std::endl;
                    **/



                    crypto_generichash_blake2b(script_data_hash, 32, cbor_script_data.data(), cbor_script_data.size(), nullptr, 0); //blake2b256(cbor_script_data)
                    cbor.addIndexMap(11);                         /// 11:
                    cbor.addBytesArray(script_data_hash,32);      /// script_data_hash

                };break;
                case 12 :{};break;
                case 13 :{
                    // 13: transacction_input ; collateral input
                    // se agregara en case 0

                };break;
                case 14 :{};break;
                case 15 :{};break;
                case 16 :{
                    // 16: transacction_output ; collateral return
                    // se agregara en case 1
                };break;
                case 17 :{
                    cbor.addIndexMap(17);                                 /// 17:
                    cbor.addUint(totalcollateral);                        /// total collateral
                };break;
                case 18 :{
                    // 18 : set<transaction_input> ; reference inputs;
                    // Se agregara en case 0
                };break;
                }
            }
        }
    }
    else{
        cbor.createArray(0);
    }
    return cbor.getCbor();
}


std::vector<std::uint8_t> const & TransactionBody::getcborDatums_afterBuild() const{
    return cbor_datums;
}

std::vector<std::uint8_t> const & TransactionBody::getcborRedeemers_afterBuild() const{
    return cbor_redeemers;
}

std::uint16_t const & TransactionBody::getWitnessMapcountbit() const{
    return witnessmapcountbit;
}

}
