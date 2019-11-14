#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>

checksum256 deposit::get_trx_id()
{
    size_t size = eosio::transaction_size();
    char buf[size];
    size_t read = eosio::read_transaction( buf, size );
    check( size == read, "read_transaction failed");
    return eosio::sha256( buf, read );
}