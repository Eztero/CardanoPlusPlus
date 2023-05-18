/**
MIT License

Copyright (c) 2023 Eztero

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

#ifndef PLUTUSJSONSCHEMA
#define PLUTUSJSONSCHEMA

#include <stdexcept>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include "cbor_lite.hpp"
#include "txutils.hpp"

class PlutusJsonSchema{
public:

    explicit PlutusJsonSchema();
    virtual ~PlutusJsonSchema();
    void addSchemaJson(std::string json);
    std::vector<std::uint8_t> getCborSchemaJson() const;


private:

    enum class tipo_t{
        tipo_int,
        tipo_bytes,
        tipo_map,
        tipo_list,
        tipo_constructor,
        tipo_constructor_field,
        tipo_error
    };

    std::vector<std::uint8_t> cborschema;
    std::size_t find_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end);
    std::size_t pos_primer_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end );
    std::size_t pos_ultimo_caracter_it(char const caracter, std::string::const_iterator it, std::string::const_iterator it_end );
    std::size_t posfinal_primer_string_it(std::string frase, std::string::const_iterator it, std::string::const_iterator it_end );
    std::uint64_t obtener_int_str(std::string::iterator &it, std::string::const_iterator const &it_end, bool &npositivo);
    std::uint64_t obtener_int_constructor_str(std::string::iterator &it, std::string::const_iterator const &it_end);
    bool es_igual_ydesplazaIt(std::string const frase, std::string::iterator &it, std::string::const_iterator const &it_end);
    bool obtener_bytes_str(std::string::iterator &it, std::string::const_iterator const &it_end, std::vector<std::uint8_t> &bytes_vector);
    bool obtener_key_value_map(std::string::iterator &it, std::string::const_iterator &it_end, std::vector<std::uint8_t> &key_cbor, std::vector<std::uint8_t> &value_cbor );
    bool obtener_list_cbor(std::string::iterator &it, std::string::const_iterator &it_end, std::vector<std::uint8_t> &list_cbor);
    bool obtener_tipo(std::string::iterator &it, std::string::const_iterator &it_end, std::vector<std::uint8_t> &cbor_data );
    tipo_t detectar_tipo(std::string::iterator &it, std::string::const_iterator const &it_end);


};


#endif
