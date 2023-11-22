/**
MIT License

Copyright (c) 2022 Eztero

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#ifndef CARDANOPPENUM_HPP
#define CARDANOPPENUM_HPP

#include <cstdint>



namespace Cardano{

enum class Wallet : std::uint8_t{
HD,
MultiSignHD
};

enum class Role : std::uint8_t{
Extern,
Intern,
Staking,
OnlyAccount
};

enum class OutputKey : std::uint8_t{
Private,
Public
};

enum class InputKey : std::uint8_t{
MasterKey,  // is a Extended MasterKey
AccountKey_xvk,
AccountKey_xsk
};


enum class Network : std::uint8_t{
Testnet,
Mainnet
};

enum class Address : std::uint8_t{
Base_Extern,  // normal
Base_Intern,  //change
Enterprise_Extern, //normal
Enterprise_Intern, //change
Stake
};

enum class ScriptType{
        Native_Script = 0,
        Plutus_Script_V1,
        Plutus_Script_V2,
        None,
    };

enum class ScriptAddress : std::uint8_t{
Payment,
Stake
};

enum class ScriptReference : std::uint8_t{
Spending,
Certificate,
Withdrawal
};

enum class Credential{
RawAddressKeyHash = 0,
RawScriptHash
};

}


#endif
