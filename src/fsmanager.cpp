#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include "lib/floorplan.h"
#include "lib/property.h"
#include "lib/unit.h"
#include "lib/guest.h"
#include "lib/event.h"

using namespace eosio;
using namespace std;

namespace feesimple{
  class fsmanager : contract {
    using contract::contract;

  public:
    fsmanager(account_name self):
      contract(self),
      properties(_self,_self),
      floorplans(_self,_self),
      flplanimgs(_self,_self),
      units(_self,_self),
      termpricings(_self,_self),
      guests(_self,_self),
      events(_self,_self){}

    // PROPERTY TABLE -----------------------------------------------------------

    // @abi action
    void addproperty(account_name author, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(author);

      properties.emplace(author, [&] (auto& row) {
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
    void modproperty(account_name author, uint64_t id, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count) {
      require_auth(author);

      auto iter = properties.find(id);
      eosio_assert(iter != properties.end(), "Property does not exist");

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
    void delproperty(account_name author, uint64_t id) {
      require_auth(author);

      auto propidx = floorplans.get_index<N(property_id)>();
      auto floorplan = propidx.find(id);
      eosio_assert(floorplan == propidx.end(), "Foreign key constrant violation: row referenced on floorplans");

      auto iter = properties.find(id);
      eosio_assert(iter != properties.end(), "Property does not exist");

      properties.erase(iter);
    }


    // FLOOR TABLE --------------------------------------------------------------

    // @abi action
    void addfloorplan(account_name author, uint64_t property_id, string name, uint64_t bedrooms,
    uint64_t bathrooms, uint64_t sq_ft_min, uint64_t sq_ft_max, uint64_t rent_min,
    uint64_t rent_max, uint64_t deposit){
      require_auth(author);

      auto property = properties.find(property_id);
      eosio_assert(property != properties.end(), "Referenced property does not exist");

      floorplans.emplace(author, [&] (auto& row) {
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
    void modfloorplan(account_name author, uint64_t id, uint64_t property_id, string name,
    uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, uint64_t deposit) {
      require_auth(author);

      auto iter = floorplans.find(id);
      eosio_assert(iter != floorplans.end(), "Floor Plan does not exist");

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
    void delfloorplan(account_name author, uint64_t id) {
      require_auth(author);

      auto floorplanidx = flplanimgs.get_index<N(floorplan_id)>();
      auto flplanimg = floorplanidx.find(id);
      eosio_assert(flplanimg == floorplanidx.end(), "Foreign key constrant violation: row referenced on flplanimgs");

      auto iter = floorplans.find(id);
      eosio_assert(iter != floorplans.end(), "Floor Plan does not exist");

      floorplans.erase(iter);
    }

    // FLOOR IMAGE TABLE -------------------------------------------------------

    // @abi action
    void addflplanimg(account_name author, uint64_t floorplan_id, checksum256 image_hash,
      string ipfs_address){
      require_auth(author);

      flplanimgs.emplace(author, [&] (auto& row) {
        row.id           = flplanimgs.available_primary_key();
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void modflplanimg(account_name author, uint64_t id, uint64_t floorplan_id,
      checksum256 image_hash, string ipfs_address) {
      require_auth(author);

      auto iter = flplanimgs.find(id);
      eosio_assert(iter != flplanimgs.end(), "Floor Plan Image does not exist");

      flplanimgs.modify(iter, 0, [&] (auto& row) {
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
      });
    }

    // @abi action
    void delflplanimg(account_name author, uint64_t id) {
      require_auth(author);

      auto iter = flplanimgs.find(id);
      eosio_assert(iter != flplanimgs.end(), "Floor Plan Image does not exist");

      flplanimgs.erase(iter);
    }

    // UNIT TABLE --------------------------------------------------------

    // @abi action
    void addunit(account_name author, uint64_t floorplan_id, uint64_t property_id,
    string name, uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, string status,
    uint64_t date_available){
      require_auth(author);

      units.emplace(author, [&] (auto& row) {
        row.id             = units.available_primary_key();
        row.floorplan_id   = floorplan_id;
        row.property_id    = property_id;
        row.name           = name;
        row.bedrooms       = bedrooms;
        row.bathrooms      = bathrooms;
        row.sq_ft_min      = sq_ft_min;
        row.sq_ft_max      = sq_ft_max;
        row.rent_min       = rent_min;
        row.rent_max       = rent_max;
        row.status         = status;
        row.date_available = date_available;
      });
    }

    // @abi action
    void modunit(account_name author,uint64_t id , uint64_t floorplan_id, uint64_t property_id,
    string name, uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, string status,
    uint64_t date_available) {
      require_auth(author);

      auto iter = units.find(id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      units.modify(iter, 0, [&] (auto& row) {
        row.floorplan_id   = floorplan_id;
        row.property_id    = property_id;
        row.name           = name;
        row.bedrooms       = bedrooms;
        row.bathrooms      = bathrooms;
        row.sq_ft_min      = sq_ft_min;
        row.sq_ft_max      = sq_ft_max;
        row.rent_min       = rent_min;
        row.rent_max       = rent_max;
        row.status         = status;
        row.date_available = date_available;
      });
    }

    // @abi action
    void delunit(account_name author, uint64_t id) {
      require_auth(author);

      auto iter = units.find(id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      auto termpricingidx = termpricings.get_index<N(unit_id)>();
      auto termpricing = termpricingidx.find(id);
      eosio_assert(termpricing == termpricingidx.end(), "Foreign key constrant violation: row referenced on termpricings");

      units.erase(iter);
    }

    // TERM PRICE TABLE -----------------------------------------------------

    // @abi action
    void addtmpricing(account_name author, uint64_t unit_id, uint64_t rent,
    uint64_t term, uint64_t start_date, uint64_t end_date){
      require_auth(author);

      auto iter = units.find(unit_id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      termpricings.emplace(author, [&] (auto& row) {
        row.id         = termpricings.available_primary_key();
        row.unit_id    = unit_id;
        row.rent       = rent;
        row.term       = term;
        row.start_date = start_date;
        row.end_date   = end_date;
      });
    }

    // @abi action
    void modtmpricing(account_name author, uint64_t id, uint64_t unit_id, uint64_t rent,
    uint64_t term, uint64_t start_date, uint64_t end_date) {
      require_auth(author);

      auto tpiter = termpricings.find(id);
      eosio_assert(tpiter != termpricings.end(), "Price term does not exist");

      auto uiter = units.find(unit_id);
      eosio_assert(uiter != units.end(), "Unit does not exist");

      termpricings.modify(tpiter, 0, [&] (auto& row) {
        row.unit_id    = unit_id;
        row.rent       = rent;
        row.term       = term;
        row.start_date = start_date;
        row.end_date   = end_date;
      });
    }

    // @abi action
    void deltmpricing(account_name author, uint64_t id) {
      require_auth(author);

      auto iter = termpricings.find(id);
      eosio_assert(iter != termpricings.end(), "Price term does not exist");

      termpricings.erase(iter);
    }

    // GUEST TABLE -----------------------------------------------------------

    // @abi action
    void addguest(account_name author, string firstname, string lastname, string phone,
    string email, string status) {
      require_auth(author);

      guests.emplace(author, [&] (auto& row) {
        row.id        = guests.available_primary_key();
        row.firstname = firstname;
        row.lastname  = lastname;
        row.phone     = phone;
        row.email     = email;
        row.status    = status;
      });
    }

    // @abi action
    void modguest(account_name author, uint64_t id, string firstname, string lastname,
    string phone, string email, string status) {
      require_auth(author);

      auto iter = guests.find(id);
      eosio_assert(iter != guests.end(), "Guest does not exist");

      guests.modify( iter, 0, [&]( auto& row) {
        row.firstname = firstname;
        row.lastname  = lastname;
        row.phone     = phone;
        row.email     = email;
        row.status    = status;
      });

    }

    // @abi action
    void delguest(account_name author, uint64_t id) {
      require_auth(author);

      auto guestsitr = guests.find(id);
      eosio_assert(guestsitr != guests.end(), "Guest does not exist");

      auto eventidx = events.get_index<N(guest_id)>();
      auto event = eventidx.find(id);
      eosio_assert(event != eventidx.end(), "Foreign key constrant violation: row referenced on events");

      guests.erase(guestsitr);
    }


    // EVENT TABLE --------------------------------------------------------------

    // @abi action
    void addevent(account_name author, uint64_t id_guest, string type, uint64_t date,
    string source, string agent, string comments){
      require_auth(author);

      events.emplace(author, [&] (auto& row) {
        row.id       = events.available_primary_key();
        row.id_guest = id_guest;
        row.type     = type;
        row.date     = date;
        row.source   = source;
        row.agent    = agent;
        row.comments = comments;
      });
    }

    // @abi action
    void modevent(account_name author, uint64_t id, uint64_t id_guest, string type,
    uint64_t date, string source, string agent, string comments) {
      require_auth(author);

      auto iter = events.find(id);
      eosio_assert(iter != events.end(), "Event does not exist");

      events.modify( iter, 0, [&]( auto& row) {
        row.id_guest = id_guest;
        row.type     = type;
        row.date     = date;
        row.source   = source;
        row.agent    = agent;
        row.comments = comments;
      });
    }

    // @abi action
    void delevent(account_name author, uint64_t id) {
      require_auth(author);

      auto iter = events.find(id);
      eosio_assert(iter != events.end(), "Event does not exist");

      events.erase(iter);
    }

  private:
    typedef multi_index<N(property), property> property_index;
    property_index  properties;

    typedef multi_index<N(floorplan), floorplan,
      indexed_by< N(property_id), const_mem_fun<floorplan, uint64_t, &floorplan::by_property>>
      > floorplan_index;
    floorplan_index floorplans;

    typedef multi_index<N(floorplanimg), floorplanimg,
      indexed_by< N(floorplan_id), const_mem_fun<floorplanimg, uint64_t, &floorplanimg::by_floorplan>>
      > flplanimg_index;
    flplanimg_index flplanimgs;

    typedef multi_index<N(unit), unit,
      indexed_by< N(floorplan_id), const_mem_fun<unit, uint64_t, &unit::by_floorplan>>,
      indexed_by< N(property_id), const_mem_fun<unit, uint64_t, &unit::by_property>>
      > unit_index;
    unit_index units;

    typedef multi_index<N(termpricing), termpricing,
      indexed_by< N(unit_id), const_mem_fun<termpricing, uint64_t, &termpricing::by_unit>>
      > termpricing_index;
    termpricing_index termpricings;

    typedef multi_index<N(guest), guest> guest_index;
    guest_index  guests;

    typedef multi_index<N(event), event,
      indexed_by< N(guest_id), const_mem_fun<event, uint64_t, &event::by_guest>>
      > event_index;
    event_index events;

  };

  EOSIO_ABI(fsmanager,(addproperty)(modproperty)(delproperty)(addfloorplan)(modfloorplan)(delfloorplan)(addflplanimg)(modflplanimg)(delflplanimg)(addunit)(modunit)(delunit)(addtmpricing)(modtmpricing)(deltmpricing)(addguest)(modguest)(delguest)(addevent)(modevent)(delevent));
}
