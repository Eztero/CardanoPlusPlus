#include "plutusjsonschema.hpp"
#include <iomanip>

namespace Cardano{
namespace Utils{

PlutusJsonSchema::PlutusJsonSchema(){
    cborschema.reserve(150);
};

PlutusJsonSchema::~PlutusJsonSchema(){
    cborschema.clear();
};

std::size_t PlutusJsonSchema::find_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end){
    std::size_t posicion_it = 0;

    while(it+posicion_it <= it_end){
        if(it[posicion_it] == caracter){
            return posicion_it;
        }
        posicion_it++;
    }

    return std::string::npos;
}

std::size_t PlutusJsonSchema::pos_primer_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end ){
    std::size_t posicion_it = 0;

    while(it + posicion_it != it_end){
        switch( *(it + posicion_it) ){
        case ' ':{posicion_it++;};break;
        default:{
            if( *(it + posicion_it ) == caracter){
                return posicion_it;
            }else{
                return std::string::npos;
            }
        };break;
        }
    }
    return std::string::npos;
}

std::size_t PlutusJsonSchema::pos_ultimo_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end ){
    std::size_t posicion_it = 0;

    if(*it_end == '\0'){
        posicion_it++;
    }

    while(it_end - posicion_it != it){
        switch( *(it_end-posicion_it) ){
        case ' ':{posicion_it++;};break;
        default:{
            if(*(it_end - posicion_it) == caracter){
                return std::distance(it,it_end) - (posicion_it);
            }else{
                return std::string::npos;
            }
        };break;
        }
    }

    return std::string::npos;
}

std::size_t PlutusJsonSchema::posfinal_primer_string_it(std::string const frase, std::string::const_iterator it, std::string::const_iterator it_end ){

    std::size_t posicion_it = 0;
    std::size_t a = 0;
    std::size_t const t = frase.size();

    while(it+posicion_it != it_end && a < t){

        switch(*(it+posicion_it)){
        case ' ':{posicion_it++;};break;
        default:{
            if(frase[a] == *(it+posicion_it)){
                a++;
                posicion_it++;
            }else{
                return std::string::npos;
            }
        };break;
        }
    }
    return posicion_it;
}

bool PlutusJsonSchema::es_igual_ydesplazaIt(std::string const frase, std::string::iterator &it, std::string::const_iterator const &it_end ){ //  busca una igualdad y ademas cambia la posicion de it durante su busqueda
    std::size_t const t = frase.size();

    for(std::size_t a = 0 ; a < t; a++){
        if(it == it_end || frase[a] != *it){
            return false;
        }
        it++;
    }

    return true;
}

std::uint64_t PlutusJsonSchema::obtener_int_constructor_str(std::string::iterator &it, std::string::const_iterator const &it_end){


    std::size_t cierre_valor = 0;
    bool ciclo_w = true;
    while(ciclo_w && ((it+cierre_valor) != it_end) ){
        if((it+cierre_valor) != it_end ){
            switch(*(it+cierre_valor)){
            case ' ':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{cierre_valor++;};break;
            case ',':
            case '}':{
                ciclo_w = false;
            };break;
            default:{
                throw std::invalid_argument("error in \"constructor\":  positive numbers are expected  )");
            };break;
            }

        }else{
            throw std::invalid_argument("error in \"constructor\": missing a ( } )");
        }

    }
    // ahora cierre_valor contiene la posicion del } que se usa para extraer los valores int
    std::string recorte;
    recorte.append(it, it+cierre_valor);
    std::uint64_t r ;
    try{
        r = std::stoull(recorte, nullptr, 10);
    }catch(std::out_of_range & nerr){
        throw std::out_of_range(" error in \"constructor\" : number out of range, they must be in the range  0 .. 2^64");
    }
    it += cierre_valor;  // se dezplaza al final del numero
    return r;

}

std::uint64_t PlutusJsonSchema::obtener_int_str(std::string::iterator &it, std::string::const_iterator const &it_end, bool &npositivo){

    std::size_t cierre_valor = 0;
    std::size_t inicio_valor = 0;
    bool ciclo_w = true;
    while(ciclo_w ){
        if((it+cierre_valor) != it_end ){

            switch(*(it+cierre_valor)){
            case ' ':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{cierre_valor++;};break;
            case '-': {npositivo = false; cierre_valor++; inicio_valor = cierre_valor;};break;/// una condicion para agregar numeros negativos
            case '}':{
                ciclo_w = false;
            };break;
            default:{
                throw std::invalid_argument("error in \"int\":  numbers are expected  )");
            };break;
            }

        }else{
            throw std::invalid_argument("error in \"int\": missing a ( } )");
        }

    }

    // ahora cierre_valor contiene la posicion del } que se usa para extraer los valores int
    std::string recorte{};
    recorte.append(it+inicio_valor, it+cierre_valor);
    std::uint64_t r;
    try{
        r = std::stoull(recorte, nullptr, 10);
    }catch(std::out_of_range & nerr){
        throw std::out_of_range(" error in \"int\" : number out of range, they must be in the range -2^64 .. 2^64");
    }
    it += cierre_valor + 1;  // se dezplaza fuera del }
    return r;
}

bool PlutusJsonSchema::obtener_bytes_str(std::string::iterator &it, std::string::const_iterator const &it_end, std::vector<std::uint8_t> &bytes_vector){
    // para bytes
    std::size_t cierre_valor = find_caracter_it('}', it, it_end);
    if(cierre_valor != std::string::npos){
        std::string recorte;
        std::size_t inicio_byte = pos_primer_caracter_it('"',it, it_end);
        if(inicio_byte != std::string::npos){
            inicio_byte++; //para saltar el " del inicio
            std::size_t fin_byte = find_caracter_it('"',it+inicio_byte, it+cierre_valor); // lee solo hasta el }
            if(fin_byte != std::string::npos){
                fin_byte += inicio_byte; // se suma los espacios de inicio_byte porque el calculo de fin_byte parte adelantado

                if(inicio_byte < cierre_valor && fin_byte < cierre_valor){
                    if(fin_byte - inicio_byte <= 128 ){ // 64 bytes * 2 = 128, el maximo de la cadena debe de ser 64bytes
                        recorte.append(it+inicio_byte, it+fin_byte);
                        std::size_t bytes_uint8_len = 0;
                        std::uint8_t const *bytes_uint8 = hexchararray2uint8array(recorte, &bytes_uint8_len);
                        if(bytes_uint8 != nullptr){
                            bytes_vector.assign(bytes_uint8, bytes_uint8+bytes_uint8_len);
                            delete[] bytes_uint8;
                            it += fin_byte + 1;
                            cierre_valor= pos_primer_caracter_it('}',it, it_end);
                            if(cierre_valor != std::string::npos){
                                it += cierre_valor + 1;
                                return true;
                            }else{
                                throw std::invalid_argument("error in \"bytes\": missing a ( } ) ");
                            }
                        }else{
                            throw std::invalid_argument("error in \"bytes\": invalid hexadecimal character");
                        }

                    }else{
                        throw std::invalid_argument("error in \"bytes\": exceeds 64 bytes");
                    }

                }else{
                    throw std::invalid_argument("error in \"bytes\": missing a ( \" ) ");
                }
            }else{
                throw std::invalid_argument("error in \"bytes\": missing a ( \" ) at end of byte string ");
            }
        }else{
            throw std::invalid_argument("error in \"bytes\": missing a ( \" ) begin of byte string");
        }
    }
    else{
        throw std::invalid_argument("error in \"bytes\": missing a ( } )");
    }
    return false;
}

bool PlutusJsonSchema::obtener_key_value_map(std::string::iterator &it, std::string::const_iterator &it_end, std::vector<std::uint8_t> &key_cbor, std::vector<std::uint8_t> &value_cbor ){
    std::size_t desplazamiento_buffk = posfinal_primer_string_it("{" ,it, it_end );
    std::size_t desplazamiento_buffv = 0;
    std::uint8_t bit_keyvalue = 0; // si es igual a 3 es porque se encontro encontro el value y el key

    if(desplazamiento_buffk != std::string::npos){
        it += desplazamiento_buffk;

        do{

            desplazamiento_buffk = posfinal_primer_string_it(R"("k")",  it, it_end);
            desplazamiento_buffv = posfinal_primer_string_it(R"("v")",  it,  it_end);

            if(desplazamiento_buffk != std::string::npos){
                it += desplazamiento_buffk;
                desplazamiento_buffk = posfinal_primer_string_it(":",  it, it_end);
                if(desplazamiento_buffk != std::string::npos){
                    it += desplazamiento_buffk;
                    //Aca se crea el tipo en cbor
                    key_cbor = obtener_tipo(it, it_end);
                    bit_keyvalue |= 0x01;
                    desplazamiento_buffk = pos_primer_caracter_it(',',it,it_end);
                    if(desplazamiento_buffk != std::string::npos){ // si encuentra una coma, entonces avanza una unidad, si no hara que el ciclo se cierre
                        it += desplazamiento_buffk + 1;
                    }
                }else{
                    throw std::invalid_argument("error in \"map\" elements: a ( : ) is expected at the end of \"k\"");
                }

            }else if(desplazamiento_buffv != std::string::npos){
                it += desplazamiento_buffv;
                desplazamiento_buffv = posfinal_primer_string_it(":",  it, it_end);
                if(desplazamiento_buffv != std::string::npos){
                    it += desplazamiento_buffv;
                    //Aca se crea el tipo en cbor
                    value_cbor = obtener_tipo(it, it_end);
                    bit_keyvalue |= 0x02;
                    desplazamiento_buffv = pos_primer_caracter_it(',',it,it_end);
                    if(desplazamiento_buffv != std::string::npos){ // si encuentra una coma, entonces avanza una unidad, si no hara que el ciclo se cierre
                        it += desplazamiento_buffv +1;
                    }

                }else{
                    throw std::invalid_argument("error in \"map\" elements: a ( : ) is expected at the end of \"v\"");
                }
            }

        }while(desplazamiento_buffk != std::string::npos || desplazamiento_buffv != std::string::npos);


        if(bit_keyvalue != 3){
            std::string ex_error;
            if(bit_keyvalue == 0x01){
                ex_error = "error in \"map\" elements: missing value ( \"v\" )";
            }else if(bit_keyvalue == 0x02){
                ex_error = "error in \"map\" elements: missing key ( \"k\" ) ";
            }else{
                ex_error = "error in \"map\" elements: missing value or key ( \"v\" or \"k\" ) ";
            }
            throw std::invalid_argument(ex_error);
        }


        desplazamiento_buffk = posfinal_primer_string_it("}" ,it, it_end );
        if(desplazamiento_buffk != std::string::npos){
            it += desplazamiento_buffk ; // se sale del { } que contiene el value y key

        }else{
            //return false;
            throw std::invalid_argument("error in \"map\" elements: missing a ( } )");
        }

    }else{
        throw std::invalid_argument("error in \"map\" elements: missing a ( { )");
    }

    return true;
}

std::vector<std::uint8_t> PlutusJsonSchema::obtener_list_cbor(std::string::iterator &it, std::string::const_iterator &it_end){
    std::unique_ptr<CborSerialize> a(new CborSerialize);

    std::size_t pos_corchete_inicio = pos_primer_caracter_it('[', it, it_end); // se delimita el espacio de trabajo de field
    std::size_t pos_corchete_fin = 0;
    std::size_t pos_coma = 0;
    std::vector<std::uint8_t> listvalue_cbor;
    std::vector<std::uint8_t> value_cbor;
    std::uint64_t numero_elementos_lista = 0;



    if(pos_corchete_inicio != std::string::npos){
        it += pos_corchete_inicio + 1; //agrega una unidad para avanzar desde el [

        while(pos_coma != std::string::npos && it < it_end){

            value_cbor = obtener_tipo(it, it_end);
                numero_elementos_lista++;
                listvalue_cbor.insert( listvalue_cbor.end(), value_cbor.begin(), value_cbor.end() );
                value_cbor.clear();


            pos_coma = pos_primer_caracter_it(',',it,it_end);
            if(pos_coma != std::string::npos){
                it += pos_coma + 1; // se avanza la posicion de la coma
            }

        }

        pos_corchete_fin = pos_primer_caracter_it(']', it, it_end);
        if(pos_corchete_fin != std::string::npos){
            it += pos_corchete_fin + 1;
            //a->createArray(numero_elementos_lista); // las listas de momento se representan como array indefinidos
            a->createArrayUndefined();
            a->bypassIteratorVectorCbor(listvalue_cbor.begin(),listvalue_cbor.end());
            a->addBreak();
            return a->getCbor();
        }else{
            throw std::invalid_argument("error in list of element: missing a ( ] )");
        }

    }else{
        throw std::invalid_argument("error in list of element: missing a ( [ )");
    }

    return a->getCbor();
}

std::vector<std::uint8_t> PlutusJsonSchema::obtener_tipo( std::string::iterator &it, std::string::const_iterator &it_end ){
    std::unique_ptr<CborSerialize> a(new CborSerialize);
    std::size_t pos_caracter = 0;

    tipo_t elemento = detectar_tipo(it, it_end);

    if(elemento != tipo_t::tipo_error){

        pos_caracter = pos_primer_caracter_it(':',it,it_end);

        if(pos_caracter != std::string::npos){
            it += pos_caracter+1;

            switch(elemento){
            case tipo_t::tipo_constructor:
            case tipo_t::tipo_constructor_field:{

                std::uint64_t tag_constructor = 0;
                std::vector<std::uint8_t> field_constructor;
                std::size_t pos_coma = 0;

                while(pos_coma != std::string::npos && it < it_end){



                    if(elemento == tipo_t::tipo_constructor){
                        tag_constructor = obtener_int_constructor_str(it, it_end);
                    }
                    else if(elemento == tipo_t::tipo_constructor_field){
                        field_constructor = obtener_list_cbor( it, it_end );
                    }

                    pos_coma = pos_primer_caracter_it(',', it, it_end);
                    if(pos_coma != std::string::npos){
                        it += pos_coma + 1; // se avanza la posicion de la coma
                        elemento = detectar_tipo(it, it_end);

                        if(elemento == tipo_t::tipo_error){
                            throw std::invalid_argument("error in constructor element, unknown element type");
                        }else{
                            pos_caracter = pos_primer_caracter_it(':',it,it_end); // avanza de los :
                            if(pos_caracter != std::string::npos){
                                it += pos_caracter+1;
                            }else{
                                std::string ex_error =" error, ( : ) is expected at the end of  ";
                                switch (elemento){
                                case tipo_t::tipo_int :{ex_error.append("\"int\"");};break;
                                case tipo_t::tipo_bytes :{ex_error.append("\"bytes\"");};break;
                                case tipo_t::tipo_map :{ex_error.append("\"map\"");};break;
                                case tipo_t::tipo_list :{ex_error.append("\"list\"");};break;
                                case tipo_t::tipo_constructor :{ex_error.append("\"constructor\"");};break;
                                case tipo_t::tipo_constructor_field :{ex_error.append("\"fields\"");};break;
                                }
                                throw std::invalid_argument(ex_error);
                            }
                        }

                    }
                }

                pos_coma = pos_primer_caracter_it('}', it, it_end);
                if(pos_coma != std::string::npos){
                    it += pos_coma + 1; // se avanza la posicion del }
                }else{
                    throw std::invalid_argument("error in constructor element, missing ( } ) at end");
                }

                if(tag_constructor < 128){

                    if(tag_constructor < 7){
                        tag_constructor += 121;
                    }else{
                        tag_constructor += 1273;
                    }
                    a->addTag(tag_constructor);

                }else{
                    a->addTag(102);
                    a->createArray(2);
                    a->addUint(tag_constructor);
                }

                a->bypassIteratorVectorCbor(field_constructor.begin(),field_constructor.end());

                return a->getCbor();

            };break;
            case tipo_t::tipo_int:{
                bool n_positivo = true;
                std::uint64_t n_entero = obtener_int_str(it, it_end, n_positivo);
                if(n_entero == 0){
                    n_positivo = true;
                }
                if(n_positivo){
                    a->addUint(n_entero);
                }
                else{
                    a->addNint_withoutzero(n_entero);
                }

                return a->getCbor();;

            };break;
            case tipo_t::tipo_bytes:{
                std::vector<std::uint8_t> bytes_;
                if(obtener_bytes_str(it, it_end, bytes_)){
                    a->addBytesArray(bytes_);
                    return a->getCbor();
                }
            };break;
            case tipo_t::tipo_map:{

                std::size_t pos_corchete_inicio = pos_primer_caracter_it('[', it, it_end); // se delimita el espacio de trabajo de map
                std::size_t pos_corchete_fin = 0;
                std::size_t pos_coma = 0;
                std::vector<std::uint8_t> keyvalue_cbor;
                std::vector<std::uint8_t> key_cbor;
                std::vector<std::uint8_t> value_cbor;
                std::uint64_t numero_elementos_mapa = 0;

                if(pos_corchete_inicio != std::string::npos ){
                    it += pos_corchete_inicio + 1; //agrega una unidad para avanzar desde el [

                    while(pos_coma != std::string::npos && it < it_end){

                        if(obtener_key_value_map(it, it_end, key_cbor, value_cbor)){
                            numero_elementos_mapa++;
                            keyvalue_cbor.insert(keyvalue_cbor.end(),key_cbor.begin(),key_cbor.end());
                            keyvalue_cbor.insert(keyvalue_cbor.end(),value_cbor.begin(),value_cbor.end());
                            key_cbor.clear();
                            value_cbor.clear();
                        }else{
                            throw std::invalid_argument("error in \"map\": could not determine the value of a variable");
                        }

                        pos_coma = pos_primer_caracter_it(',',it,it_end);
                        if(pos_coma != std::string::npos){
                            it += pos_coma + 1; // se avanza la posicion de la coma
                        }

                    }

                    pos_corchete_fin = pos_primer_caracter_it(']', it, it_end);  // realiza la busqueda del ] del mapa
                    if(pos_corchete_fin != std::string::npos){
                        it += pos_corchete_fin + 1;
                        pos_corchete_fin = pos_primer_caracter_it('}',it, it_end); // Se sale del esquema map y se crea el mapa en cbor
                        if(pos_corchete_fin != std::string::npos){
                            it += pos_corchete_fin + 1;
                            a->createMap(numero_elementos_mapa);
                            a->bypassIteratorVectorCbor(keyvalue_cbor.begin(),keyvalue_cbor.end());
                            return a->getCbor();;

                        }else{
                            throw std::invalid_argument("error in \"map\", missing ( } ) at end");
                        }
                    }else{
                        throw std::invalid_argument("error in \"map\": missing a ( ] )");
                    }



                }else{
                    throw std::invalid_argument("error in \"map\": missing a ( [ )");
                }

            };break;
            case tipo_t::tipo_list:{ /// poner el contenido de list en una funcion para que se pueda reutilizar en field, ver lo mismo con map

                //if(!obtener_list_cbor(it, it_end, cbor_data)){  // se pasa el vector cbor_data para que se serialize dentro de obtener_list_cbor(), pero tambien esta ligado al elemento CborSerialize de esta funcion
                //    return false;                               // pero en este "case" no se llama, asi que no genera sobre escrituras de datos
                //}
                std::vector<std::uint8_t> const &cbor_data = obtener_list_cbor(it, it_end);
                std::size_t pos_llave_fin = pos_primer_caracter_it('}',it, it_end); // Se sale del esquema list y se crea el mapa en cbor
                if(pos_llave_fin != std::string::npos){
                    it += pos_llave_fin + 1;
                    return cbor_data;

                }else{
                    throw std::invalid_argument("error in \"list\", missing ( } ) at end");
                }


            };break;
            default:{
                throw std::invalid_argument("error could not find the data type");
            };break;
            }

        }else{
            std::string ex_error =" error, ( : ) is expected at the end of  ";
            switch (elemento){
            case tipo_t::tipo_int :{ex_error.append("\"int\"");};break;
            case tipo_t::tipo_bytes :{ex_error.append("\"bytes\"");};break;
            case tipo_t::tipo_map :{ex_error.append("\"map\"");};break;
            case tipo_t::tipo_list :{ex_error.append("\"list\"");};break;
            case tipo_t::tipo_constructor :{ex_error.append("\"constructor\"");};break;
            case tipo_t::tipo_constructor_field :{ex_error.append("\"fields\"");};break;
            }
            throw std::invalid_argument(ex_error);
        }

    }
    return a->getCbor();
}

PlutusJsonSchema::tipo_t PlutusJsonSchema::detectar_tipo(std::string::iterator &it, std::string::const_iterator const &it_end){ //PROBADO
    std::uint8_t bit_caracter = 0;
    while(it != it_end){
        switch(*it){
        case ' ':{};break; // en caso de espacios no hace nada
        case '"':{  // si no detecta previamente { , lanza error , no cuenta para el constructor
            if(bit_caracter == 1 ){
                bit_caracter |= 0x02;
            }else{
                if( *(it+1) == 'c' || *(it+1) == 'f' ){
                    bit_caracter = 3;
                }else{
                    throw std::invalid_argument("error, can't find element type: missing a ( { )");
                    //return tipo_t::tipo_error;
                }
            }
        };break;
        case '{':{ // si detecta algun otro caracter previo, lanza error
            if(bit_caracter == 0 ){
                bit_caracter |= 0x01;
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
                return tipo_t::tipo_error;
            }
        };break;
        case 'i':{ // en caso de detectar un int
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("int\"", it, it_end)){
                    return tipo_t::tipo_int;
                }else{
                    throw std::invalid_argument("error, expected a ( \"int\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        case 'b':{ // en caso de detectar un bytes
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("bytes\"", it, it_end)){
                    return tipo_t::tipo_bytes;
                }else{
                    throw std::invalid_argument("error, expected a ( \"bytes\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        case 'm':{ // en caso de detectar un map
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("map\"", it, it_end)){
                    return tipo_t::tipo_map;
                }else{
                    throw std::invalid_argument("error, expected a ( \"map\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        case 'l':{ // en caso de detectar un list
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("list\"", it, it_end)){
                    return tipo_t::tipo_list;
                }else{
                    throw std::invalid_argument("error, expected a ( \"list\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        case 'c':{ // en caso de detectar un constructor
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("constructor\"", it, it_end)){
                    return tipo_t::tipo_constructor;
                }else{
                    throw std::invalid_argument("error, expected a ( \"constructor\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        case 'f':{ // en caso de detectar un fileds
            if(bit_caracter == 3){
                if(es_igual_ydesplazaIt("fields\"", it, it_end)){
                    return tipo_t::tipo_constructor_field;
                }else{
                    throw std::invalid_argument("error, expected a ( \"fields\" ) ");
                }
            }else{
                throw std::invalid_argument("error, can't find element type: missing a ( \" ) ");
            }
        };break;
        default:{ /** genera error **/
            std::string ex_error = "error, can't find element type: invalid argument  ( ";
            ex_error.append(1, static_cast<char>(*it));
            ex_error.append(" ) ; expect ( \" ) , ( { ) or the name of some element, eg: \"int\" ");
            throw std::invalid_argument(ex_error);
        };break; // para el resto genera un error
        }
        if(it != it_end){ // es un seguro para evitar el desbordamiento, en caso de que it halla llegado al final de su cadena, debido al uso de la funcion es_igual_ydesplazaIt()
            it++;
        }
    }

    return tipo_t::tipo_error;
};

void PlutusJsonSchema::addSchemaJson(std::string json){


    std::string::iterator cit = json.begin(); // crea un puntero al primer elemento del string
    std::string::const_iterator cit_end = json.cend(); // crea un puntero al primer elemento del string


    try{
        cborschema = obtener_tipo(cit, cit_end);
    }
    catch(std::logic_error &erl){
        cborschema.clear();
        std::cerr<<"Plutus Json Schema: "<<erl.what()<< std::endl;

    }
};

std::vector<std::uint8_t> const &PlutusJsonSchema::getCborSchemaJson() const{

    return cborschema;

};

std::uint8_t const *const PlutusJsonSchema::getHash32CborSchemaJson(){

    crypto_generichash_blake2b(datum_hash, 32, cborschema.data() ,cborschema.size(), nullptr, 0);

    return datum_hash;
};

        }
        }
