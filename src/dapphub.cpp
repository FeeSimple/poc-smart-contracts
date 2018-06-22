#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include "lib/dapp.h"


using namespace eosio;
using namespace std;

namespace feesimple{
  class dapphub : contract {
    using contract::contract;

  public:
    dapphub(account_name self):
      contract(self),
      dapps(_self,_self){}

    // DAPP TABLE -----------------------------------------------------------    

    // @abi action
    void adddapp(name owner, string name, string category, string account) {
      require_auth(owner);

      dapps.emplace(owner, [&] (auto& row) {
        row.id       = dapps.available_primary_key();
        row.name     = name;
        row.category = category;
        row.account  = account;
      });
    }

    // @abi action
    void moddapp(name owner, uint64_t id, string name, string category, string account) {
      require_auth(owner);

      auto iter = dapps.find(id);
      eosio_assert(iter != dapps.end(), "Dapp does not exist");

      dapps.modify( iter, 0, [&]( auto& row) {
        row.name     = name;
        row.category = category;
        row.account  = account;
      });
    }

    // @abi action
    void deldapp(name owner, uint64_t id) {
      require_auth(owner);      

      auto iter = dapps.find(id);
      eosio_assert(iter != dapps.end(), "Dapp does not exist");

      dapps.erase(iter);
    }

  private:
    typedef multi_index<N(dapp), dapp> dapp_index;
    dapp_index dapps;
  };

  EOSIO_ABI(dapphub,(adddapp)(moddapp)(deldapp));
}
