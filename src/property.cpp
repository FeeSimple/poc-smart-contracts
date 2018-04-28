
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/db.h>


class property_contract : public eosio::contract {
  public:
      using eosio::contract::contract;

      void create(account_name receiver) {
         eosio::print("Creating property");
         auto property = N(property);

         db_store_i64(receiver, property, receiver, N(bb), "{name:Blueberry Building}", strlen("{name:Blueberry Building}"));
      }

      void read(account_name receiver) {
         eosio::print("Reading property");
      }

      void update(account_name receiver) {
         eosio::print("Updating property");
      }

      void remove(account_name receiver) {
         eosio::print("Deleting property");
      }
};

EOSIO_ABI( property_contract, (create)(read)(update)(remove) )
