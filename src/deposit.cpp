#include "deposit.hpp"
#include "get_trx_id.cpp"

[[eosio::on_notify("*::transfer")]]
void deposit::transfer( const name&    from,
                        const name&    to,
                        const asset&   quantity,
                        const string&  memo )
{
    // Only monitor incoming transfers to `get_self()` account
    if ( to != get_self() ) return;

    // authenticate incoming `from` account
    require_auth( from );

    // enforce `eosio.token` contract & `EOS` symbol
    check( get_first_receiver() == "eosio.token"_n, "only incoming transfers from `eosio.token` contract are allowed");
    check( quantity.symbol == symbol{"EOS", 4}, "only incoming transfers using `EOS` symbol are allowed");

    // require minimum balance
    check ( quantity.amount >= 10000, "minimum deposit must be `1.0000 EOS`");

    // memo must not be blank
    check ( memo.length() > 0, "memo must not be blank");

    // send receipt (optional)
    receipt( from, quantity, memo, current_time_point(), get_trx_id() );
}

void deposit::receipt( const eosio::name from, const eosio::asset quantity, const string memo, const eosio::time_point_sec timestamp, const eosio::checksum256 trx_id )
{
    require_auth( get_self() );

    // create row in table
    deposits_table _deposits( get_self(), get_self().value );
    _deposits.emplace( get_self(), [&]( auto& row ) {
        row.id = _deposits.available_primary_key();
        row.from = from;
        row.quantity = quantity;
        row.memo = memo;
        row.timestamp = timestamp;
        row.trx_id = trx_id;
    });
}
