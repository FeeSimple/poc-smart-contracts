#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include "lib/dapp.h"


using namespace eosio;
using namespace std;

namespace feesimple{
  class dapphub : contract {
    using contract::contract;

  public:
    dapphub(account_name self):contract(self){}      

    // DAPP TABLE -----------------------------------------------------------    

    // @abi action
    void adddapp(name author, string name, string category, string account) {
      require_auth(author);

      dapp_index dapps(_self,author);
      dapps.emplace(author, [&] (auto& row) {
        row.id       = dapps.available_primary_key();
        row.name     = name;
        row.category = category;
        row.account  = account;
      });
    }

    // @abi action
    void moddapp(name author, uint64_t id, string name, string category, string account) {
      require_auth(author);

      dapp_index dapps(_self,author);
      auto iter = dapps.find(id);
      eosio_assert(iter != dapps.end(), "Dapp does not exist");

      dapps.modify( iter, 0, [&]( auto& row) {
        row.name     = name;
        row.category = category;
        row.account  = account;
      });
    }

    // @abi action
    void deldapp(name author, uint64_t id) {
      require_auth(author);      

      dapp_index dapps(_self,author);
      auto iter = dapps.find(id); 
      eosio_assert(iter != dapps.end(), "Dapp does not exist");

      dapps.erase(iter);
    }

  private:
    typedef multi_index<N(dapp), dapp> dapp_index;
  };

  EOSIO_ABI(dapphub,(adddapp)(moddapp)(deldapp));
}
