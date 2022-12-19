#include "certificates.hpp"

Certificates::Certificates(){
    ptrvec = nullptr;
    buff_sizet = 0;
    certificatesmapcountbit = 0;
    stake_registration_count = 0;
    stake_deregistration_count = 0;
    stake_delegation_count = 0;
}

Certificates::~Certificates(){
    ptrvec = nullptr;
}

void Certificates::addStakeRegistration(std::uint8_t const *const addr_stakekeyhash){
    // addr_stakekeyhash(28bytes)

    if(stake_registration_count < UINT16_MAX && !existen_coincidencias(addr_stakekeyhash, stake_registration.data(),28,stake_registration_count,28)){
        buff_sizet = static_cast<std::size_t>( stake_registration.capacity() ) - static_cast<std::size_t>( stake_registration.size() );

        // Si la capacidad reservada es menor a la que se debe ingresar se aumenta el espacio de reserva
        if(buff_sizet < 28){
            stake_registration.reserve(stake_registration.size() + 28);
        }
        stake_registration.insert(stake_registration.end(), addr_stakekeyhash, addr_stakekeyhash + 28);

        stake_registration_count++;

        certificatesmapcountbit |= 0x01;
    }
}

void Certificates::addStakeDeregistration(std::uint8_t const *const addr_stakekeyhash){
    // addr_stakekeyhash(28bytes)

    if(stake_deregistration_count < UINT16_MAX && !existen_coincidencias(addr_stakekeyhash, stake_deregistration.data(),28,stake_deregistration_count,28)){
        buff_sizet = static_cast<std::size_t>( stake_deregistration.capacity() ) - static_cast<std::size_t>( stake_deregistration.size() );
        if(buff_sizet < 28){
            stake_deregistration.reserve(stake_deregistration.size() + 28);
        }
        stake_deregistration.insert(stake_deregistration.end(),addr_stakekeyhash,addr_stakekeyhash + 28);

        stake_deregistration_count++;

        certificatesmapcountbit |= 0x02;
    }
}

void Certificates::addStakeDelegation(std::uint8_t const *const addr_stakekeyhash, std::uint8_t const *const addr_poolkeyhash){
    // addr_stakekeyhash(28bytes) + addr_poolkeyhash(28bytes) = 56 bytes

    if(stake_delegation_count < UINT16_MAX && !existen_coincidencias(addr_stakekeyhash, stake_delegation.data(),28,stake_delegation_count,56)){
        buff_sizet = static_cast<std::size_t>( stake_delegation.capacity() ) - static_cast<std::size_t>( stake_delegation.size() );
        if(buff_sizet < 56){
            stake_delegation.reserve(stake_delegation.size() + 56);
        }

        stake_delegation.insert(stake_delegation.end(), addr_stakekeyhash, addr_stakekeyhash + 28);
        stake_delegation.insert(stake_delegation.end(), addr_poolkeyhash, addr_poolkeyhash + 28);

        stake_delegation_count++;

        certificatesmapcountbit |= 0x04;
    }
}

bool Certificates::arethereCertificates() const{
    return (certificatesmapcountbit > 0) ? true : false;
}


std::vector<std::uint8_t> const &Certificates::getCborCertificates(){

    CborSerialize cert_cbor(&cborCertificates);
    cert_cbor.ClearCbor();

    if(certificatesmapcountbit > 0){ //Condicion que salta el proceso en caso de que no hayan datos
        cert_cbor.createArray(stake_delegation_count + stake_deregistration_count + stake_registration_count); // [ ]

        for(std::uint8_t x = 0; x < 3; x++ ){ //se asignan los datos
            if((certificatesmapcountbit >> x ) & 0x01){  //revisa cada item de Certificates
                switch(x){
                case 0:{
                    ptrvec = stake_registration.data();

                    for(std::uint8_t s = 0; s < stake_registration_count; s++ ){

                        cert_cbor.createArray(2);                                                       // [ , ]
                        cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [0,   ]
                        cert_cbor.createArray(2);                                                       // [0,[ ] ]
                        cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [0,[0, ] ]
                        cert_cbor.addBytesArray(ptrvec, 28);                                            // [0,[0, keyhash ] ]
                        ptrvec += 28;
                    }
                };break;
                case 1:{
                    ptrvec = stake_deregistration.data();

                    for(std::uint8_t s = 0; s < stake_deregistration_count; s++ ){

                        cert_cbor.createArray(2);                                                       // [ , ]
                        cert_cbor.addUint(1);                                                           // [1,   ]
                        cert_cbor.createArray(2);                                                       // [1,[ ] ]
                        cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [1,[0, ] ]
                        cert_cbor.addBytesArray(ptrvec, 28);                                            // [1,[0, keyhash ] ]
                        ptrvec += 28;
                    }

                };break;
                case 2:{
                    ptrvec = stake_delegation.data();

                    for(std::uint8_t s = 0; s < stake_delegation_count; s++ ){

                        cert_cbor.createArray(3);                                                       // [ , , ]
                        cert_cbor.addUint(2);                                                           // [2, , ]
                        cert_cbor.createArray(2);                                                       // [2,[ , ], ]
                        cert_cbor.addUint(static_cast<uint64_t>(0));                                    // [2,[0, ], ]
                        cert_cbor.addBytesArray(&ptrvec[0], 28);                                        // [2,[0, keyhash ], ]
                        cert_cbor.addBytesArray(&ptrvec[28], 28);                                       // [2,[0, keyhash ], poolkeyhash ]
                        ptrvec += 56;
                    }

                };break;
                }
            }
        }

    }
    return cborCertificates;
}
