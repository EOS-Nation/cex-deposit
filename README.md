# CEX Deposit

> Handles incoming EOS token transfers to Centralized Exchange deposit account.

## Chains

- [Jungle](https://jungle.bloks.io/account/deposits1111)

## Basic Features

- [x] Must be `eosio.token` contract & `EOS` symbol (no fake EOS)
- [x] Must deposit greater then `1.0000 EOS` (minimum deposit amount)
- [x] Must require a memo (can't be blank)
- [x] Must require a memo (can't be blank)

## Advanced Features

- [ ] Memo must be all numbers (no letters)
- [ ] Memo must be 8 digits (or whatever your internal database uses for ID's)
- [ ] Allow incoming deposits by verrifying `extended_symbol` (ex: only accept `eosio.token` contract & `EOS` symbol)
- [ ] Whitelist accounts (ex: `eosio.ram` & `eosio.stake`) to prevent all checks from incoming transfers
- [ ] Blacklist accounts, prevent deposits from specified accounts
- [ ] Trigger receipt notifier
- [ ] Clear table row using `id` & `trx_id`

## Quickstart

```bash
$ ./build.sh
```

## Deploy contract

```bash
$ cleos set contract <ACCOUNT> ./dist deposit.wasm deposit.abi
```

## Add `<account>@eosio.code`

```bash
$ cleos set account permission <ACCOUNT> active <ACCOUNT> owner --add-code
```

# EOSIO Smart Contract

## ACTION `receipt`

Creates a receipt of incoming deposit and stores row in database

- Authority:  `get_self()`

### params

- `{name} from` - account which sent deposit
- `{asset} quantity` - amount of deposit
- `{string} memo` - memo used
- `{time_point_sec} timestamp` - time of deposit
- `{checksum256} trx_id` - transaction id

### example

```bash
cleos push action deposit push '["myaccount", "1.0000 EOS", "12345", "2019-11-14T12:00:00", "TRANSACTION ID"]' -p deposit
```

## TABLE `deposits`

- `{uint64_t} id` - deposit id
- `{name} from` - account which sent deposit
- `{asset} quantity` - amount of deposit
- `{string} memo` - memo used
- `{time_point_sec} timestamp` - time of deposit
- `{checksum256} trx_id` - transaction id

### example

```json
{
  "id": 1,
  "from": "myaccount",
  "quantity": "1.0000 EOS",
  "memo": "12345",
  "timestamp": "2019-11-14T12:00:00",
  "trx_id": "TRANSACTION ID",
}
```