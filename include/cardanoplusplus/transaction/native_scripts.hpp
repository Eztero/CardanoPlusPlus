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

Documentation:
https://github.com/input-output-hk/cardano-ledger/blob/master/eras/babbage/test-suite/cddl-files/babbage.cddl
**/

#include <cstdint>
#include <vector>
#include <string>

#include "../utils/cbor_lite.hpp"
#include "../utils/cmacros.hpp"
namespace Cardano{
class NativeScripts{
public:
    explicit NativeScripts();
    NativeScripts & SignatureOf(std::uint8_t const *const vk_hash);                // Argumento
    NativeScripts & SignatureOf(std::string const payment_address);                // Argumento
    NativeScripts & requireAllOf();                                                // Condicionales
    NativeScripts & requireAnyOf();                                                // Condicionales
    NativeScripts & requireAtLeastNOf(std::uint64_t const n);                      // Condicionales
    NativeScripts & Endrequire();
    NativeScripts & thisIsValidBeforeSlot(std::uint64_t const slot); //antes del tiempo  // Condicionales
    NativeScripts & thisIsValidAfterSlot(std::uint64_t const slot); //despues del tiempo // Condicionales
    std::vector<std::uint8_t> const & getCborNativeScripts();
    std::string const & getJsonNativeScripts();

private:
    std::uint8_t eoc; //espacio ocupado en capsula
    std::uint8_t pec; // posicion en capsula
    std::uint8_t bufferbech32[BECH32_MAX_LENGTH]{};
    std::vector< std::vector<std::uint8_t> > capsula; // guarda el contenido de las firmas y tiempo
    std::vector<std::uint8_t> condiciones{}; //Guarda las condicionantes como All, Any, N de K. Endrequire
    std::vector<std::uint8_t> ppec{}; // posicion previa en capsula
    Utils::CborSerialize cbor;
    //std::vector<std::uint8_t> cbornativescript;  // recibe los datos serializados en Cbor
    //std::vector<std::uint8_t> vector_buffer;
};
}
