#include "../bip39.hpp"
#include <string>
#include <iostream>
#include <iomanip>
int main(int argc, char *argv[]){
    
    std::string mnemotic="blanket pistol rough habit hockey virus protect page bread buffalo duty canoe six clarify scare arrive december device page silly attend defense elder icon";
    
    std::uint8_t entropy_length_bytes=0;
    std::uint8_t *entropy=mnemotic2entropy(mnemotic.c_str(),Diccionary::English, &entropy_length_bytes);
    if(entropy!=nullptr){
        std::cout<<"\nentropy: ";
        for(std::uint8_t a=0;a<entropy_length_bytes;a++){
            std::cout<<std::hex <<std::setfill('0')<<std::setw(2) <<static_cast<int>(entropy[a]);
        }
        std::cout<<std::endl;
        delete[] entropy;
    }
    else{
        std::cout<<"invalid mnemotic"<<std::endl;
    }
    
    return 0;
}
