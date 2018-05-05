#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>
#include "lib/property.h"
#include "lib/floor.h"

using namespace eosio;
using namespace std;
using namespace property;
using namespace floor;

class inventory : contract {
  using contract::contract;

public:
  inventory( account_name self ) : contract(self){}

  // @abi action
  void addproperty( name owner, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      Properties properties(_self,_self);

      properties.emplace(owner, [&] (auto& row) {
          row.id = properties.available_primary_key();
          row.name = name;
          row.address_1 = address_1;
          row.address_2 = address_2;
          row.city = city;
          row.region = region;
          row.postal_code = postal_code;
          row.unit_count = unit_count;
      });

  }

  // @abi action
  void modproperty(name owner, uint64_t id, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      Properties properties(_self,_self);

      auto iter = properties.find(id);
      properties.modify( iter, 0, [&]( auto& row) {
          row.id = id;
          row.name = name;
          row.address_1 = address_1;
          row.address_2 = address_2;
          row.city = city;
          row.region = region;
          row.postal_code = postal_code;
          row.unit_count = unit_count;
      });
  }

  // @abi action
  void delproperty(uint64_t id, name owner) {
      require_auth(owner);

      Properties properties(_self,_self);

      auto iter = properties.find(id);
      properties.erase(iter);
  }


private:
  typedef multi_index<N(property), Property> Properties;
  typedef multi_index<N(floor), Floor> Floors;
};

EOSIO_ABI(inventory, (addproperty)(modproperty)(delproperty));
