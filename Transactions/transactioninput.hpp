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

Documentation:
https://github.com/input-output-hk/cardano-ledger/blob/master/eras/babbage/test-suite/cddl-files/babbage.cddl
**/


//0 : set<transaction_input> --> transaction_input = [ transaction_id : $hash32, index : uint]

#ifndef TRANSACTIONINPUT_HPP
#define TRANSACTIONINPUT_HPP
#include <memory>
#include <cstdint>
#include <vector>
#include <string>
#include "../Utils/txutils.hpp"
#include "../Utils/plutusjsonschema.hpp"
#include "../Utils/cbor_lite.hpp"

class TransactionsInputs {
public:
    explicit TransactionsInputs();

    enum class ScriptType{
        None = 0,
        Native_Script,
        Plutus_Script_V1,
        Plutus_Script_V2,

    };
    enum class RTag{
        Spend = 0,
        Mint,
        Cert,
        Reward
    };
    /// Agregar un bloqueo para que solo acepte un datum y un redeemer por direccion , ademas incorporar una excepcion que avise cuando se agrega un datum o reedeemer antes de ingresar
    /// la primera direccion
    TransactionsInputs & addInput( std::string const & TxHash, std::uint64_t const TxIx );  // -> addScript() || addReferenceInput -> addDatum() -> addRedeemer()
    TransactionsInputs & addReferenceInput( std::string const & TxHash, std::uint64_t const TxIx ); // <- con el setReferenceStriptType se determina el tipo de input y se vera si es necesario el datum y redeemer
    TransactionsInputs & addCollateralInput( std::string const & TxHash, std::uint64_t const TxIx );
    TransactionsInputs & setGlobalReferencesStriptsType( TransactionsInputs::ScriptType const script_type); // necesario para el calculo del scriptdatahash , se debe especificar por obligacion
    TransactionsInputs & addDatum( std::string & json_datum );  // Se usa para el witness y el scriptdatahash
    TransactionsInputs & addRedeemer( TransactionsInputs::RTag const r_tag, std::uint64_t r_index,  std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits );  // Se usa para el witness y el scriptdatahash
    TransactionsInputs & addScript( TransactionsInputs::ScriptType const script_type, std::uint8_t const * const & script, std::size_t & script_len ); // Se usa para el witness y el scriptdatahash (solo el script_type)
    TransactionsInputs & addScript( TransactionsInputs::ScriptType const script_type, std::string const & script );
    std::uint32_t const & getBodyMapcountbit() const;
    std::uint16_t const & getWitnessMapcountbit() const;

    void alphanumeric_organization();  // ordena los index  de los utxos, redeemer y datum de manera alfanumerica
    std::uint16_t const & getInputCount() const;
    std::uint16_t const & getReferenceInputCount() const;
    std::uint16_t const & getCollateralInputCount() const;
    std::uint16_t const & getDatumInputCount() const;
    std::uint16_t const & getRedeemerInputCount() const;
    std::uint16_t const & getPlutusV1InputCount() const;
    std::uint16_t const & getPlutusV2InputCount() const;
    std::uint16_t const & getNativeScriptInputCount() const;


    std::uint8_t const getReferencesStriptsType() const;

    std::vector<std::uint8_t> const & getInputs() const;
    std::vector<std::uint8_t> const & getReferenceInputs() const;
    std::vector<std::uint8_t> const & getCollateralInputs() const;
    std::vector<std::uint8_t> const & getDatums() const;
    std::vector<std::uint8_t> const & getRedeemers() const;
    std::vector<std::uint8_t> const & getPlutusV1Scripts() const;
    std::vector<std::uint8_t> const & getgetPlutusV2Scripts() const;
    std::vector<std::uint8_t> const & getNativeScripts() const;

private:
    bool addUtxoInput( std::uint8_t const t_selector, std::uint8_t const * const & TxHash, std::uint64_t const & TxIx );
    ScriptType globalreferencescript;
    std::uint16_t tx_input_count;              //maximo 65534
    std::uint16_t reference_input_count;       //maximo 65534
    std::uint16_t collateral_input_count;      //maximo 65534
    std::uint16_t datum_input_count;           //maximo 65534
    std::uint16_t redeemer_input_count;        //maximo 65534
    std::uint16_t plutusscript1_input_count;   //maximo 65534
    std::uint16_t plutusscript2_input_count;   //maximo 65534
    std::uint16_t nativescript_input_count;    //maximo 65534
    std::vector<std::uint8_t> tx_input{};
    std::vector<std::uint8_t> reference_input{};
    std::vector<std::uint8_t> collateral_input{};
    std::vector<std::uint8_t> datum_input{};
    std::vector<std::uint8_t> redeemer_input{};
    std::vector<std::uint8_t> plutusscript1_input{};
    std::vector<std::uint8_t> plutusscript2_input{};
    std::vector<std::uint8_t> nativescript_input{};
    std::size_t buff_sizet;
    std::uint32_t bodymap_countbit;      ///  0x0001 , Tiene que iniciar con cero
    std::uint16_t witnessmap_countbit;      ///  0x0001 , Tiene que iniciar con cero
};



#endif