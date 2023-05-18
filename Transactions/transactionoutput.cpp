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
    cbor.reset(new CborSerialize( &cbor_array ));
    outputmap_countbit = 0;
    bodymap_countbit = 0; ///  0x0002 , Tiene que iniciar con cero
    tx_output_count = 0;      // maximo 65534
    buff_sizet = 0;
}

// 0: address
// 1: value / [value , multiasset]
TransactionsOutputs &TransactionsOutputs::addOutput(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount){

    if(outputmap_countbit & 0x02){ // si tiene un asset previo, se borran los datos
            getCborMultiassets();
            tx_output.push_back(0x01); //separador
            agregarUint64BytestoVector(tx_output, cbor_array.size()); // se agrega el largo de la cadena data_cbor
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
            agregarUint64BytestoVector(tx_output, amount); //Agrega el amount en 8 bytes
            //tx_output.push_back(0); // cierra esta transaccion
            bodymap_countbit |= 0x0002;
            outputmap_countbit |= 0x01;

        }

    }

    return *this;
}

TransactionsOutputs &TransactionsOutputs::addOutput(std::string payment_address, std::uint64_t const amount){

    if(bech32_decode(payment_address.c_str(), addr_keyhash_buffer, &addr_keyhash_buffer_len)){
        addOutput(addr_keyhash_buffer, addr_keyhash_buffer_len, amount);
    }

    return *this;
}

TransactionsOutputs &TransactionsOutputs::addReturnColateral(std::uint8_t const *const address_keyhash, std::size_t address_keyhash_len, std::uint64_t const amount){
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
            agregarUint64BytestoVector(tx_output, amount); //Agrega el amount en 8 bytes
            //tx_output.push_back(0); // cierra esta transaccion
            bodymap_countbit |= 0x10000;  // indica a txbody que existe un return colateral
            outputmap_countbit |= 0x10;

        }

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

            cbor->ClearCbor();
            cbor->addBytesArray(assetname, assetname_len);
            cbor->addUint(amount);
            capsula[posicion][0] += 1;  ///aumenta la cantidad de elementos en el mapa ///PONER LIMITE A 254 elementos
            capsula[posicion].insert( capsula[posicion].end(), cbor_array.begin(), cbor_array.end() ); //inserta los datos en cbor

        }else{              // Si no encontro el mismo PolicyID , solo se agregan el policyID y el assetname
            capsula[0][0] += 1;
            capsula[0].insert (capsula[0].end(), policyID, policyID+28 ); //agrego el nuevo policyID, a la primera columna
            capsula.push_back(std::vector<std::uint8_t>(0));  //se crea otra columna para almacenar los datos de esa posicion
            posicion += 1; // se pasa a la siguiente posicion (columna)
            capsula[posicion].push_back(0); //se agrega el primer elemento del array en esa posicion

            cbor->ClearCbor();
            cbor->addBytesArray(assetname, assetname_len);
            cbor->addUint(amount);
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

// ? datum = hash32
TransactionsOutputs &TransactionsOutputs::addDatumHash(std::uint8_t const *const datum_hash, std::size_t &datum_hash_len){
 if((outputmap_countbit & 0x05) == 1){   // si existe un output y no existe un datumhash
 cbor->ClearCbor();
 cbor->addBytesArray(datum_hash, datum_hash_len);

 tx_output.push_back(2); //separador
 agregarUint64BytestoVector(tx_output, cbor_array.size());
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end());
 outputmap_countbit |= 0x04;
 tx_output[pos_registro_elementos] +=1;
 }
 return *this;
}

// ? datum = 24(h'datum_value cbor')]
TransactionsOutputs &TransactionsOutputs::addDatumValue(std::uint64_t const datum_value){
 if((outputmap_countbit & 0x05) == 1 ){  // si existe un output y no existe un datumvalue
 cbor->ClearCbor();
 cbor->addTag(24);
 cbor->addUint2BytesArray(datum_value);

 tx_output.push_back(3); //separador
 agregarUint64BytestoVector(tx_output, cbor_array.size());
 tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end());
 outputmap_countbit |= 0x04;
 tx_output[pos_registro_elementos] +=1;
 }
 return *this;
}

//? script_ref = 24(h'script')
TransactionsOutputs &TransactionsOutputs::addReferenceScript(TransactionsOutputs::ScriptType script_type, std::uint8_t const *const script_, std::size_t &script_len ){
if((outputmap_countbit & 0x09) == 1 ){ // si existe un output y no existe un scriptref
std::vector<std::uint8_t> buff_vector;
cbor->ClearCbor();
cbor->createArray(2);
cbor->addUint(static_cast<std::uint64_t>(script_type));
cbor->bypassPtrUint8Cbor(script_, script_len);
buff_vector.insert(buff_vector.begin(),cbor_array.begin(),cbor_array.end());
cbor->ClearCbor();
cbor->addTag(24);
cbor->addBytesArray(buff_vector.data(),buff_vector.size());

tx_output.push_back(4); //separador
agregarUint64BytestoVector(tx_output, cbor_array.size());
tx_output.insert(tx_output.end(), cbor_array.begin(),cbor_array.end());
outputmap_countbit |= 0x08;
tx_output[pos_registro_elementos] +=1;

}
return *this;
}


std::uint32_t const &TransactionsOutputs::getBodyMapcountbit() const{
   return bodymap_countbit;
}


void TransactionsOutputs::getCborMultiassets(){
    cbor->ClearCbor();
    int policyID_count = static_cast<std::uint64_t>(capsula[0][0]);
    if(policyID_count != 0){ // si no esta vacio se procede con el resto
    std::uint8_t *ptr_policyID = capsula[0].data();
    ptr_policyID++; //salta la primera posicion
    cbor->createMap(capsula[0][0]);

    policyID_count += 1; //aumento en uno, para que concuerde la posicion de los datos en las otras columnas
    for(int a = 1; a < policyID_count;a++){
     cbor->addBytesArray(ptr_policyID,28);
     cbor->createMap(static_cast<std::uint64_t>(capsula[a][0]));
     cbor->bypassIteratorVectorCbor(capsula[a].begin()+1, capsula[a].end());
     ptr_policyID += 28;
    }
    }
}


std::vector<std::uint8_t> const &TransactionsOutputs::getTransactionsOutputs() {
    if(outputmap_countbit & 0x02){    // en caso de que no se cree una nueva salida (addOutput), se escriben los asset almacenados en memoria a la ultima salida
            getCborMultiassets();
            tx_output.push_back(0x01); //separador
            agregarUint64BytestoVector(tx_output, cbor_array.size()); // se agrega el largo de la cadena data_cbor
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
