
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

class guest_contract : public eosio::contract {
  public:
      using eosio::contract::contract;

      void create(account_name receiver) {
         eosio::print("Hello feesimple");
      }
};

EOSIO_ABI( guest_contract, (ping) )
