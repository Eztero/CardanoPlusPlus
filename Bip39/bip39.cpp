#include "bip39.hpp"

static std::uint16_t *word2bits(char const *const mnemotic,char const diccionary[][2048], std::uint8_t *const word_bin_length_array) //suport at 255 word
{
    std::uint16_t inicio=0;
    std::uint16_t fin=0;
    std::uint8_t word_amount=0;
    std::uint16_t mnemotic_length=0;
    bool while_true=true;
    while(while_true)   //una condicion bool para que haga la deteccion de la ultima letra y cuenta la cantidad de palabras
    {
        if(mnemotic[mnemotic_length]==0x20 || mnemotic[mnemotic_length]=='\0' ) //si encuentra un espacio lo registra
        {
            if(mnemotic[mnemotic_length]=='\0') //si llega al final de la cadena se sale del while
            {
                while_true=false;
            }
            inicio=fin;
            fin=mnemotic_length;
            if((fin-inicio) > static_cast<std::uint8_t>(2)) //comprueba si el espacio es mayor a 2 para considerarlo una palabra
            {
                word_amount++;
            }
        }
        mnemotic_length+=1;
    }

    std::uint16_t *word2bits_out = static_cast<std::uint16_t*>(std::calloc (word_amount,sizeof(std::uint16_t))); //se asigna el espacio de memoria requerido para almacenar los bits

    while_true=true;
    word_amount=0;
    mnemotic_length=0;
    char buffer_word[10];
    bool word_is_found=true;
    fin=0;

    while(while_true)   //se vuelve a realizar el procedimiento anterior pero comparando las palabras con el diccionario
    {
        word_is_found=false;
        if(mnemotic[mnemotic_length]==0x20 || mnemotic[mnemotic_length]=='\0' ) //si encuentra un espacio lo registra
        {
            if(mnemotic[mnemotic_length]=='\0') //si llega al final de la cadena se sale del while
            {
                while_true=false;
            }
            inicio=fin;
            fin=mnemotic_length;
            if((fin-inicio) > static_cast<std::uint8_t>(2)) //comprueba si el espacio es mayor a 2 para considerarlo una palabra
            {
                if(inicio!=0){
                    inicio=inicio+1; //se inicia de la posicion de la palabra
                }
                if(fin-inicio<9){ //para que no se genere un desbordamiento en el buffer buffer_word
                    std::strncpy (buffer_word, &mnemotic[inicio], fin-inicio); //se copia la palabra al buffer_word
                    buffer_word[fin-inicio]='\0'; //se agrega el caracter null manualmente
                    for(std::uint16_t bits=0; bits < (static_cast<std::uint16_t>(2047)); bits++) //se busca el nombre en el diccionario dado
                    {
                        if( std::strcmp(buffer_word, diccionary[bits]) ==0){
                            word2bits_out[word_amount]=bits;
                            word_amount+=1;
                            word_is_found=true;
                            break;
                        }
                    }
                }
                if(word_is_found==false){ //en caso de que no encuentre la palabra en el diccionario borra el puntero se le asigna una direccion null y se sale del while;
                    std::free(word2bits_out); ///FALTA BORRADO SEGURO AQUI
                    word2bits_out=nullptr;
                    word_amount=0;
                    while_true=false;
                }
            }
        }
        mnemotic_length+=1;
    }

    ///ASIGNAR BORRADO SEGURO A buffer_word
    *word_bin_length_array=word_amount;
    return word2bits_out;
};


std::uint8_t *mnemotic2entropy(char const *const mnemotic,char const diccionary[][2048], std::uint8_t *const entropy_length_bytes) {
    std::uint8_t word_bin_length_array;
    std::uint16_t *word_bin=word2bits(mnemotic,diccionary, &word_bin_length_array);
    std::uint8_t *entropy=nullptr;
    if(word_bin!=nullptr && word_bin_length_array%3==0 && entropy_length_bytes!=nullptr){
        *entropy_length_bytes=((word_bin_length_array)*(static_cast<std::uint8_t>(44)))/static_cast<std::uint8_t>(33); //se pasan la cantidad de bytes que tiene la entropy
        entropy=new std::uint8_t[*entropy_length_bytes];
        std::uint8_t cbe=8; //conteo bit entropia
        std::uint8_t entropy_count=0;
        for(std::uint8_t i=0; i<word_bin_length_array;i++){
            std::uint8_t cbw=11; //conteo bit word_bin
            if(entropy_count<*entropy_length_bytes){ //para evitar desbordamiento de memoria en entropy[]
            while(cbw!=0){
                if(cbe<=cbw){
                        cbw-=cbe; //se resta de cbw la cantidad de bits a tomar para completar entropy[]
                        entropy[entropy_count] = (static_cast<std::uint8_t>(word_bin[i] >> (cbw)) & (0xff >> (static_cast<std::uint8_t>(8)- cbe))) | entropy[entropy_count];
                        cbe=8; //como todos los bits en entropy[] son completados, se vuleve a indicar que tiene 8 bit disponibles a llenar
                        entropy_count++; //se pasa al siguente espacio en el array de entropy[]
                        if(entropy_count>=*entropy_length_bytes){ //se activa en caso de que entropy_count se salga de los limites de entropy[]
                            cbw=0; //se pone a cero para que no entre en el while{}
                            break; //se sale del while
                        }
                }
                if(cbe > cbw){
                        cbe-=cbw; //se resta de cbe la cantidad de bits a tomar de word_bin[] , para completar entropy[]
                        entropy[entropy_count] = static_cast<std::uint8_t>(word_bin[i] << (cbe)) | (entropy[entropy_count]);
                    cbw=0; //como todos los bits de word_bin[] entra en entropy[] entonces este queda en cero
                }
            }
            }
        }
    std::free (word_bin);  ///ASIGNAR UN BORRADO SEGURO AQUÍ
    }
    return entropy;
}
//-------In construction-----------------
//char *entropy2mnemotic(std::uint8_t const *const entropy,std::uint8_t const *const entropy_length_bytes,std::uint8_t *const mnemotic_length)
//{}
