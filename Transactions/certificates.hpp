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

#ifndef CERTIFICATES_HPP
#define CERTIFICATES_HPP

#include <vector>
#include <cstdint>
#include "../Utils/cbor_lite.hpp"
#include "../Utils/txutils.hpp"
#include "../Utils/plutusjsonschema.hpp"

class Certificates {

public:
    explicit Certificates();
    virtual ~Certificates();

    enum class Credential{
KeyHash = 0,
ScriptHash
};

    void addStakeRegistration(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash);
    void addStakeDeregistration(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash);
    void addStakeDelegation(Certificates::Credential ckey, std::uint8_t const *const stake_credential_hash, std::uint8_t const *const pool_keyhash);
    void addCertificateRedeemer( std::string & json_redeemer, std::uint64_t const cpusteps, std::uint64_t const memoryunits );
    std::vector<std::uint8_t> const & getCertificateRedeemers() const;

    std::uint16_t const & getCertificateRedeemersCount() const;
    std::uint32_t const & getBodyMapcountbit() const;
    std::uint16_t const & getWitnessMapcountbit() const;
    std::uint16_t const & getCborCertificatesCount() const;  //serializa en cbor los certificados
    std::vector<std::uint8_t> const & getCborCertificates() const;  //serializa en cbor los certificados
private:

    std::uint16_t redeemer_cert_count;
    std::uint16_t cbor_certificates_count;
    std::vector <std::uint8_t> redeemer_cert{};
    std::vector <std::uint8_t> cbor_certificates{};
    CborSerialize cert_cbor;
    std::uint32_t bodymap_countbit;      ///  0x0001 , Tiene que iniciar con cero
    std::uint16_t witnessmap_countbit;      ///  0x0001 , Tiene que iniciar con cero

};


#endif
