#include "../../include/cardanoplusplus/transaction/withdrawal.hpp"
#include "../../include/cardanoplusplus/utils/txutils.hpp"
#include "../../include/cardanoplusplus/utils/plutusjsonschema.hpp"
#include "../../include/cardanoplusplus/hash/bech32.hpp"

/// Vector<std::uint8_t> redeemer_withdrawals (redeeemer) :
/// +2 (index_redemer)  // index_redemer = index (withdrawals_count), indica la posicion del raw_address en withdrawals despues ser ordenado
/// +1 (tag)
/// +8 (largo_plutus_data)
/// +n (plutus_data)
/// +8 (largo_ex_units)
/// +n (ex_units) (previamente serializado en cbor)
/// 19+n bytes maximo de largo minimo

/// Vector<std::uint8_t> withdrawals (withdrawals) :
/// + 2 (index)
/// + 29 (raw stake address)
/// + 8 (amount)
/// + 39 bytes de largo minimo

namespace Cardano{

Withdrawals::Withdrawals(){

    withdrawals_count = 0; //maximo 65534
    redeemer_withdrawals_count = 0;
    bodymapcountbit = 0;
    witnessmap_countbit = 0;
    buff_sizet = 0;

}

Withdrawals::~Withdrawals(){

}

Withdrawals & Withdrawals::addWithdrawals(std::uint8_t const * const raw_stake_address, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 2(index) + 29 (raw stake address) + 8 (amount) = 39
    if(withdrawals_count < UINT16_MAX && !Utils::existen_coincidencias(raw_stake_address, withdrawals.data(), 29, withdrawals_count, 41) ){ // Comprueba de que no se repitan las direcciones, si hay coincidencia se omite la direccion

        buff_sizet = static_cast<std::size_t>( withdrawals.capacity() ) - static_cast<std::size_t>( withdrawals.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 39){
            withdrawals.reserve(withdrawals.size() + 39);
        }
        Utils::addUint16toVector(withdrawals, withdrawals_count);  // Index
        withdrawals.insert(withdrawals.end(), raw_stake_address, raw_stake_address + 29);
        Utils::addUint64toVector(withdrawals, amount);

        bodymapcountbit |= 0x0020;
        ++withdrawals_count;

    }

    /// SINO LANZAR ERROR

    return *this;
}

Withdrawals & Withdrawals::addWithdrawals(std::string & stake_address, std::uint64_t const amount){ // ? 5 : withdrawals
    /// 2 (index) + 29 (raw stake address)  + 8 (amount) = 39
    std::uint16_t buffbech32_len = 0;
    if(Hash::bech32_decode(stake_address.c_str(), buffbech32, &buffbech32_len)){
        if(buffbech32_len == 29){
            addWithdrawals(buffbech32, amount);
        }
        /// SINO LANZAR ERROR
    }

    return *this;
}

void Withdrawals::addRedeemer( std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits ){
    std::unique_ptr<Utils::CborSerialize> rcbor(new Utils::CborSerialize);
    std::unique_ptr<Utils::CborSerialize> unitscbor(new Utils::CborSerialize);
    std::unique_ptr<Utils::PlutusJsonSchema> Json_p(new Utils::PlutusJsonSchema);

    unitscbor->createArray(2);
    unitscbor->addUint(memoryunits); // mem
    unitscbor->addUint(cpusteps);    // step

    Json_p->addSchemaJson(json_redeemer);

    std::vector<std::uint8_t> const & cbor_units = unitscbor->getCbor();
    std::vector<std::uint8_t> const & cbor_plutusdata = Json_p->getCborSchemaJson();

    std::uint16_t numero_wdrls = withdrawals_count - 1;
    if(numero_wdrls < 0){
      throw std::invalid_argument("Error in addRedeemer: no previous Withdrawals found");
    }

    Utils::addUint16toVector( redeemer_withdrawals, numero_wdrls );
    redeemer_withdrawals.push_back(static_cast<std::uint8_t>(3));                                          // tag = 3
    Utils::addUint64toVector(redeemer_withdrawals,cbor_plutusdata.size());                                 // plutusdata_len
    redeemer_withdrawals.insert(redeemer_withdrawals.end(),cbor_plutusdata.begin(),cbor_plutusdata.end()); // plutusdata
    Utils::addUint64toVector(redeemer_withdrawals,cbor_units.size());                                      // cbor_ex_units_len
    redeemer_withdrawals.insert(redeemer_withdrawals.end(),cbor_units.begin(),cbor_units.end());           // cbor_ex_units

    ++redeemer_withdrawals_count;
    witnessmap_countbit |= 0x20;
}

void Withdrawals::alphanumeric_organization(){

    std::unique_ptr<std::vector<std::uint8_t>> vector_data(new std::vector<std::uint8_t>{});
    std::uint8_t *ptr_data = nullptr;
    std::uint8_t *ptr_wdrl[withdrawals_count];
    std::uint8_t *ptr_redeemers[redeemer_withdrawals_count];
    std::uint16_t countmenosuno;
    int w_cmp = 0;


    if(withdrawals_count > 0){
        ptr_data = withdrawals.data();
        for(std::uint16_t i = 0; i < withdrawals_count; i++ ){ // asigno las localizaciones a punteros
            ptr_wdrl[i] = ptr_data;
            ptr_data += 39;
        }

        countmenosuno = withdrawals_count - 1;                // reordeno los punteros
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  e = 0; e < countmenosuno;e++ ){
                w_cmp = std::memcmp( ptr_wdrl[e]+2, ptr_wdrl[e+1]+2, 29);
                if(w_cmp > 0){
                    ptr_data = ptr_wdrl[e];
                    ptr_wdrl[e] = ptr_wdrl[e+1];
                    ptr_wdrl[e+1] = ptr_data;
                }
            }
        }
    }


    if(redeemer_withdrawals_count > 0){
        ptr_data = redeemer_withdrawals.data();
        for(std::uint16_t  i= 0; i<redeemer_withdrawals_count;i++ ){ // asigno las localizaciones a punteros
            ptr_redeemers[i] = ptr_data;
            ptr_data += Utils::extract8bytestoUint64(ptr_data + 3) + 11;
            ptr_data += Utils::extract8bytestoUint64(ptr_data) + 8;

        }
        // reasigna los index
        for(std::uint16_t  i = 0; i < redeemer_withdrawals_count; i++ ){
            for(std::uint16_t  e = 0; e < withdrawals_count;e++ ){
                if(ptr_redeemers[i][0] == ptr_wdrl[e][0] && ptr_redeemers[i][1] == ptr_wdrl[e][1]){
                    Utils::replaceUint16toVector(ptr_redeemers[i], e );
                    break;
                }
            }
        }
        // reordena los punteros
        countmenosuno = redeemer_withdrawals_count-1;
        for(std::uint16_t  i = 0; i < countmenosuno; i++ ){
            for(std::uint16_t  c = 0; c < countmenosuno; c++ ){
                if(Utils::extract2bytestoUint16(ptr_redeemers[c]) > Utils::extract2bytestoUint16(ptr_redeemers[c+1])){
                    ptr_data = ptr_redeemers[c];
                    ptr_redeemers[c] = ptr_redeemers[c+1];
                    ptr_redeemers[c+1] = ptr_data;
                    break;
                }
            }
        }
    }


    // Reemplaza los vectores actuales
    if(withdrawals_count > 0){
        vector_data->clear();
        vector_data->reserve( withdrawals_count * 39);
        for(std::uint16_t  i = 0; i < withdrawals_count; i++ ){
            vector_data->insert(vector_data->end(),ptr_wdrl[i] ,ptr_wdrl[i] + 39);
        }
        withdrawals.assign( vector_data->begin(), vector_data->end() );
    }


    if(redeemer_withdrawals_count > 0){
        vector_data->clear();
        for(std::uint16_t  i = 0; i < redeemer_withdrawals_count; i++ ){
            std::size_t ptr_redeeemers_len = Utils::extract8bytestoUint64(ptr_redeemers[i] + 3) + 11;
            ptr_redeeemers_len += Utils::extract8bytestoUint64(ptr_redeemers[i] + ptr_redeeemers_len) + 8;
            vector_data->insert(vector_data->end(),ptr_redeemers[i] ,ptr_redeemers[i] + ptr_redeeemers_len);
        }
        redeemer_withdrawals.assign(vector_data->begin(), vector_data->end());
    }

}

std::uint16_t const & Withdrawals::getWithdrawalRedeemersCount() const{
    return redeemer_withdrawals_count;
}

std::uint32_t const & Withdrawals::getBodyMapcountbit() const{
    return bodymapcountbit;
}

std::uint16_t const & Withdrawals::getWitnessMapcountbit() const{
    return witnessmap_countbit;
}

std::uint16_t const & Withdrawals::getWithdrawalsCount() const{
    return withdrawals_count;
}

std::vector<std::uint8_t> const & Withdrawals::getWithdrawals() const{
    return withdrawals;
}

std::vector<std::uint8_t> const & Withdrawals::getWithdrawalRedeemers() const{
    return redeemer_withdrawals;
}

}
