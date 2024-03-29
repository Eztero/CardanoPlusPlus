#include "../../include/cardanoplusplus/transaction/certificates.hpp"
#include "../../include/cardanoplusplus/utils/txutils.hpp"
#include "../../include/cardanoplusplus/utils/plutusjsonschema.hpp"
#include "../../include/cardanoplusplus/hash/bech32.hpp"
namespace Cardano{
/// Vector<std::uint8_t> redeemer_cert (redeeemer) :
/// +2 (index_redemer)  // index_redemer = index (cbor_certificates_count), indica la posicion del hash en cbor_certificates despues ser ordenado
/// +1 (tag)
/// +8 (largo_plutus_data)
/// +n (plutus_data)
/// +8 (largo_ex_units)
/// +n (ex_units) (previamente serializado en cbor)
/// 19+n bytes maximo de largo minimo

Certificates::Certificates(){
    cbor_certificates_count = 0;
    redeemer_cert_count = 0;
    bodymap_countbit = 0;
    witnessmap_countbit = 0;
}

Certificates::~Certificates(){
}

void Certificates::addStakeRegistration(Cardano::Credential const ckey, std::uint8_t const *const stake_credential){
    // stake_credential_vk(28bytes)
    //crypto_generichash_blake2b(blake224, 28, stake_credential_vk, 32, nullptr, 0);
    cert_cbor.clearCbor();
    cert_cbor.createArray(2);                                          // [ , ]
    cert_cbor.addUint(static_cast<uint64_t>(0));                       // [0,   ]
    cert_cbor.createArray(2);                                          // [0,[ ] ]
    switch(ckey){
    case Credential::RawAddressKeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                       // [0,[0, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                             // [0,[0, addr_keyhash ] ]

    };break;
    case Credential::RawScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                       // [0,[1, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                             // [0,[1, scripthash ] ]
    };break;
    }


    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;

}

void Certificates::addStakeDeregistration(Cardano::Credential const ckey, std::uint8_t const *const stake_credential){
    // stake_credential_vk(28bytes)
    //crypto_generichash_blake2b(blake224, 28, stake_credential_vk, 32, nullptr, 0);
    cert_cbor.clearCbor();
    cert_cbor.createArray(2);                                                       // [ , ]
    cert_cbor.addUint(1);                                                           // [1,   ]
    cert_cbor.createArray(2);                                                       // [1,[ ] ]
    switch(ckey){
    case Credential::RawAddressKeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [1,[0, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                                  // [1,[0, addr_keyhash ] ]

    };break;
    case Credential::RawScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                                    // [1,[1, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                                  // [1,[1, scripthash ] ]
    };break;
    }

    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;
}

void Certificates::addStakeDelegation(Cardano::Credential const ckey, std::uint8_t const *const stake_credential, std::uint8_t const *const pool_keyhash){
    // stake_credential_vk(28bytes) + addr_poolkeyhash(28bytes) = 56
    //crypto_generichash_blake2b(blake224, 28, stake_credential_vk, 32, nullptr, 0);
    cert_cbor.clearCbor();
    cert_cbor.createArray(3);                                                       // [ , , ]
    cert_cbor.addUint(2);                                                           // [2, , ]
    cert_cbor.createArray(2);                                                       // [2,[ , ], ]
    switch(ckey){
    case Credential::RawAddressKeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [2,[0, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                                  // [2,[0, addr_keyhash ] ]

    };break;
    case Credential::RawScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                                    // [2,[1, ] ]
    cert_cbor.addBytesArray(stake_credential, 28);                                  // [2,[1, scripthash ] ]
    };break;
    }
    cert_cbor.addBytesArray(pool_keyhash, 28);                                      // [2,[ 0/1, keyhash/scripthash ], poolkeyhash ]

    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;
}

void Certificates::addStakeDelegation(Cardano::Credential const ckey, std::uint8_t const *const stake_credential, std::string const & poolID_bech32){
std::uint8_t pool_keyhash[BECH32_MAX_LENGTH]{};
std::uint16_t pool_keyhash_len = 0;
if(!Hash::bech32_decode(poolID_bech32.data(),pool_keyhash,&pool_keyhash_len) && pool_keyhash_len != 28){
    throw std::invalid_argument("addStakeDelegation error, poolID_bech32 is not a valid bech32");
}
addStakeDelegation(ckey, stake_credential, pool_keyhash);
}
// redeemer = [ tag: redeemer_tag, index: uint, data: plutus_data, ex_units: ex_units ]
void Certificates::addRedeemer(std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits ){

    std::unique_ptr<Utils::CborSerialize> rcbor(new Utils::CborSerialize);
    std::unique_ptr<Utils::CborSerialize> unitscbor(new Utils::CborSerialize);
    std::unique_ptr<Utils::PlutusJsonSchema> Json_p(new Utils::PlutusJsonSchema);

    unitscbor->createArray(2);
    unitscbor->addUint(memoryunits); // mem
    unitscbor->addUint(cpusteps);    // step

    Json_p->addSchemaJson(json_redeemer);

    std::vector<std::uint8_t> const & cbor_units = unitscbor->getCbor();
    std::vector<std::uint8_t> const & cbor_plutusdata = Json_p->getCborSchemaJson();

    int numero_certs = cbor_certificates_count - 1;
    if(numero_certs < 0){
      throw std::invalid_argument("Error in addRedeemer: no previous Certificates found");
    }

    Utils::addUint16toVector( redeemer_cert, numero_certs );
    redeemer_cert.push_back(static_cast<std::uint8_t>(2));                                    // tag = 2
    Utils::addUint64toVector(redeemer_cert,cbor_plutusdata.size());                           // plutusdata_len
    redeemer_cert.insert(redeemer_cert.end(),cbor_plutusdata.begin(),cbor_plutusdata.end());  // plutusdata
    Utils::addUint64toVector(redeemer_cert,cbor_units.size());                                // cbor_ex_units_len
    redeemer_cert.insert(redeemer_cert.end(),cbor_units.begin(),cbor_units.end());            // cbor_ex_units

    ++redeemer_cert_count;
    bodymap_countbit |= 0x800;
    witnessmap_countbit |= 0x20;

}

std::vector<std::uint8_t> const & Certificates::getCertificateRedeemers() const{
return redeemer_cert;
}

std::uint16_t const & Certificates::getCertificateRedeemersCount() const{
return redeemer_cert_count;
}

std::uint16_t const & Certificates::getCborCertificatesCount() const{
return cbor_certificates_count;
}

std::uint32_t const & Certificates::getBodyMapcountbit() const{
return bodymap_countbit;
}

std::uint16_t const & Certificates::getWitnessMapcountbit() const{
return witnessmap_countbit;
}

std::vector<std::uint8_t> const & Certificates::getCborCertificates() const {
return cbor_certificates;
}

}
