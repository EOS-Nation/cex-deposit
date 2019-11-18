#include "deposit.hpp"
#include "get_trx_id.cpp"
#include "clean.cpp"

[[eosio::on_notify("*::transfer")]]
void deposit::transfer( const name from, const name to, const asset quantity, const string memo )
{
    // exclude system contracts
    if ( from == "eosio.ram"_n || from == "eosio.stake"_n ) return;

    // Monitor incoming transfers to contract account
    if ( to == get_self() ) incoming( from, to, quantity, memo );

    // Monitor outgoing transfers from contract account
    if ( from == get_self() ) outgoing( from, to, quantity, memo );
}

void deposit::outgoing( const name from, const name to, const asset quantity, const string memo )
{
    // authenticate incoming `from` account
    require_auth( from );

    // create row in table
    outgoing_table _outgoing( get_self(), get_self().value );
    _outgoing.emplace( get_self(), [&]( auto& row ) {
        row.id = _outgoing.available_primary_key();
        row.from = from;
        row.quantity = quantity;
        row.memo = memo;
        row.timestamp = current_time_point();
        row.trx_id = get_trx_id();
    });
}

void deposit::incoming( const name from, const name to, const asset quantity, const string memo )
{
    // authenticate incoming `from` account
    require_auth( from );

    // enforce `eosio.token` contract & `EOS` symbol
    check( get_first_receiver() == "eosio.token"_n, "only incoming transfers from `eosio.token` contract are allowed");
    check( quantity.symbol == symbol{"EOS", 4}, "only incoming transfers using `EOS` symbol are allowed");

    // require minimum balance
    check ( quantity.amount >= 10000, "minimum deposit must be `1.0000 EOS`");

    // memo must not be blank
    check ( memo.length() > 0, "memo must not be blank");

    // create row in table
    incoming_table _incoming( get_self(), get_self().value );
    _incoming.emplace( get_self(), [&]( auto& row ) {
        row.id = _incoming.available_primary_key();
        row.from = from;
        row.quantity = quantity;
        row.memo = memo;
        row.timestamp = current_time_point();
        row.trx_id = get_trx_id();
    });

    // send receipt (optional)
    // contract account must have `<deposit>@eosio.code` permission setup
    deposit::receipt_action receipt( get_self(), { get_self(), "active"_n });
    receipt.send( from, quantity, memo, current_time_point(), get_trx_id() );
}

void deposit::receipt( const eosio::name from, const eosio::asset quantity, const string memo, const eosio::time_point_sec timestamp, const eosio::checksum256 trx_id )
{
    require_auth( get_self() );
}
