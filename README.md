# FeeSimple Inventory

> Provides on-chain CRUD operations for inventory data.

## Install

Run `make` on the root to rebuild the contract.

## Deployment

You must have `nodeos` running with an unlocked wallet.

1. Run the command bellow to create an account for the contract. Swap $OWNER_KEY and $ACTIVE_KEY with yours. More information [here](https://github.com/EOSIO/eos/wiki/Tutorial-Getting-Started-With-Contracts).
```
cleos create account eosio inv.code $OWNER_KEY $ACTIVE_KEY
```
2. Run `make reinstall` to rebuild and deploy the contract.

## Usage example

With an account named `user`:
#### Create
```
cleos push action inv.code addproperty '["user","Abbey Road Studios","3 Abbey Road, St John`s Wood","2nd Floor","City of Westminster","London","123456",1]' -p user
```

Should return something like:
```
executed transaction: 3a6e65cde0666532030adaec49d75eef93bda81e5749b23d2d2d92af800007f6  296 bytes  131072 cycles
#      inv.code <= inv.code::addproperty        {"owner":"user","name":"Abbey Road Studios, St John's Wood","address_1":"Abbey Road Studios","address_2":"2nd Floor","city":"City of Westminster","...

```
#### Read
```
cleos get table inv.code inv.code properties
```
Should return:
```
{
  "rows": [{
      "id": 0,
      "owner": "user",
      "name": "Abbey Road Studios",
      "address_1": "3 Abbey Road, St John`s Wood",
      "address_2": "2nd Floor",
      "city": "City of Westminster",
      "region": "London",
      "postal_code": "123456",
      "unit_count": 1
    }
  ],
  "more": false
}
```
