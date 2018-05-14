# FeeSimple Manager

> Provides on-chain CRUD operations for property, event and guest management. data.

## Install

Run `make` on the root to rebuild the contract.

## Deployment

You must have `nodeos` running with an unlocked wallet.

1. Run the command bellow to create an account for the contract. Swap $OWNER_KEY and $ACTIVE_KEY with yours. More information [here](https://github.com/EOSIO/eos/wiki/Tutorial-Getting-Started-With-Contracts).
```
cleos create account eosio fsmgr.code $OWNER_KEY $ACTIVE_KEY
```
2. Run `make reinstall` to rebuild and deploy the contract.

## Database Test Examples

With an account named `user`:
##### Create Property
```
cleos push action fsmgr.code addproperty '["user","Abbey Road Studios","3 Abbey Road, St John`s Wood","2nd Floor","City of Westminster","London","123456",1]' -p user
```

Should return:
```
executed transaction: 3a6e65cde0666532030adaec49d75eef93bda81e5749b23d2d2d92af800007f6  296 bytes  131072 cycles
#      fsmgr.code <= fsmgr.code::addproperty        {"owner":"user","name":"Abbey Road Studios, St John's Wood","address_1":"Abbey Road Studios","address_2":"2nd Floor","city":"City of Westminster","...

```
##### Read Property
```
cleos get table fsmgr.code fsmgr.code property
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
##### Foreign Key Constraints

With account named `user` and at property of `id=0`:
```
cleos push action fsmgr.code addfloorplan '["user",0,"floorplan1",2,3,750,800,1800,2500,7000]' -p user
```
If we attempt to delete the referenced property, an assertion error will be thrown:
```
cleos push action fsmgr.code delproperty '["user",0]' -p user
```
Output:
```
Error 3030000: transaction validation exception
Ensure that your transaction satisfy the contract's constraint!
Error Details:
condition: assertion failed: Foreign key constrant violation: row referenced on floorplans
```
