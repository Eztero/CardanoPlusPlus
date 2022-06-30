#include "bip39.hpp"

static std::uint16_t *word2bits(char const *const mnemotic, char const diccionary[][2048], std::uint8_t *const word_bin_length_array) //suport at 255 word, limited to 24 word (128 - 256 bits)
{
    if(mnemotic == nullptr){  // condicion de seguridad
        return nullptr;
    }
    std::uint16_t inicio = 0;
    std::uint16_t fin = 0;
    std::uint8_t word_amount = 0;
    std::uint16_t mnemotic_length = 0;
    bool while_true = true;
    while(while_true)   //una condicion bool para que haga la deteccion de la ultima letra, cuenta la cantidad de palabras
    {
        if(mnemotic[mnemotic_length] == 0x20 || mnemotic[mnemotic_length] == '\0' ) //si encuentra un espacio lo registra
        {
            if(mnemotic[mnemotic_length] == '\0') //si llega al final de la cadena se sale del while
            {
                while_true = false;
            }
            inicio = fin;
            fin = mnemotic_length;
            if((fin - inicio) > 2) //comprueba si el espacio es mayor a 2 para considerarlo una palabra
            {
                word_amount++;
            }
        }
        mnemotic_length += 1;
    }
    if(word_amount < 12 || word_amount > 24 || word_amount%3 != 0){ // condicion de seguridad, si las palabras no estan en el rango
        return nullptr;
    }

    //std::uint16_t *word2bits_out = new (std::nothrow) std::uint16_t[word_amount](); // #include <new>, alternativa con new, cambiar free() por delete[]
    std::uint16_t *word2bits_out = static_cast<std::uint16_t*>(std::calloc(word_amount,sizeof(std::uint16_t))); ///se asigna el espacio de memoria requerido para almacenar los bits
    word_amount = 0;

    if(word2bits_out != nullptr){ //sigue solo si se puede asignar memoria
        while_true = true;
        mnemotic_length = 0;
        char buffer_word[10];
        bool word_is_found = true;
        fin = 0;

        while(while_true)   //se vuelve a realizar el procedimiento anterior pero comparando las palabras con el diccionario
        {
            word_is_found = false;
            if(mnemotic[mnemotic_length] == 0x20 || mnemotic[mnemotic_length] == '\0' ) //si encuentra un espacio lo registra
            {
                if(mnemotic[mnemotic_length] == '\0') //si llega al final de la cadena se sale del while
                {
                    while_true = false;
                }
                inicio = fin;
                fin = mnemotic_length;
                if((fin - inicio) > 2) //comprueba si el espacio es mayor a 2 para considerarlo una palabra
                {
                    if(inicio != 0){
                        inicio = inicio + 1; //se inicia de la posicion de la palabra
                    }
                    if((fin - inicio) < 9){ //para que no se genere un desbordamiento en el buffer buffer_word
                        std::strncpy(buffer_word, &mnemotic[inicio], (fin - inicio)); //se copia la palabra al buffer_word
                        buffer_word[fin-inicio] = '\0'; //se agrega el caracter null al final, para que cumpla con las caracteristicas de un string
                        for(std::uint16_t bits = 0; bits < 2047; bits++) //se busca el nombre en el diccionario dado
                        {
                            if(std::strcmp(buffer_word, diccionary[bits]) == 0){
                                word2bits_out[word_amount] = bits;
                                word_amount += 1;
                                word_is_found = true;
                                break;
                            }
                        }
                    }
                    if(word_is_found == false ){ //en caso de que no encuentre la palabra en el diccionario;
                        sodium_memzero(word2bits_out, word_amount);/// Pone a Cero la Memoria antes de liberarla
                        std::free(word2bits_out);
                        word2bits_out = nullptr;
                        word_amount = 0;
                        while_true = false;
                    }
                }
            }
            mnemotic_length += 1;
        }
        sodium_memzero(buffer_word, 10);/// Pone a Cero la Memoria antes de liberarla
    }

    *word_bin_length_array = word_amount;
    return word2bits_out;
};


std::uint8_t *mnemotic2entropy(char const *const mnemotic, char const diccionary[][2048], std::uint8_t *const entropy_length_bytes)
{
    std::uint8_t word_bin_length_array;
    std::uint8_t *entropy = nullptr;
    std::uint16_t *word_bin = word2bits(mnemotic, diccionary, &word_bin_length_array);

    if(entropy_length_bytes != nullptr && word_bin != nullptr){ // aqui se comienza a reconstruir la entropia con la numeracion las palabras
        *entropy_length_bytes = (word_bin_length_array * 44) / 33; //se pasan la cantidad de bytes que tiene la entropy
        //entropy = new (std::nothrow) std::uint8_t[*entropy_length_bytes](); // #include <new>, alternativa con new, cambiar free() por delete[]
        entropy = static_cast<std::uint8_t*>(std::calloc(*entropy_length_bytes, sizeof(std::uint8_t))); ///se asigan un espacio de memoria a entropy
        if(entropy != nullptr){ //sigue solo si se puede asignar memoria
            std::uint8_t cbe = 8; //conteo bit entropia
            std::uint8_t cbw = 11; //conteo bit word_bin
            std::uint8_t entropy_count = 0;
            for(std::uint8_t i=0; i < word_bin_length_array; i++){
                cbw = 11;
                if(entropy_count < *entropy_length_bytes){ //para evitar desbordamiento de memoria en entropy[]
                    while(cbw != 0){
                        if(cbe <= cbw){
                            cbw -= cbe; //se resta de cbw la cantidad de bits a tomar para completar entropy[]
                            entropy[entropy_count] |= static_cast<std::uint8_t>(word_bin[i] >> cbw) & (0xff >> (8 - cbe));
                            cbe = 8; //como todos los bits en entropy[] son completados, se vuleve a indicar que tiene 8 bit disponibles a llenar
                            entropy_count++; //se pasa al siguente espacio en el array de entropy[]
                            if(entropy_count >= *entropy_length_bytes){ //se activa en caso de que entropy_count se salga de los limites de entropy[]
                                cbw = 0; //se pone a cero para que no entre en el while{}
                                break; //se sale del while
                            }
                        }
                        if(cbe > cbw){
                            cbe -= cbw; //se resta de cbe la cantidad de bits a tomar de word_bin[] , para completar entropy[]
                            entropy[entropy_count] |= static_cast<std::uint8_t>(word_bin[i] << cbe);
                            cbw = 0; //como todos los bits de word_bin[] entra en entropy[] entonces este queda en cero
                        }
                    }
                }
            }
            // Aqui se comprueba la validez de la entropia
            std::uint8_t cs_length = static_cast<std::uint8_t>((*entropy_length_bytes * 8) / 32);
            std::uint8_t checksumsha256[crypto_hash_sha256_BYTES]; //almacena el hash de la entropia
            crypto_hash_sha256(checksumsha256, entropy, *entropy_length_bytes); ///se obtiene el hash sha256 con la libreria libsodium, iniciar antes con sodium_init()
            std::uint8_t cs_bits = checksumsha256[0] >> (8 - cs_length); // Se extraen los cs_bits del hash
            std::uint8_t word_bin_cs_bits = static_cast<std::uint8_t>(word_bin[word_bin_length_array - 1]) & (0xff >> (8 - cs_length)); // se extaen los cs_bits de word_bin
            sodium_memzero(checksumsha256, crypto_hash_sha256_BYTES); /// Pone a Cero la Memoria

            if(word_bin_cs_bits != cs_bits){ //Se comparan los cs_bits de ambos para ver si la entropia es replicable (valida)
                sodium_memzero(entropy, *entropy_length_bytes); /// Pone a Cero la Memoria
                free(entropy);
                entropy = nullptr;
            }
        }
        sodium_memzero(word_bin, word_bin_length_array);/// Pone a Cero la Memoria antes de liberarla
        std::free(word_bin);
    }
    return entropy;
}


char *entropy2mnemotic(std::uint8_t const *const entropy, std::uint8_t const *const entropy_length_bytes, char const diccionary[][2048], std::uint8_t *const mnemotic_length)
{
    std::uint16_t entropy_length_bits = 0;
    std::uint8_t checksumsha256[crypto_hash_sha256_BYTES]; //almacena el hash de la entropia
    std::uint8_t cs_length = 0; //indica la cantidad de bits a tomar del hash
    std::uint8_t cs_bits = 0;// aca se guardan los bits tomados del hash (maximo 8 bits)
    if(entropy_length_bytes != nullptr){
        entropy_length_bits = *entropy_length_bytes * 8; // Se convierte de bytes a bits la longitud de la entropia (rango 128 - 256 bits)
    }
    if(entropy != nullptr && entropy_length_bits >= 128 && entropy_length_bits <= 256){  //comprueba que el largo en bits tiene que estar en el rango 128 - 256
        crypto_hash_sha256(checksumsha256, entropy, *entropy_length_bytes); ///se obtiene el hash sha256 con la libreria libsodium, iniciar antes con sodium_init()
        cs_length = static_cast<std::uint8_t>(entropy_length_bits / 32);
        cs_bits = checksumsha256[0] >> (8 - cs_length);
        sodium_memzero(checksumsha256, crypto_hash_sha256_BYTES); /// Pone a Cero la Memoria
    }
    else{
        return nullptr;
    }

    std::uint8_t ms_length = static_cast<std::uint8_t>((entropy_length_bits + static_cast<std::uint16_t>(cs_length)) / static_cast<std::uint16_t>(11)); // indica la cantida de palabras a generar
    //std::uint16_t *ms = new (std::nothrow) std::uint16_t[ms_length](); // #include <new>, alternativa con new, cambiar free() por delete[]
    std::uint16_t *ms = static_cast<std::uint16_t*>(std::calloc(ms_length, sizeof(std::uint16_t))); /// crea un array dinamico rellenado con ceros, para almacenar la numeracion de las palabras
    if(ms == nullptr){ ///si no puede reservar memoria
        return nullptr;
    }
    std::int8_t bt = 0; //Bit tomados
    std::uint8_t pbi_e = 8; // Bits disponible entropia
    std::uint8_t pbe = 0; // Posicion Bytes entropia

    for(std::uint8_t i = 0; i <= ms_length; i++){ //recorre todas las palabras
        bt = 11;  // la numeracion de cada palabra esta compuesta por 11 bits, esta variable se reinicia a 11 al pasar a la siguente palabra
        while(bt > 0){
            if(bt >= pbi_e){  // Si bt es mayor o igual a los bit existentes a tomar de la entropia, se desplaza bits a la izquierda
                bt -= pbi_e; // se restan a bt los bits a tomar del byte de la entropia
                ms[i] |= (static_cast<std::uint16_t>(entropy[pbe]) << bt) & 0x7ff;
                pbi_e = 8; // se da por hecho que se tomaron todos los bits de el byte y se reinician los bits disponibles
                pbe++; // se pasa al proximo byte de la entropia
                if(pbe >= *entropy_length_bytes){ // si se excede el limite del array de la entropia se adicionan los bits de cs_bits
                    ms[i] |= static_cast<std::uint16_t>(cs_bits); // Adiciona los bits del checksum de la entropia (cs_bits)
                    bt = 0;
                    i++; //Se suma +1 a i para salir del bucle for()
                }
            }
            else{ // Si bt es menor a los bit existentes a tomar de la entropia, se desplaza bits a la derecha
                pbi_e -= bt; //se restan a pbi_e los bits a tomar del byte de la entropia
                ms[i] |= static_cast<std::uint16_t>(entropy[pbe]) >> pbi_e;
                bt = 0;
            }
        }
    }
    uint8_t words_length = (ms_length * 9);
    if(mnemotic_length != nullptr){
        *mnemotic_length = words_length;
    }
    //std::uint16_t *words = new (std::nothrow) std::uint16_t[words_length](); // #include <new>, alternativa con new, cambiar free() por delete[]
    char *words = static_cast<char*>(std::calloc(words_length, sizeof(char))); ///Crea un array dinamico iniciado en ceros para almacenar las paladras
    if(words == nullptr){ ///si no puede reservar memoria
        sodium_memzero(ms, ms_length); /// Pone a Cero la Memoria antes de liberarla
        std::free(ms);
        return nullptr;
    }
    for(std::uint8_t i = 0; i< ms_length; i++){
        std::strcat(words, diccionary[ms[i]]);// concatena las palabras del diccionario en la variable word
        std::strcat(words, " ");
    }
    sodium_memzero(ms, ms_length); /// Pone a Cero la Memoria antes de liberarla
    std::free(ms);
    return words;
}
