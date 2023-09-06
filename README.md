# CardanoPlusPlus
![](https://img.shields.io/github/stars/Eztero/CardanoPlusPlus.svg)
![](https://img.shields.io/github/forks/Eztero/CardanoPlusPlus.svg)
![](https://img.shields.io/github/tag/Eztero/CardanoPlusPlus.svg)
![](https://img.shields.io/github/release/Eztero/CardanoPlusPlus.svg)

It is a library written in c++ using the c++11 standard, this library is compatible with the serialization system used to interact with the chain of blocks, among the functions it stands out

* Support multi account
* Send and receive ADAs and tokens
* Register and unregister the stake key
* Delegation and withdrawal of rewards
* Use of simple script
* Use of plutus V1 and V2 contracts

Minting at this time is not supported

## Getting Started

### Dependencies

The library uses libsodium for the cryptographic functions, it is cross-platform and can be found in various Linux distributions and Windows.

For debian and derivatives.
```
sudo apt install libsodium-dev
```

### Install

```
./configure
make
sudo make install
```

By default it is installed in the `/usr` directory, it can be changed using the `--prefix=< directory >` command with `./configure`

### Use

The header must be included
```
#include <cardanoplusplus.h>
```
To access the library functions you must use the namespace `Cardano`

Example:
```
#include <cardanoplusplus.h>
#include <iostream>
int main(){
    /// Mnemotic to entropy
    std::string word_seed = "draw maximum video ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... ..... animal predict scale truly snake";
    std::size_t entropy_len = 0;
    std::uint8_t *entropy = Cardano::mnemotic2entropy(word_seed.data(), Cardano::Diccionary::English, &entropy_len);
    word_seed.clear();

    /// Get RawMasterKey
    std::uint8_t masterkey[MASTERSECRETKEY_LENGTH];
    bool status = Cardano::getRawMasterKey(entropy, entropy_len, nullptr, 0 , masterkey);
    if(!status){
      std::cout<<"An error occurred while generating the master key "<<std::endl;
    }
    sodium_memzero(entropy, entropy_len);
    delete[] entropy;

    /// Get Address
    std::uint32_t account_index = 0;
    std::uint32_t address_index = 0;
    std::string address_base;

    try{

    Cardano::getBech32Address(masterkey, Cardano::InputKey::MasterKey, Cardano::Network::Testnet, Cardano::Wallet::HD,
                              Cardano::Address::Base, account_index, address_index, address_base);
                              
    }catch(std::invalid_argument const &e){
      std::cout << e.what();
    }

    std::cout<<"address_base: "<<address_base<<std::endl;

  return 0;
}
```
