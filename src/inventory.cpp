#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;
using namespace std;

class inventory : contract {
  using contract::contract;

public:
  inventory( account_name self ) :
  contract(self),
  _property(_self,_self){}

  // @abi action
  void addproperty( name owner, string name, string address1, string address2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      _property.emplace(owner, [&] (auto& row) {
          row.id = _property.available_primary_key();
          row.owner = owner;
          row.name = name;
          row.address1 = address1;
          row.address2 = address2;
          row.city = city;
          row.region = region;
          row.postal_code = postal_code;
          row.unit_count = unit_count;
      });

  }

  // @abi action
  void modproperty(name owner, uint64_t id, string name, string address1, string address2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      auto iter = _property.find(id);
      _property.modify( iter, 0, [&]( auto& row) {
          row.id = id;
          row.owner = owner;
          row.name = name;
          row.address1 = address1;
          row.address2 = address2;
          row.city = city;
          row.region = region;
          row.postal_code = postal_code;
          row.unit_count = unit_count;
      });
  }

  // @abi action
  void delproperty(uint64_t id, name owner) {
      require_auth(owner);

      auto iter = _property.find(id);
      _property.erase(iter);
  }


private:
  // @abi table
  struct property {
      uint64_t id;
      name owner;

      string name;
      string address1;
      string address2;
      string city;
      string region;
      string postal_code;
      uint64_t unit_count;

      auto primary_key()const { return id; }

      EOSLIB_SERIALIZE(property, (id)(owner)(name)(address1)(address2)(city)(region)(postal_code)(unit_count));
  };

  multi_index<N(property), property> _property;
};

EOSIO_ABI( inventory, (addproperty)(modproperty)(delproperty) );
