#include "transactioninput.hpp"

/// Vector<std::uint8_t> tx_input / reference_input / collateral_input:
/// +2  (index) // indica el orden
/// +32 (TxHash)
/// +8  (TxIx)
/// 42 bytes maximo de largo minimo

/// Vector<std::uint8_t> datum_input (datum) :
/// +2 (index_datum)     // index_datum = index (tx_input), indica la posicion del uxto en tx_input despues ser ordenado
/// +8 (largo_plutus_data)
/// +n (plutus_data)
/// 10+n bytes maximo de largo minimo

/// Vector<std::uint8_t> redeemer_input (redeeemer) :
/// +2 (index_redemer)  // index_redemer = index (tx_input), indica la posicion del uxto en tx_input despues ser ordenado
/// +1 (tag)
/// +8 (largo_plutus_data)
/// +n (plutus_data)
/// +8 (largo_ex_units)
/// +n (ex_units) (previamente serializado en cbor)
/// 19+n bytes maximo de largo minimo

/// Vector<std::uint8_t> scripts
/// +2 (script count // indica la cantidad de script que tiene este vector
/// +n (scripts)




TransactionsInputs::TransactionsInputs(){
    bodymap_countbit = 0;
    witnessmap_countbit = 0;
    tx_input_count = 0;
    reference_input_count = 0;
    collateral_input_count = 0;
    datum_input_count = 0;
    buff_sizet = 0 ;
    redeemer_input_count = 0;
    plutusscript1_input_count = 0;
    plutusscript2_input_count = 0;
    nativescript_input_count = 0;
    globalreferencescript = ScriptType::None;

}

bool TransactionsInputs::addUtxoInput(std::uint8_t const t_selector, std::uint8_t const * const & TxHash,std::uint64_t const & TxIx){   //0 : set<transaction_input> --> transaction_input = [ transaction_id : hash32, index : uint]
    ///2(index) + 32(TxHash) + 8(TxIx) = 42 bytes de largo cada input

    std::vector<std::uint8_t> *data_input = nullptr;
    std::uint16_t *data_input_count = nullptr;

    switch(t_selector){
    case 0x00:{
        data_input = &tx_input;
        data_input_count = &tx_input_count;
        bodymap_countbit |= 0x0001;
    };break;
    case 0x01:{
        data_input = &reference_input;
        data_input_count = &reference_input_count;
        bodymap_countbit |= 0x40000;
    };break;
    case 0x02:{
        data_input = &collateral_input;
        data_input_count = &collateral_input_count;
        bodymap_countbit |= 0x2000;
    };break;
    }

    if( ( *data_input_count < UINT16_MAX ) && ( !existen_coincidencias(TxHash, data_input->data(), 32, *data_input_count, 40) ) ){
        buff_sizet = static_cast<std::size_t>( data_input->capacity() ) - static_cast<std::size_t>( data_input->size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 42){
            data_input->reserve(data_input->size() + 42);
        }

        addUint16toVector(data_input,data_input_count);  // Index
        data_input->insert(data_input->end(), TxHash, TxHash + 32);
        addUint64toVector(*data_input, TxIx);

        *data_input_count += 1;

        return true;
    }

    return false;
}

// 0 : set<transaction_input>    ; inputs
TransactionsInputs &TransactionsInputs::addInput(std::string const &TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const * const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
        addUtxoInput(0, TxHash_uint8t ,TxIx);
    }
    delete[] TxHash_uint8t;

    return *this;
}

// ? 18 : set<transaction_input> ; reference inputs;
TransactionsInputs &TransactionsInputs::addSpendingReference(std::string const & TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const * const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
        addUtxoInput(1, TxHash_uint8t ,TxIx);
    }
    delete[] TxHash_uint8t;

    return *this;
}

TransactionsInputs &TransactionsInputs::addWithdrawalReference(std::string const & TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const * const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
        addUtxoInput(1, TxHash_uint8t ,TxIx);
    }
    delete[] TxHash_uint8t;

    return *this;
}


TransactionsInputs &TransactionsInputs::addCertificateReference(std::string const & TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const * const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
        addUtxoInput(1, TxHash_uint8t ,TxIx);
    }
    delete[] TxHash_uint8t;

    return *this;
}

// ? 13 : set<transaction_input> ; collateral inputs
TransactionsInputs &TransactionsInputs::addCollateral(std::string const & TxHash, std::uint64_t const TxIx){
    std::size_t txhash_len;
    std::uint8_t const * const TxHash_uint8t = hexchararray2uint8array(TxHash, &txhash_len);
    if(txhash_len == 32){
        addUtxoInput(2, TxHash_uint8t ,TxIx);
    }
    delete[] TxHash_uint8t;

    return *this;
}

// plutus_data
TransactionsInputs &TransactionsInputs::addSpendingDatum(std::string & json_datum){
    std::unique_ptr<PlutusJsonSchema> Json_p(new PlutusJsonSchema());
    Json_p->addSchemaJson(json_datum);
    std::vector<std::uint8_t> const & cbor_datum = Json_p->getCborSchemaJson();

    addUint16toVector( datum_input, (tx_input_count - 1 ) );                                /// Index_datum , LANZAR ERROR SI tx_input_count=0
    addUint64toVector(datum_input,cbor_datum.size());                                       // cbor_datum_len

    datum_input.insert(datum_input.end(),cbor_datum.begin(),cbor_datum.end());
    ++datum_input_count;

    bodymap_countbit |= 0x800;
    witnessmap_countbit |= 0x10;

    return *this;
}


// redeemer = [ tag: redeemer_tag, index: uint, data: plutus_data, ex_units: ex_units ]
TransactionsInputs &TransactionsInputs::addSpendingRedeemer(std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits ){


    std::unique_ptr<CborSerialize> rcbor(new CborSerialize);
    std::unique_ptr<CborSerialize> unitscbor(new CborSerialize);
    std::unique_ptr<PlutusJsonSchema> Json_p(new PlutusJsonSchema);

    unitscbor->createArray(2);
    unitscbor->addUint(memoryunits); // mem
    unitscbor->addUint(cpusteps);    // step

    Json_p->addSchemaJson(json_redeemer);

    std::vector<std::uint8_t> const & cbor_units = unitscbor->getCbor();
    std::vector<std::uint8_t> const & cbor_plutusdata = Json_p->getCborSchemaJson();

    addUint16toVector( redeemer_input, (tx_input_count - 1 ) );                                /// Index_redeemer , LANZAR ERROR SI tx_input_count=0
    redeemer_input.push_back(static_cast<std::uint8_t>(0));                                    // tag = 0
    addUint64toVector(redeemer_input,cbor_plutusdata.size());                                  // plutusdata_len
    redeemer_input.insert(redeemer_input.end(),cbor_plutusdata.begin(),cbor_plutusdata.end()); // plutusdata
    addUint64toVector(redeemer_input,cbor_units.size());                                       // cbor_ex_units_len
    redeemer_input.insert(redeemer_input.end(),cbor_units.begin(),cbor_units.end());           // cbor_ex_units

    ++redeemer_input_count;
    bodymap_countbit |= 0x800;
    witnessmap_countbit |= 0x20;

    return *this;
}


TransactionsInputs &TransactionsInputs::addScript(TransactionsInputs::ScriptType const script_type, std::uint8_t const * const & script, std::size_t & script_len){
    switch(script_type){
    case ScriptType::Native_Script:{
        if(nativescript_input_count == 0){
            nativescript_input.assign( 2 , 0 );  // crea un espacio con ceros para despues indicar la cantidad de script en el array
        }
        nativescript_input.insert(nativescript_input.end(),script,script+script_len);
        nativescript_input_count++;
        witnessmap_countbit |= 0x02;
    };break;
    case ScriptType::Plutus_Script_V1:{
        if(plutusscript1_input_count == 0){
            plutusscript1_input.assign( 2 , 0 );  // crea un espacio con ceros para despues indicar la cantidad de script en el array
        }
        plutusscript1_input.insert(plutusscript1_input.end(),script,script+script_len);
        plutusscript1_input_count ++;
        witnessmap_countbit |= 0x08;
    };break;
    case ScriptType::Plutus_Script_V2:{
        if(plutusscript2_input_count == 0){
            plutusscript2_input.assign( 2 , 0 );  // crea un espacio con ceros para despues indicar la cantidad de script en el array
        }
        plutusscript2_input.insert(plutusscript2_input.end(),script,script+script_len);
        plutusscript2_input_count++;
        witnessmap_countbit |= 0x40;
    };break;

    }
    setGlobalReferencesStriptsType(script_type);
    return *this;
}


TransactionsInputs & TransactionsInputs::addScript( TransactionsInputs::ScriptType const script_type, std::string const & script ){
    std::size_t script_tipo_len = 0;
    std::uint8_t const *script_tipo = hexchararray2uint8array(script, &script_tipo_len);
    addScript(script_type, script_tipo, script_tipo_len);
    delete[] script_tipo;
    return *this;
}


TransactionsInputs & TransactionsInputs::setGlobalReferencesStriptsType( TransactionsInputs::ScriptType const script_type){
    globalreferencescript = script_type;
    return *this;
}


void TransactionsInputs::alphanumeric_organization(){

    std::unique_ptr<std::vector<std::uint8_t>> vector_data(new std::vector<std::uint8_t>{});
    std::uint8_t *ptr_data = nullptr;
    std::uint8_t *ptr_input[tx_input_count];
    std::uint8_t *ptr_reference_input[reference_input_count];
    std::uint8_t *ptr_collateral_input[collateral_input_count];
    std::uint8_t *ptr_datums[datum_input_count];
    std::uint8_t *ptr_redeemers[redeemer_input_count];
    std::uint16_t countmenosuno;
    int i_cmp = 0;

    if(tx_input_count > 0){
        ptr_data = tx_input.data();
        for(std::uint16_t  i= 0; i<tx_input_count;i++ ){ // asigno las localizaciones a punteros
            ptr_input[i] = ptr_data;
            ptr_data += 42;
        }
        countmenosuno = tx_input_count - 1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  e = 0; e < countmenosuno;e++ ){
                i_cmp = std::memcmp(ptr_input[e]+2, ptr_input[e+1]+2, 40);
                if(i_cmp > 0){
                    ptr_data = ptr_input[e];
                    ptr_input[e] = ptr_input[e+1];
                    ptr_input[e+1] = ptr_data;
                }
            }
        }
    }

    if(reference_input_count > 0){
        ptr_data = reference_input.data();
        for(std::uint16_t  i= 0; i<reference_input_count;i++ ){ // asigno las localizaciones a punteros
            ptr_reference_input[i] = ptr_data;
            ptr_data += 42;
        }

        countmenosuno = reference_input_count - 1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  e = 0; e < countmenosuno;e++ ){
                i_cmp = std::memcmp(ptr_reference_input[e]+2, ptr_reference_input[e+1]+2, 40);
                if(i_cmp > 0){
                    ptr_data = ptr_reference_input[e];
                    ptr_reference_input[e] = ptr_reference_input[e+1];
                    ptr_reference_input[e+1] = ptr_data;
                }
            }
        }

    }

    if(collateral_input_count > 0){
        ptr_data = collateral_input.data();
        for(std::uint16_t  i= 0; i<collateral_input_count;i++ ){ // asigno las localizaciones a punteros
            ptr_collateral_input[i] = ptr_data;
            ptr_data += 42;
        }

        countmenosuno = collateral_input_count - 1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  e = 0; e < countmenosuno;e++ ){
                i_cmp = std::memcmp(ptr_collateral_input[e]+2, ptr_collateral_input[e+1]+2, 40);
                if(i_cmp > 0){
                    ptr_data = ptr_collateral_input[e];
                    ptr_collateral_input[e] = ptr_collateral_input[e+1];
                    ptr_collateral_input[e+1] = ptr_data;
                }
            }
        }

    }

    if(datum_input_count > 0){
        ptr_data = datum_input.data();
        for(std::uint16_t  i= 0; i<datum_input_count;i++ ){ // asigno las localizaciones a punteros
            ptr_datums[i] = ptr_data;
            ptr_data += extract8bytestoUint64(ptr_data+2) + 10;
        }

        // reasigna los index
        for(std::uint16_t  i = 0; i < datum_input_count; i++ ){
            for(std::uint16_t  e = 0; e < tx_input_count;e++ ){
                if(ptr_datums[i][0] == ptr_input[e][0] && ptr_datums[i][1] == ptr_input[e][1]){
                    replaceUint16toVector(ptr_datums[i], e );
                    break;
                }
            }

        }
        // ordena los punteros
        countmenosuno = datum_input_count-1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  c = 0; c < countmenosuno; c++ ){
                if(extract2bytestoUint16(ptr_datums[c]) > extract2bytestoUint16(ptr_datums[c+1])){
                    ptr_data = ptr_datums[c];
                    ptr_datums[c] = ptr_datums[c+1];
                    ptr_datums[c+1] = ptr_data;
                    break;
                }
            }
        }

    }

    if(redeemer_input_count > 0){
        ptr_data = redeemer_input.data();
        for(std::uint16_t  i= 0; i<redeemer_input_count;i++ ){ // asigno las localizaciones a punteros
            ptr_redeemers[i] = ptr_data;
            ptr_data += extract8bytestoUint64(ptr_data + 3) + 11;
            ptr_data += extract8bytestoUint64(ptr_data) + 8;

        }
        // reasigna los index
        for(std::uint16_t  i = 0; i < redeemer_input_count; i++ ){
            for(std::uint16_t  e = 0; e < tx_input_count;e++ ){
                if(ptr_redeemers[i][0] == ptr_input[e][0] && ptr_redeemers[i][1] == ptr_input[e][1]){
                    replaceUint16toVector(ptr_redeemers[i], e );
                    break;
                }
            }
        }
        // reordena los punteros
        countmenosuno = redeemer_input_count-1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  c = 0; c < countmenosuno; c++ ){
                if(extract2bytestoUint16(ptr_redeemers[c]) > extract2bytestoUint16(ptr_redeemers[c+1])){
                    ptr_data = ptr_redeemers[c];
                    ptr_redeemers[c] = ptr_redeemers[c+1];
                    ptr_redeemers[c+1] = ptr_data;
                    break;
                }
            }
        }


    }


    // Reemplaza los vectores actuales
    if(tx_input_count > 0){
        vector_data->clear();
        vector_data->reserve( tx_input_count * 42 );
        for(std::uint16_t  i = 0; i < tx_input_count; i++ ){
            vector_data->insert(vector_data->end(),ptr_input[i] ,ptr_input[i] + 42);
        }
        tx_input.assign( vector_data->begin(), vector_data->end() );
    }

    if(reference_input_count > 0){
        vector_data->clear();
        vector_data->reserve( reference_input_count * 42 );
        for(std::uint16_t  i = 0; i < reference_input_count; i++ ){
            vector_data->insert(vector_data->end(),ptr_reference_input[i] ,ptr_reference_input[i] + 42);
        }
        reference_input.assign( vector_data->begin(), vector_data->end() );
    }

    if(collateral_input_count > 0){
        vector_data->clear();
        vector_data->reserve( collateral_input_count * 42 );
        for(std::uint16_t  i = 0; i < collateral_input_count; i++ ){
            vector_data->insert(vector_data->end(),ptr_collateral_input[i] ,ptr_collateral_input[i] + 42);
        }
        collateral_input.assign(vector_data->begin(), vector_data->end() );
    }

    if(datum_input_count > 0){
        vector_data->clear();
        for(std::uint16_t  i = 0; i < datum_input_count; i++ ){
            std::size_t ptr_datums_len = extract8bytestoUint64(ptr_datums[i]+2) + 10;
            vector_data->insert(vector_data->end(),ptr_datums[i] ,ptr_datums[i] + ptr_datums_len);
        }
        datum_input.assign(vector_data->begin(), vector_data->end());
    }

    if(redeemer_input_count > 0){
        vector_data->clear();
        for(std::uint16_t  i = 0; i < redeemer_input_count; i++ ){
            std::size_t ptr_redeeemers_len = extract8bytestoUint64(ptr_redeemers[i] + 3) + 11;
            ptr_redeeemers_len += extract8bytestoUint64(ptr_redeemers[i] + ptr_redeeemers_len) + 8;
            vector_data->insert(vector_data->end(),ptr_redeemers[i] ,ptr_redeemers[i] + ptr_redeeemers_len);
        }
        redeemer_input.assign(vector_data->begin(), vector_data->end());
    }


}

std::uint32_t const & TransactionsInputs::getBodyMapcountbit() const{
    return bodymap_countbit;
}

std::uint16_t const & TransactionsInputs::getWitnessMapcountbit() const{
    return witnessmap_countbit;
}

std::uint8_t const TransactionsInputs::getGlobalReferencesScriptsType() const{
    return static_cast<std::uint8_t>(globalreferencescript);
}

std::uint16_t const & TransactionsInputs::getInputsCount() const{
    return tx_input_count;
}
std::uint16_t const & TransactionsInputs::getInputsReferencesCount() const{
    return reference_input_count;
}
std::uint16_t const & TransactionsInputs::getCollateralCount() const{
    return collateral_input_count;
}
std::uint16_t const & TransactionsInputs::getSpendingDatumsCount() const{
    return datum_input_count;
}
std::uint16_t const & TransactionsInputs::getSpendingRedeemersCount() const{
    return redeemer_input_count;
}
std::uint16_t const & TransactionsInputs::getPlutusV1ScriptsCount() const{
    return plutusscript1_input_count;
}
std::uint16_t const & TransactionsInputs::getPlutusV2ScriptsCount() const{
    return plutusscript2_input_count;
}
std::uint16_t const & TransactionsInputs::getNativeScriptsCount() const{
    return nativescript_input_count;
}


std::vector<std::uint8_t> const & TransactionsInputs::getInputs() const{
    return tx_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getInputsReferences() const{
    return reference_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getCollateral() const{
    return collateral_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getSpendingDatums() const{
    return datum_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getSpendingRedeemers() const{
    return redeemer_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getPlutusV1Scripts() {
    replaceUint16toVector(plutusscript1_input.data(),plutusscript1_input_count);
    return plutusscript1_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getgetPlutusV2Scripts() {
    replaceUint16toVector(plutusscript2_input.data(),plutusscript2_input_count);
    return plutusscript2_input;
}
std::vector<std::uint8_t> const & TransactionsInputs::getNativeScripts() {
    replaceUint16toVector(nativescript_input.data(),nativescript_input_count);
    return nativescript_input;
}
