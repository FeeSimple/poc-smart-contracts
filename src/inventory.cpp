#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>
#include "lib/property.h"
#include "lib/floor.h"

using namespace eosio;
using namespace std;

namespace feesimple{
  class inventory : contract {
    using contract::contract;

  public:
    inventory(account_name self):
      contract(self),
      _properties(_self,_self),
      _floors(_self,_self),
      _floor_images(_self,_self){}

    // PROPERTY TABLE -----------------------------------------------------------

    // @abi action
    void addproperty(name owner, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      _properties.emplace(owner, [&] (auto& row) {
        row.id          = _properties.available_primary_key();
        row.name        = name;
        row.address_1   = address_1;
        row.address_2   = address_2;
        row.city        = city;
        row.region      = region;
        row.postal_code = postal_code;
        row.unit_count  = unit_count;
      });
    }

    // @abi action
    void modproperty(name owner, uint64_t id, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

      auto iter = _properties.find(id);
      _properties.modify( iter, 0, [&]( auto& row) {
        row.name        = name;
        row.address_1   = address_1;
        row.address_2   = address_2;
        row.city        = city;
        row.region      = region;
        row.postal_code = postal_code;
        row.unit_count  = unit_count;
      });
    }

    // @abi action
    void delproperty(name owner, uint64_t id) {
      require_auth(owner);

      auto iter = _properties.find(id);
      _properties.erase(iter);
    }


    // FLOOR TABLE --------------------------------------------------------------

    // @abi action
    void addfloor(name owner, uint64_t property_id, string name, uint64_t bedrooms,
    uint64_t bathrooms, uint64_t sq_ft_min, uint64_t sq_ft_max, uint64_t rent_max,
    uint64_t rent_min, uint64_t deposit){
      require_auth(owner);

      _floors.emplace(owner, [&] (auto& row) {
        row.id          = _floors.available_primary_key();
        row.property_id = property_id;
        row.name        = name;
        row.bedrooms    = bedrooms;
        row.bathrooms   = bathrooms;
        row.sq_ft_min   = sq_ft_min;
        row.sq_ft_max   = sq_ft_max;
        row.rent_min    = rent_min;
        row.rent_max    = rent_max;
        row.deposit     = deposit;
      });
    }

    // @abi action
    void modfloor(name owner, uint64_t id, uint64_t property_id, string name,
    uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, uint64_t deposit) {
      require_auth(owner);

      auto iter = _floors.find(id);
      _floors.modify(iter, 0, [&] (auto& row) {
        row.property_id = property_id;
        row.name        = name;
        row.bedrooms    = bedrooms;
        row.bathrooms   = bathrooms;
        row.sq_ft_min   = sq_ft_min;
        row.sq_ft_max   = sq_ft_max;
        row.rent_min    = rent_min;
        row.rent_max    = rent_max;
        row.deposit     = deposit;
      });
    }

    // @abi action
    void delfloor(name owner, uint64_t id) {
      require_auth(owner);

      auto iter = _floors.find(id);
      _floors.erase(iter);
    }

    // FLOOR IMAGE TABLE --------------------------------------------------------

    // @abi action
    void addfloorimage(name owner, uint64_t floor_id, checksum256 hash,
      string ipfs_address){
      require_auth(owner);

      _floor_images.emplace(owner, [&] (auto& row) {
        row.id           = _floor_images.available_primary_key();
        row.floor_id     = floor_id;
        row.hash         = hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void modfloorimage(name owner, uint64_t id, uint64_t floor_id,
      checksum256 hash, string ipfs_address) {
      require_auth(owner);

      auto iter = _floor_images.find(id);
      _floor_images.modify(iter, 0, [&] (auto& row) {
        row.floor_id     = floor_id;
        row.hash         = hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void delfloorimage(name owner, uint64_t id) {
      require_auth(owner);

      auto iter = _floor_images.find(id);
      _floor_images.erase(iter);
    }

  private:
    multi_index<N(property),   property>    _properties;
    multi_index<N(floor),      floor>       _floors;
    multi_index<N(floorimage), floorimage>  _floor_images;
  };

  EOSIO_ABI(inventory, (addproperty)(modproperty)(delproperty)(addfloor)(modfloor)(delfloor)(addfloorimage)(modfloorimage)(delfloorimage));
}
