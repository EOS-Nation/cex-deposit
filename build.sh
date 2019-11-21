#!/usr/bin/env bash

mkdir -p dist

# deposit
eosio-cpp -abigen \
  ./src/deposit.cpp \
  -contract deposit \
  -R ./resource \
  -o ./dist/deposit.wasm \
  -I ./include
