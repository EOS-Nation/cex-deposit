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

    /**
     * ## ACTION `receipt`
     *
     * Creates a receipt of incoming deposit and stores row in database
     *
     * - Authority:  `get_self()`
     *
     * ### params
     *
     * - `{name} from` - account which sent deposit
     * - `{asset} quantity` - amount of deposit
     * - `{string} memo` - memo used
     * - `{time_point_sec} timestamp` - time of deposit
     * - `{checksum256} trx_id` - transaction id
     *
     * ### example
     *
     * ```bash
     * cleos push action deposit push '["myaccount", "1.0000 EOS", "12345", "2019-11-14T12:00:00", "<TRANSACTION ID>"]' -p deposit
     * ```
     */
    [[eosio::action]]
    void receipt( const eosio::name from, const eosio::asset quantity, const string memo, const eosio::time_point_sec timestamp, const eosio::checksum256 trx_id );

    [[eosio::action]]
    void clean( const eosio::name table, const std::optional<eosio::name> scope );

    [[eosio::on_notify("*::transfer")]]
    void transfer( const name from, const name to, const asset quantity, const string memo );

    using receipt_action = eosio::action_wrapper<"receipt"_n, &deposit::receipt>;

private:
    /**
     * ## TABLE `incoming` & `outgoing` deposits
     *
     * - `{uint64_t} id` - deposit id
     * - `{name} from` - account which sent deposit
     * - `{asset} quantity` - amount of deposit
     * - `{string} memo` - memo used
     * - `{time_point_sec} timestamp` - time of deposit
     * - `{checksum256} trx_id` - transaction id
     *
     * ### example
     *
     * ```json
     * {
     *   "id": 1,
     *   "from": "myaccount",
     *   "quantity": "1.0000 EOS",
     *   "memo": "12345",
     *   "timestamp": "2019-11-14T12:00:00",
     *   "trx_id": "<TRANSACTION ID>",
     * }
     * ```
     */
    struct deposits_row {
        uint64_t                id;
        eosio::name             from;
        eosio::asset            quantity;
        string                  memo;
        eosio::time_point_sec   timestamp;
        eosio::checksum256      trx_id;

        uint64_t primary_key() const { return id; }
    };

    struct [[eosio::table("outgoing")]] outgoing_row : public deposits_row{
        uint64_t primary_key() const { return id; }
    };

    struct [[eosio::table("incoming")]] incoming_row : public deposits_row{
        uint64_t primary_key() const { return id; }
    };

    // Tables
    typedef eosio::multi_index< "incoming"_n, incoming_row> incoming_table;
    typedef eosio::multi_index< "outgoing"_n, outgoing_row> outgoing_table;

    // private helpers
    checksum256 get_trx_id();
    void incoming( const name from, const name to, const asset quantity, const string memo );
    void outgoing( const name from, const name to, const asset quantity, const string memo );
};