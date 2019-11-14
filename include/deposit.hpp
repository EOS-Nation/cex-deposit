#pragma once

#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("deposit")]] deposit : public contract {
public:
    using contract::contract;

    [[eosio::on_notify("*::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    [[eosio::action]]
    void receipt( const eosio::name from, const eosio::asset quantity, const string memo, const eosio::time_point_sec timestamp, const eosio::checksum256 trx_id );

    using receipt_action = eosio::action_wrapper<"receipt"_n, &deposit::receipt>;

private:
    struct [[eosio::table("deposits")]] deposits_row {
        uint64_t                id;
        eosio::name             from;
        eosio::asset            quantity;
        string                  memo;
        eosio::time_point_sec   timestamp;
        eosio::checksum256      trx_id;

        uint64_t primary_key() const { return id; }
    };

    // Tables
    typedef eosio::multi_index< "deposits"_n, deposits_row> deposits_table;

    // private helpers
    checksum256 get_trx_id();
};