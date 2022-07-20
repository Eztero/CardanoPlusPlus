#include "../bip39.hpp"
#include <string>
#include <iostream>
#include <iomanip>
int main(int argc, char *argv[]){
    
    if (sodium_init() < 0) {
        std::cout<<"panic! the library couldn't be initialized; it is not safe to use"<<std::endl;
        return 0;
    }
    
    std::string mnemotic = "blanket pistol rough habit hockey virus protect page bread buffalo duty canoe six clarify scare arrive december device page silly attend defense elder icon";
    std::size_t entropy_length_bytes = 0;
    std::size_t mnemotic_length = 0;
    
    ///----- Mnemotic to entropy -----
    std::uint8_t *entropy = mnemotic2entropy(mnemotic.c_str(),Diccionary::English, &entropy_length_bytes);
    if(entropy!=nullptr){
        std::cout<<"\nentropy: ";
        for(std::uint8_t a = 0; a < entropy_length_bytes; a++){
            std::cout<<std::hex <<std::setfill('0')<<std::setw(2) <<static_cast<int>(entropy[a]);
        }
        std::cout<<std::endl;
        
    }
    else{
        std::cout<<"invalid mnemotic"<<std::endl;
    }
    
    ///----- Entropy to mnemotic -----
    char *mnemotics = entropy2mnemotic(entropy, &entropy_length_bytes, Diccionary::English, &mnemotic_length);
    if(mnemotics!=nullptr){
       std::cout<<"Mnemotic: "<<mnemotics<<std::endl;
    }
    else{
        std::cout<<"invalid Entropy"<<std::endl;
    }
    
    ///----- Delete data before freeing memory -----
    sodium_memzero(entropy, entropy_length_bytes);
    sodium_memzero(mnemotics, mnemotic_length);
    
    free(entropy);
    free(mnemotics);
    
    return 0;
}
