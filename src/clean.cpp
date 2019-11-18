void deposit::clean( const eosio::name table, const std::optional<eosio::name> scope )
{
    require_auth( get_self() );

    if (table == "incoming"_n) {
        deposit::incoming_table _incoming( get_self(), get_self().value );
        auto incoming_itr = _incoming.begin();
        while ( incoming_itr != _incoming.end() ) {
            incoming_itr = _incoming.erase( incoming_itr );
        }
    } else if (table == "outgoing"_n) {
        deposit::outgoing_table _outgoing( get_self(), get_self().value );
        auto outgoing_itr = _outgoing.begin();
        while ( outgoing_itr != _outgoing.end() ) {
            outgoing_itr = _outgoing.erase( outgoing_itr );
        }
    }
}