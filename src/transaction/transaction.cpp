#include "../../include/cardanoplusplus/transaction/transaction.hpp"
#include "../../include/cardanoplusplus/bip32_ed25519/bip32_ed25519.hpp"
#include "../../include/cardanoplusplus/addresses/addresses.hpp"
#include "../../include/cardanoplusplus/utils/txutils.hpp"


namespace Cardano{

Transaction::Transaction(){
    if(sodium_init() < 0){
    throw std::invalid_argument("Transaction error, could not start libsodium");
    }

    feefixed = 155381;
    feeperbytes = 44;

    bytesskyesInwitness =0;
    witnessmapcountbit = 0;
    bytes_transaction = 0;
}

Transaction::~Transaction(){
xskeys_ptr.clear();
}

Transaction::Transaction( std::uint64_t txfeefixed, std::uint64_t txfeeperbytes ): feefixed{txfeefixed}, feeperbytes{txfeeperbytes} {

    bytesskyesInwitness =0;
    witnessmapcountbit = 0;
    bytes_transaction = 0;
}

Transaction &Transaction::AddExtendedSigningKey(std::uint8_t const *const xsk){
    if(xsk!=nullptr){
        xskeys_ptr.push_back(xsk);
    }

    witnessmapcountbit |= 0x0001;
    return *this;
}

std::uint64_t Transaction::getFeeTransacion_PostBuild(std::uint64_t const number_of_signatures){
// Obtener Los datos por partes
if(xskeys_ptr.size() == 0 ){
   std::uint64_t bytes_key_sig = 3 + ( (Utils::bytes_structure_cbornumber(32) + Utils::bytes_structure_cbornumber(64) + 32 + 64) * number_of_signatures );
   return ( (bytes_transaction + bytes_key_sig + 9) * feeperbytes ) + feefixed;
}
return ( (bytes_transaction + 9) * feeperbytes ) + feefixed;
}

std::vector<std::uint8_t> const &Transaction::Build(){

    std::vector<std::uint8_t> const &auxiliary_data = Auxiliarydata.Build();
    // Primero se consulta los datos  de auxiliary data
    // y se agrega el auxiliarydatahash al transaccion body, si es que existe

    //std::size_t txFeeFixed = 155381;
    //std::size_t txFeePerByte = 44;

    if(Auxiliarydata.arethereAuxiliaryData()){
        std::uint8_t blake256[32] = {};
        crypto_generichash_blake2b(blake256, 32, auxiliary_data.data(), auxiliary_data.size(), nullptr, 0);
        Body.addAuxiliaryDataHash(blake256);

    }

    std::vector<std::uint8_t> const &body = Body.Build();
    //Se recostruyen los datos Nuevamente

    std::uint16_t const & witness_build = Body.getWitnessMapcountbit();
    witnessmapcountbit |= witness_build;

    for(std::uint8_t x = 0; x < 7; x++ ){ //se asignan los datos

        if((witnessmapcountbit >> x ) & 0x01){  //revisa cada item del transaction witness
            switch(x){
            case 0:{
                for(uint64_t v =0;v<xskeys_ptr.size();v++){
                    crypto_generichash_blake2b(blake256, 32, body.data(), body.size(), nullptr, 0);
                    signature(xskeys_ptr[v],blake256,32,body_signed);
                    rawprivatekey_to_rawpublickey(xskeys_ptr[v], xvkeys );
                    Witness.addVkeyWitness(xvkeys,body_signed);
                }
            };break;
            case 1:{
            Witness.addNativeScript(Body.TransactionInput.getNativeScripts());
            };break;
            case 2:{};break;
            case 3:{
            Witness.addPlutusV1Script(Body.TransactionInput.getPlutusV1Scripts());
            };break;
            case 4:{
                Witness.addDatum(Body.getcborDatums_afterBuild());
            };break;
            case 5:{
                Witness.addRedeemer(Body.getcborRedeemers_afterBuild());
            };break;
            case 6:{
            Witness.addPlutusV2Script(Body.TransactionInput.getgetPlutusV2Scripts());
            };break;
            }
        }
    }

    std::vector<std::uint8_t> const &witness = Witness.Build(); // se crea el witness
    /// en este paso se debera boorrar los datos VkeyWitness previos para crear los nuevos
    /// con los nuevos datos del txbody, AGREGAR UNA FUNCION A WITNESS PARA BORRAR LAS VkeyWitness



    ///------------------Formando TRansaccion-------------

    // Se construye la transaccion con los datos obtenidos
    cborTransaction.clear(); // Borra todo antes de crear
    cborTransaction.push_back(0x84);                                                             /// [ , , , ]
    cborTransaction.insert(cborTransaction.end(),body.begin(),body.end());                       /// [transaction_body, , , ]
    cborTransaction.insert(cborTransaction.end(),witness.begin(),witness.end());                 /// [transaction_body, transaction_witness_set, , ]
    cborTransaction.push_back(0xf5);                                                             /// [transaction_body, transaction_witness_set, true, ]
    cborTransaction.insert(cborTransaction.end(),auxiliary_data.begin(),auxiliary_data.end());   /// [transaction_body, transaction_witness_set, true, auxiliary_data/null ]
    bytes_transaction = cborTransaction.size();
    return cborTransaction;
}
}
