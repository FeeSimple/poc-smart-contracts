#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>
#include "lib/floorplan.h"
#include "lib/property.h"

using namespace eosio;
using namespace std;

namespace feesimple{
  class inventory : contract {
    using contract::contract;

  public:
    inventory(account_name self):
      contract(self),
      properties(_self,_self),
      floorplans(_self,_self),
      flplanimgs(_self,_self){}

    // PROPERTY TABLE -----------------------------------------------------------

    // @abi action
    void addproperty(name owner, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(owner);

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

      auto propidx = floorplans.get_index<N(property_id)>();
      auto floorplan = propidx.find(id);
      eosio_assert(floorplan == propidx.end(), "Foreign key constrant violation: row referenced on floorplans");

      auto iter = properties.find(id);
      properties.erase(iter);
    }


    // FLOOR TABLE --------------------------------------------------------------

    // @abi action
    void addfloorplan(name owner, uint64_t property_id, string name, uint64_t bedrooms,
    uint64_t bathrooms, uint64_t sq_ft_min, uint64_t sq_ft_max, uint64_t rent_min,
    uint64_t rent_max, uint64_t deposit){
      require_auth(owner);

      auto property = properties.find(property_id);
      eosio_assert(property != properties.end(), "Referenced property does not exist");

      floorplans.emplace(owner, [&] (auto& row) {
        row.id          = floorplans.available_primary_key();
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
    void modfloorplan(name owner, uint64_t id, uint64_t property_id, string name,
    uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, uint64_t deposit) {
      require_auth(owner);

      auto iter = floorplans.find(id);
      floorplans.modify(iter, 0, [&] (auto& row) {
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
    void delfloorplan(name owner, uint64_t id) {
      require_auth(owner);

      auto floorplanidx = flplanimgs.get_index<N(floorplan_id)>();
      auto flplanimg = floorplanidx.find(id);
      eosio_assert(flplanimg == floorplanidx.end(), "Foreign key constrant violation: row referenced on flplanimgs");

      auto iter = floorplans.find(id);
      floorplans.erase(iter);
    }

    // FLOOR IMAGE TABLE --------------------------------------------------------

    // @abi action
    void addflplanimg(name owner, uint64_t floorplan_id, checksum256 image_hash,
      string ipfs_address){
      require_auth(owner);

      flplanimgs.emplace(owner, [&] (auto& row) {
        row.id           = flplanimgs.available_primary_key();
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void modflplanimg(name owner, uint64_t id, uint64_t floorplan_id,
      checksum256 image_hash, string ipfs_address) {
      require_auth(owner);

      auto iter = flplanimgs.find(id);
      flplanimgs.modify(iter, 0, [&] (auto& row) {
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void delflplanimg(name owner, uint64_t id) {
      require_auth(owner);

      auto iter = flplanimgs.find(id);
      flplanimgs.erase(iter);
    }

  private:
    typedef multi_index<N(property), property>     property_index;
    property_index  properties;

    typedef multi_index<N(floorplan), floorplan,
      indexed_by< N(property_id), const_mem_fun<floorplan, uint64_t, &floorplan::by_property>>
      > floorplan_index;
    floorplan_index floorplans;

    typedef multi_index<N(floorplanimg), floorplanimg,
      indexed_by< N(floorplan_id), const_mem_fun<floorplanimg, uint64_t, &floorplanimg::by_floorplan>>
      > flplanimg_index;
    flplanimg_index flplanimgs;

  };

  EOSIO_ABI(inventory,(addproperty)(modproperty)(delproperty)(addfloorplan)(modfloorplan)(delfloorplan)(addflplanimg)(modflplanimg)(delflplanimg));
}
