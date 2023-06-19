#include "transactionoutput.hpp"
/// Vector<std::uint8_t> tx_output:
///  + 1(separador) "0 = indica el inicio/fin de esta transaccion"
///    1(Cantidad de elementos en este mapa "{1,2,..}")
///    1(indica si no existen assets en la transaccion) "0 = si hay, 1 = no hay", se usa para determinar el formato "amount / [amount , multiassets]"
///    1(address_keyhash_len) + 57(address_keyhash) + 8(amount)
///  + 1(separador)   "1 = asset"
///    8(multiasset_data_cbor_len) + n(multiasset_data_cbor)
///  + 1(separador)   "2 = datum value / 3 = datum hash"
///    8(datum_data_cbor_len) + n(datum_data_cbor)
///  + 1(separador)   "4 = reference script"
///    8(script_data_cbor_len) + n(script_data_cbor)
///  + 1(separador)  "5 = colateralreturn"
///    1(address_keyhash_len) + 57(address_keyhash) + 8(amount)
/// 94+n bytes maximo de largo cada output

TransactionsOutputs::TransactionsOutputs(){
    //cbor.reset(new CborSerialize( &cbor_array ));
    outputmap_countbit = 0;
    bodymap_countbit = 0; ///  0x0002 , Tiene que iniciar con cero
    tx_output_count = 0;      // maximo 65534
    buff_sizet = 0;
    addr_keyhash_buffer_len = 0;
    pos_registro_elementos = 0;
}

// 0: address
// 1: value / [value , multiasset]
TransactionsOutputs &TransactionsOutputs::addOutput(std::uint8_t const * const address_keyhash, std::size_t const & address_keyhash_len, std::uint64_t const & amount){

    if(outputmap_countbit & 0x02){ // si tiene un asset previo, se borran los datos
            std::vector<std::uint8_t> const & cbor_array = getCborMultiassets();
            tx_output.push_back(0x01); //separador
            addUint64toVector(tx_output, cbor_array.size()); // se agrega el largo de la cadena data_cbor
            tx_output.insert(tx_output.end(),cbor_array.begin(),cbor_array.end()); // se agrega data_cbor
            outputmap_countbit &= 0xfb;  // se pone a cero el outputmap_countbit para asset, en caso de un error evita que se vuelva a copiar
            tx_output[pos_registro_elementos] += 1;  // se agrega el conteo a asset
            tx_output[pos_registro_elementos+1] = 0;  // indica que existen assets en este output
            capsula.clear(); // se borran los assets anteriores

    }

    if(address_keyhash_len == 29 || address_keyhash_len == 57){
        outputmap_countbit &= 0xf0; //se borran todos los bits excepto el de colateral return, ya que solo puede haber uno en transaccion output
        if(tx_output_count < UINT16_MAX ){  // se procede si no excede el limite maximo de almacenaje


            //----------- Para optimizar el almacenamiento y uso de memoria ------------
            buff_sizet = static_cast<std::size_t>( tx_output.capacity() ) - static_cast<std::size_t>( tx_output.size() );
            addr_keyhash_buffer_len = address_keyhash_len + 10;

            // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
            if(buff_sizet < addr_keyhash_buffer_len ) {
                tx_output.reserve(tx_output.size() + addr_keyhash_buffer_len);  // Se reserva el espacio necesario para almacenar la direccion y el monto
            }
            //---------------------------------------------------------------

            tx_output_count++;
            tx_output.push_back(0); // indica que es el inicio de esta transaccion
            tx_output.push_back(1); // indica la cantidad de elementos de esta transaccion, se inicia agregando output
            pos_registro_elementos = tx_output.size() - 1; // guardas la posicion del vector donde se debe registrarse la cantidad de elementos
            tx_output.push_back(1); // agrega el indicador que define la existencia de assets en este output, 1 = no hay
            tx_output.push_back(static_cast<std::uint8_t>(address_keyhash_len)); //Agrega el address_keyhash_len en un bytes antes del address_keyhash
            tx_output.insert(tx_output.end(), address_keyhash, address_keyhash + address_keyhash_len); //Agrega el array address_keyhash
            addUint64toVector(tx_output, amount); //Agrega el amount en 8 bytes
            //tx_output.push_back(0); // cierra esta transaccion
            bodymap_countbit |= 0x0002;
            outputmap_countbit |= 0x01;

        }

    }

    return *this;
}

TransactionsOutputs &TransactionsOutputs::addOutput(std::string const payment_address, std::uint64_t const amount){

    if(bech32_decode(payment_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){
        addOutput(addr_keyhash_buffer, addr_keyhash_buffer_len, amount);
    }

    return *this;
}

TransactionsOutputs &TransactionsOutputs::addColateralReturn(std::uint8_t const * const address_keyhash, std::size_t const & address_keyhash_len, std::uint64_t const & amount){
if((outputmap_countbit & 0x10) == 0){   // si no existe un returncolateral


        outputmap_countbit = 0;
        if(tx_output_count < UINT16_MAX ){  // se procede si no excede el limite maximo de almacenaje


            //----------- Para optimizar el almacenamiento y uso de memoria ------------
            buff_sizet = static_cast<std::size_t>( tx_output.capacity() ) - static_cast<std::size_t>( tx_output.size() );
            addr_keyhash_buffer_len = address_keyhash_len + 10;

            // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
            if(buff_sizet < addr_keyhash_buffer_len ) {
                tx_output.reserve(tx_output.size() + addr_keyhash_buffer_len);  // Se reserva el espacio necesario para almacenar la direccion y el monto
            }
            //---------------------------------------------------------------

            tx_output_count++;
            tx_output.push_back(5); // separador, indica que es el inicio de esta transaccion
            tx_output.push_back(static_cast<std::uint8_t>(address_keyhash_len)); //Agrega el address_keyhash_len en un bytes antes del address_keyhash
            tx_output.insert(tx_output.end(), address_keyhash, address_keyhash + address_keyhash_len); //Agrega el array address_keyhash
            addUint64toVector(tx_output, amount); //Agrega el amount en 8 bytes
            //tx_output.push_back(0); // cierra esta transaccion
            bodymap_countbit |= 0x10000;  // indica a txbody que existe un return colateral
            outputmap_countbit |= 0x10;

        }

    }


    return *this;
}

TransactionsOutputs &TransactionsOutputs::addColateralReturn(std::string const payment_address, std::uint64_t const amount){

    if(bech32_decode(payment_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){
        addColateralReturn(addr_keyhash_buffer, addr_keyhash_buffer_len, amount);
    }

    return *this;
}

//          [x][0]     --> cantidad de elementos en el mapa cbor (policyID o assetname)
//capsula:  [0][x+1]   --> POLICYID(1),...,POLICYID2(n-1)
//          [1][x+1]   --> assetname,assetname,assetname
//          [n-1][x+1] --> assetname,assetname,assetname


// multiasset = { * policyID : * {assetname : amount }}
TransactionsOutputs &TransactionsOutputs::addAsset(std::uint8_t const *const policyID, std::uint8_t const *const assetname, std::size_t const &assetname_len, std::uint64_t const amount){

   if((outputmap_countbit & 0x01)){  //si existe un output

     if( capsula.size() == 0 ){ //si no se han creado datos en la capsula se crea el primer dato
                capsula.push_back(std::vector<std::uint8_t>(0));
                capsula[0].push_back(0); //se agrega el primer elemento del array
                capsula[0].reserve(30);
    }
    std::uint8_t igual = 0;   // indica si el policyid es el mismo
    std::size_t posicion = 0; // parte de la posicion cero
    std::vector<std::uint8_t>::iterator it = capsula[0].begin();

    //----- Comprueba si se repiten los policyID ---------
    for(int b = 0;b<capsula[0][0];b++){ // solo si existen policyID en capsula[0] se entra a comparar
        posicion += 1;                  // se pasa a la siguiente posicion (columna)
        igual = 0;
        for(int a = 0; a < 28 ;a++){  // compara en largos de 28bytes, revisa si se repiten los policyID
            it++;                     // inicia saltando a la primera posicion
            if(*it == policyID[a]){
                igual++;
            }
        }
        if(igual == 28){    //si encuentra similitud sale del bucle
            break;  // o it = capsula[0].end();
        }
    }
    //-------------------------------------------------

    if(assetname_len < 32){   // el largo del nombre no debe exceder los 32 bytes
        if(igual == 28){      // Si se encontro el mismo PolicyID , solo se agregan el assetname

            cbor.clearCbor();
            cbor.addBytesArray(assetname, assetname_len);
            cbor.addUint(amount);
            std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();
            capsula[posicion][0] += 1;  ///aumenta la cantidad de elementos en el mapa ///PONER LIMITE A 254 elementos
            capsula[posicion].insert( capsula[posicion].end(), cbor_array.begin(), cbor_array.end() ); //inserta los datos en cbor

        }else{              // Si no encontro el mismo PolicyID , solo se agregan el policyID y el assetname
            capsula[0][0] += 1;
            capsula[0].insert (capsula[0].end(), policyID, policyID+28 ); //agrego el nuevo policyID, a la primera columna
            capsula.push_back(std::vector<std::uint8_t>(0));  //se crea otra columna para almacenar los datos de esa posicion
            posicion += 1; // se pasa a la siguiente posicion (columna)
            capsula[posicion].push_back(0); //se agrega el primer elemento del array en esa posicion

            cbor.clearCbor();
            cbor.addBytesArray(assetname, assetname_len);
            cbor.addUint(amount);
            std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();
            capsula[posicion][0] += 1;  //aumenta la cantidad de elementos en el mapa
            capsula[posicion].insert( capsula[posicion].end(), cbor_array.begin(), cbor_array.end() ); //inserta los datos en cbor

        }
        outputmap_countbit |= 0x02;
    }

   }

    return *this;

    }
TransactionsOutputs &TransactionsOutputs::addAsset(std::uint8_t const *const policyID, std::string assetname, std::uint64_t const amount){

    std::uint8_t assetname_len = assetname.size();
    std::uint8_t asset_name[assetname_len]{};
    for(std::uint8_t a = 0; a < assetname_len; a ++){
        asset_name[a] = static_cast<uint8_t>(assetname[a]);

    }
    addAsset(policyID, asset_name, assetname_len, amount);

    return *this;
}

// ? datum = h'hash32'
TransactionsOutputs &TransactionsOutputs::addDatumHash(std::uint8_t const *const datum_hash, std::size_t const &datum_hash_len){
 if((outputmap_countbit & 0x05) == 1){   // si existe un output y no existe un datumhash
 cbor.clearCbor();
 cbor.addBytesArray(datum_hash, datum_hash_len);
 std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();

 tx_output.push_back(2); //separador
 addUint64toVector(tx_output, cbor_array.size());               // cantidad de bytes del datumhash
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end()); // datumhash
 outputmap_countbit |= 0x04;             // indica que existe un datum  o un datumhash ligado a una direccion
 tx_output[pos_registro_elementos] +=1; // se aumenta en 1 el contador de elementos en tx_output
 }
 return *this;
}

// ? datum = h'hash32'
TransactionsOutputs &TransactionsOutputs::addDatumHashcreatedfromJson(std::string &json_datum){
std::unique_ptr<PlutusJsonSchema> Json_p(new PlutusJsonSchema());
Json_p->addSchemaJson(json_datum);

 if((outputmap_countbit & 0x05) == 1){   // si existe un output y no existe un datumhash
 cbor.clearCbor();
 cbor.addBytesArray(Json_p->getHash32CborSchemaJson(), 32);
 std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();

 tx_output.push_back(2); //separador
 addUint64toVector(tx_output, cbor_array.size());               // cantidad de bytes del datumhash
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end()); // datumhash
 outputmap_countbit |= 0x04;            // indica que existe un datum  o un datumhash ligado a una direccion
 tx_output[pos_registro_elementos] +=1; // se aumenta en 1 el contador de elementos en tx_output
 }
 return *this;
}

// ? datum = 24(h'datum_value cbor')]
TransactionsOutputs &TransactionsOutputs::addDatumIntValue(std::uint64_t const integer_datum){
 if((outputmap_countbit & 0x05) == 1 ){  // si existe un output y no existe un datumvalue
 cbor.clearCbor();
 cbor.addTag(24);
 cbor.addUint2BytesArray(integer_datum);
 std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();

 tx_output.push_back(3); //separador
 addUint64toVector(tx_output, cbor_array.size()); // cantidad de bytes del datum
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end()); // datum
 outputmap_countbit |= 0x04;             // indica que existe un datum  o un datumhash ligado a una direccion
 tx_output[pos_registro_elementos] +=1; // se aumenta en 1 el contador de elementos en tx_output
 }
 return *this;
}

// ? datum = 24(h'datum_value cbor')]
TransactionsOutputs &TransactionsOutputs::addDatum(std::string &json_datum){
std::unique_ptr<PlutusJsonSchema> Json_p(new PlutusJsonSchema());
Json_p->addSchemaJson(json_datum);
 if((outputmap_countbit & 0x05) == 1 ){  // si existe un output y no existe un datumvalue
 cbor.clearCbor();
 cbor.addTag(24);
 cbor.addBytesArray(Json_p->getCborSchemaJson());
 std::vector<std::uint8_t> const & cbor_array = cbor.getCbor();

 tx_output.push_back(3); //separador
 addUint64toVector(tx_output, cbor_array.size()); // cantidad de bytes del datum
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end());  // datum
 outputmap_countbit |= 0x04;            // indica que existe un datum  o un datumhash ligado a una direccion
 tx_output[pos_registro_elementos] +=1; // se aumenta en 1 el contador de elementos en tx_output
 }
 return *this;
}

//? script_ref = 24(h'script')
TransactionsOutputs &TransactionsOutputs::addReferenceScript(TransactionsOutputs::ScriptType script_type, std::uint8_t const *const script_, std::size_t &script_len ){
if((outputmap_countbit & 0x09) == 1 ){ // si existe un output y no existe un scriptref
std::vector<std::uint8_t> buff_vector;
cbor.clearCbor();
cbor.createArray(2);
cbor.addUint(static_cast<std::uint64_t>(script_type));
cbor.bypassPtrUint8Cbor(script_, script_len);
std::vector<std::uint8_t> const & cbor_array = cbor.getCbor(); // como es un alias a la valiable privada de cbor, cbor_array actualizara su valor si cbor cambia mas adelante

buff_vector.insert(buff_vector.begin(),cbor_array.begin(),cbor_array.end());
cbor.clearCbor();
cbor.addTag(24);
cbor.addBytesArray(buff_vector.data(),buff_vector.size());

tx_output.push_back(4); //separador
addUint64toVector(tx_output, cbor_array.size());
tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end());
outputmap_countbit |= 0x08;
tx_output[pos_registro_elementos] +=1;

}
return *this;
}


std::uint32_t const &TransactionsOutputs::getBodyMapcountbit() const{
   return bodymap_countbit;
}


std::vector<std::uint8_t> const & TransactionsOutputs::getCborMultiassets(){
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


std::vector<std::uint8_t> const & TransactionsOutputs::getTransactionsOutputs() {
    if(outputmap_countbit & 0x02){    // en caso de que no se cree una nueva salida (addOutput), se escriben los asset almacenados en memoria a la ultima salida
            std::vector<std::uint8_t> const & cbor_array = getCborMultiassets();
            tx_output.push_back(0x01); //separador
            addUint64toVector(tx_output, cbor_array.size()); // se agrega el largo de la cadena data_cbor
            tx_output.insert(tx_output.end(),cbor_array.begin(),cbor_array.end()); // se agrega data_cbor
            outputmap_countbit &= 0xfb;  // se pone a cero el outputmap_countbit para asset, en caso de un error evita que se vuelva a copiar
            tx_output[pos_registro_elementos] += 1;  // se agrega el conteo a assets
            tx_output[pos_registro_elementos+1] = 0;  // indica que existen assets en este output
            //capsula.clear(); // se borran los assets anteriores
    }
    return tx_output;
}

std::uint16_t const &TransactionsOutputs::getAmountTransactionsOutputs() const {
return tx_output_count;
}
