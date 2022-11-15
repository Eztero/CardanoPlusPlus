#include "transaction.hpp"
Transaction::Transaction(){

}

std::vector const &Transaction::Build(){

    return cborTransaction;
}
