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
  inventory(name self):contract(self){}
  // PROPERTY TABLE -----------------------------------------------------------

  // @abi action
  void addproperty(name owner, string name, string address_1, string address_2,
    string city, string region, string postal_code, uint64_t unit_count) {
    require_auth(owner);

    _Properties properties(_self,_self);

    properties.emplace(owner, [&] (auto& row) {
      row.id          = properties.available_primary_key();

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

    _Properties properties(_self,_self);

    auto iter = properties.find(id);
    properties.modify( iter, 0, [&]( auto& row) {
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

    _Properties properties(_self,_self);

    auto iter = properties.find(id);
    properties.erase(iter);
  }


  // FLOOR TABLE --------------------------------------------------------------

  // @abi action
  void addfloor(name owner, uint64_t property_id, string name, uint64_t bedrooms,
  uint64_t bathrooms, uint64_t sq_ft_min, uint64_t sq_ft_max, uint64_t rent_max,
  uint64_t rent_min, uint64_t deposit){
    require_auth(owner);

    _Floors floors(_self,_self);

    floors.emplace(owner, [&] (auto& row) {
      row.id = floors.available_primary_key();

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

    _Floors floors(_self,_self);

    auto iter = floors.find(id);
    floors.modify(iter, 0, [&] (auto& row) {
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

    _Floors floors(_self,_self);

    auto iter = floors.find(id);
    floors.erase(iter);
  }

  // FLOOR IMAGE TABLE --------------------------------------------------------

  // @abi action
  void addfloorimage(name owner, uint64_t floor_id, checksum256 hash,
    string ipfs_address){
    require_auth(owner);

    _FloorImages floorimages(_self,_self);

    floorimages.emplace(owner, [&] (auto& row) {
      row.id           = floorimages.available_primary_key();

      row.floor_id     = floor_id;
      row.hash         = hash;
      row.ipfs_address = ipfs_address;
    });
  }

  // @abi action
  void modfloorimage(name owner, uint64_t id, uint64_t floor_id,
    checksum256 hash, string ipfs_address) {
    require_auth(owner);

    _FloorImages floorimages(_self,_self);

    auto iter = floorimages.find(id);
    floorimages.modify(iter, 0, [&] (auto& row) {
      row.floor_id     = floor_id;
      row.hash         = hash;
      row.ipfs_address = ipfs_address;
    });
  }

  // @abi action
  void delfloorimage(name owner, uint64_t id) {
    require_auth(owner);

    _FloorImages floorimages(_self,_self);

    auto iter = floorimages.find(id);
    floorimages.erase(iter);
  }

private:
  typedef multi_index<N(property),  Properties>   _Properties;
  typedef multi_index<N(floor),      Floors>      _Floors;
  typedef multi_index<N(floorImage), FloorImages> _FloorImages;
};

EOSIO_ABI(inventory, (addproperty)(modproperty)(delproperty)(addfloor)(modfloor)(delfloor)(addfloorimage)(modfloorimage)(delfloorimage));
