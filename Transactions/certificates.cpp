#include "certificates.hpp"

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

void Certificates::addStakeRegistration(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash){
    // stake_credential_hash(28bytes)

    cert_cbor.clearCbor();
    cert_cbor.createArray(2);                                                       // [ , ]
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [0,   ]
    cert_cbor.createArray(2);                                                       // [0,[ ] ]
    switch(ckey){
    case Credential::KeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [0,[0, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [0,[0, addr_keyhash ] ]

    };break;
    case Credential::ScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                                    // [0,[1, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [0,[1, scripthash ] ]
    };break;
    }


    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;

}

void Certificates::addStakeDeregistration(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash){
    // stake_credential_hash(28bytes)
    cert_cbor.clearCbor();
    cert_cbor.createArray(2);                                                       // [ , ]
    cert_cbor.addUint(1);                                                           // [1,   ]
    cert_cbor.createArray(2);                                                       // [1,[ ] ]
    switch(ckey){
    case Credential::KeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [1,[0, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [1,[0, addr_keyhash ] ]

    };break;
    case Credential::ScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                                    // [1,[1, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [1,[1, scripthash ] ]
    };break;
    }

    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;
}

void Certificates::addStakeDelegation(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash, std::uint8_t const *const pool_keyhash){
    // stake_credential_hash(28bytes) + addr_poolkeyhash(28bytes) = 56 bytes
    cert_cbor.clearCbor();
    cert_cbor.createArray(3);                                                       // [ , , ]
    cert_cbor.addUint(2);                                                           // [2, , ]
    cert_cbor.createArray(2);                                                       // [2,[ , ], ]
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [2,[0, ], ]
    switch(ckey){
    case Credential::KeyHash:{
    cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [2,[0, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [2,[0, addr_keyhash ] ]

    };break;
    case Credential::ScriptHash:{
    cert_cbor.addUint(static_cast<uint64_t>(1));                                    // [2,[1, ] ]
    cert_cbor.addBytesArray(stake_credential_hash, 28);                             // [2,[1, scripthash ] ]
    };break;
    }
    cert_cbor.addBytesArray(pool_keyhash, 28);                                      // [2,[ 0/1, keyhash/scripthash ], poolkeyhash ]

    std::vector<std::uint8_t> const &cbor = cert_cbor.getCbor();
    cbor_certificates.insert(cbor_certificates.end(),cbor.begin(),cbor.end());

    ++cbor_certificates_count;
    bodymap_countbit = 0x10;
}

// redeemer = [ tag: redeemer_tag, index: uint, data: plutus_data, ex_units: ex_units ]
void Certificates::addCertificateRedeemer(std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits ){

    std::unique_ptr<CborSerialize> rcbor(new CborSerialize);
    std::unique_ptr<CborSerialize> unitscbor(new CborSerialize);
    std::unique_ptr<PlutusJsonSchema> Json_p(new PlutusJsonSchema);

    unitscbor->createArray(2);
    unitscbor->addUint(memoryunits); // mem
    unitscbor->addUint(cpusteps);    // step

    Json_p->addSchemaJson(json_redeemer);

    std::vector<std::uint8_t> const & cbor_units = unitscbor->getCbor();
    std::vector<std::uint8_t> const & cbor_plutusdata = Json_p->getCborSchemaJson();

    addUint16toVector( redeemer_cert, (cbor_certificates_count - 1 ) );                       /// Index_redeemer , LANZAR ERROR SI tx_input_count=0
    redeemer_cert.push_back(static_cast<std::uint8_t>(2));                                    // tag = 2
    addUint64toVector(redeemer_cert,cbor_plutusdata.size());                                  // plutusdata_len
    redeemer_cert.insert(redeemer_cert.end(),cbor_plutusdata.begin(),cbor_plutusdata.end());  // plutusdata
    addUint64toVector(redeemer_cert,cbor_units.size());                                       // cbor_ex_units_len
    redeemer_cert.insert(redeemer_cert.end(),cbor_units.begin(),cbor_units.end());            // cbor_ex_units

    ++redeemer_cert_count;
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
