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
    fsmanager(account_name self):contract(self){}

    // PROPERTY TABLE -----------------------------------------------------------

    // @abi action
    void addproperty(account_name author, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count, uint64_t created_at) {
      require_auth(author);

      property_index properties(_self,author);
      properties.emplace(author, [&] (auto& row) {
        row.id          = properties.available_primary_key();
        row.name        = name;
        row.address_1   = address_1;
        row.address_2   = address_2;
        row.city        = city;
        row.region      = region;
        row.postal_code = postal_code;
        row.unit_count  = unit_count;
        row.created_at  = created_at;
      });
    }

    // @abi action
    void modproperty(account_name author, uint64_t id, string name, string address_1, string address_2,
      string city, string region, string postal_code, uint64_t unit_count, uint64_t created_at) {
      require_auth(author);

      property_index properties(_self,author);
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
        row.created_at  = created_at;
      });
    }

    // @abi action
    void delproperty(account_name author, uint64_t id) {
      require_auth(author);

      // Find all associated units
      unit_index units(_self,author);
      std::vector<uint64_t> keysForDeletion;
      for(auto& item : units) {
        if (item.property_id == id) {
            keysForDeletion.push_back(item.id);   
        }
      }
      
      // Then, get the associated units deleted
      for (uint64_t key : keysForDeletion) {
          auto itr = units.find(key);
          if (itr != units.end()) {
              units.erase(itr);
          }
      }
      
      // Find all associated floorplans
      floorplan_index floorplans(_self,author);
      keysForDeletion.clear();
      for(auto& item : floorplans) {
        if (item.property_id == id) {
            keysForDeletion.push_back(item.id);   
        }
      }
      
      // Then, get the associated floorplans deleted
      for (uint64_t key : keysForDeletion) {
          auto itr = floorplans.find(key);
          if (itr != floorplans.end()) {
              floorplans.erase(itr);
          }
      }

      property_index properties(_self,author);
      auto iter = properties.find(id);
      eosio_assert(iter != properties.end(), "Property does not exist");

      properties.erase(iter);
    }

    // PROPERTY IMAGE TABLE -------------------------------------------------------

    // @abi action
    void addpropimg(account_name author, uint64_t property_id, checksum256 image_hash,
      string ipfs_address, uint64_t created_at){
      require_auth(author);

      propertyimg_index propertyimgs(_self,author);
      propertyimgs.emplace(author, [&] (auto& row) {
        row.id           = propertyimgs.available_primary_key();
        row.property_id = property_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at = created_at;
      });
    }

    // @abi action
    void modpropimg(account_name author, uint64_t id, uint64_t property_id,
      checksum256 image_hash, string ipfs_address, uint64_t created_at) {
      require_auth(author);

      propertyimg_index propertyimgs(_self,author);
      auto iter = propertyimgs.find(id);
      eosio_assert(iter != propertyimgs.end(), "Property Image does not exist");

      propertyimgs.modify(iter, 0, [&] (auto& row) {
        row.property_id = property_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at = created_at;
      });
    }

    // @abi action
    void delpropimg(account_name author, uint64_t id) {
      require_auth(author);

      propertyimg_index propertyimgs(_self,author);
      auto iter = propertyimgs.find(id);
      eosio_assert(iter != propertyimgs.end(), "Property Image does not exist");

      propertyimgs.erase(iter);
    }

    // FLOOR TABLE --------------------------------------------------------------

    // @abi action
    void addfloorplan(account_name author, uint64_t property_id, string name, uint64_t bedrooms,
    uint64_t bathrooms, uint64_t sq_ft_min, uint64_t sq_ft_max, uint64_t rent_min,
    uint64_t rent_max, uint64_t deposit, uint64_t created_at){
      require_auth(author);

      property_index properties(_self,author);
      auto property = properties.find(property_id);
      eosio_assert(property != properties.end(), "Referenced property does not exist");

      floorplan_index floorplans(_self,author);
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
        row.created_at  = created_at;
      });
    }

    // @abi action
    void modfloorplan(account_name author, uint64_t id, uint64_t property_id, string name,
    uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, uint64_t deposit, uint64_t created_at) {
      require_auth(author);

      floorplan_index floorplans(_self,author);
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
        row.created_at  = created_at;
      });
    }

    // @abi action
    void delfloorplan(account_name author, uint64_t id) {
      require_auth(author);

      floorplan_index floorplans(_self,author);
      auto iter = floorplans.find(id);
      eosio_assert(iter != floorplans.end(), "Floor Plan does not exist");

      // At first, find all associated floorplan-img
      flplanimg_index flplanimgs(_self,author);
      std::vector<uint64_t> keysForDeletion;
      for(auto& item : flplanimgs) {
          if (item.floorplan_id == id) {
              keysForDeletion.push_back(item.id);   
          }
      }
      
      // Then, get the associated floorplan-img deleted
      for (uint64_t key : keysForDeletion) {
          auto itr = flplanimgs.find(key);
          if (itr != flplanimgs.end()) {
              flplanimgs.erase(itr);
          }
      }

      floorplans.erase(iter);
    }

    // FLOOR IMAGE TABLE -------------------------------------------------------

    // @abi action
    void addflplanimg(account_name author, uint64_t floorplan_id, checksum256 image_hash,
      string ipfs_address, uint64_t created_at){
      require_auth(author);

      flplanimg_index flplanimgs(_self,author);
      flplanimgs.emplace(author, [&] (auto& row) {
        row.id           = flplanimgs.available_primary_key();
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at = created_at;
      });
    }

    // @abi action
    void modflplanimg(account_name author, uint64_t id, uint64_t floorplan_id,
      checksum256 image_hash, string ipfs_address, uint64_t created_at) {
      require_auth(author);

      flplanimg_index flplanimgs(_self,author);
      auto iter = flplanimgs.find(id);
      eosio_assert(iter != flplanimgs.end(), "Floor Plan Image does not exist");

      flplanimgs.modify(iter, 0, [&] (auto& row) {
        row.floorplan_id = floorplan_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at = created_at;
      });
    }

    // @abi action
    void delflplanimg(account_name author, uint64_t id) {
      require_auth(author);

      flplanimg_index flplanimgs(_self,author);
      auto iter = flplanimgs.find(id);
      eosio_assert(iter != flplanimgs.end(), "Floor Plan Image does not exist");

      flplanimgs.erase(iter);
    }

    // UNIT TABLE --------------------------------------------------------

    // @abi action
    void addunit(account_name author, uint64_t property_id, uint64_t floorplan_id,
    string name, uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, string status,
    uint64_t date_available, uint64_t created_at){
      require_auth(author);

      unit_index units(_self,author);
      units.emplace(author, [&] (auto& row) {
        row.id             = units.available_primary_key();
        row.property_id    = property_id;
        row.floorplan_id   = floorplan_id;
        row.name           = name;
        row.bedrooms       = bedrooms;
        row.bathrooms      = bathrooms;
        row.sq_ft_min      = sq_ft_min;
        row.sq_ft_max      = sq_ft_max;
        row.rent_min       = rent_min;
        row.rent_max       = rent_max;
        row.status         = status;
        row.date_available = date_available;
        row.created_at     = created_at;
      });
    }

    // @abi action
    void modunit(account_name author,uint64_t id, uint64_t property_id, uint64_t floorplan_id,
    string name, uint64_t bedrooms, uint64_t bathrooms, uint64_t sq_ft_min,
    uint64_t sq_ft_max, uint64_t rent_max, uint64_t rent_min, string status,
    uint64_t date_available, uint64_t created_at) {
      require_auth(author);

      unit_index units(_self,author);
      auto iter = units.find(id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      units.modify(iter, 0, [&] (auto& row) {
        row.property_id    = property_id;
        row.floorplan_id   = floorplan_id;
        row.name           = name;
        row.bedrooms       = bedrooms;
        row.bathrooms      = bathrooms;
        row.sq_ft_min      = sq_ft_min;
        row.sq_ft_max      = sq_ft_max;
        row.rent_min       = rent_min;
        row.rent_max       = rent_max;
        row.status         = status;
        row.date_available = date_available;
        row.created_at = created_at;
      });
    }

    // @abi action
    void delunit(account_name author, uint64_t id) {
      require_auth(author);

      unit_index units(_self,author);
      auto iter = units.find(id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      // At first, find all associated unit-img
      unitimg_index unitimgs(_self,author);
      std::vector<uint64_t> keysForDeletion;
      for(auto& item : unitimgs) {
          if (item.unit_id == id) {
              keysForDeletion.push_back(item.id);   
          }
      }
      
      // Then, get the associated unit-img deleted
      for (uint64_t key : keysForDeletion) {
          auto itr = unitimgs.find(key);
          if (itr != unitimgs.end()) {
              unitimgs.erase(itr);
          }
      }

      // Repeat the same for associated termpricing
      // At first, find all associated termpricing
      termpricing_index termpricings(_self,author);
      keysForDeletion.clear();
      for(auto& item : termpricings) {
          if (item.unit_id == id) {
              keysForDeletion.push_back(item.id);   
          }
      }
      
      // Then, get the associated termpricing deleted
      for (uint64_t key : keysForDeletion) {
          auto itr = termpricings.find(key);
          if (itr != termpricings.end()) {
              termpricings.erase(itr);
          }
      }

      units.erase(iter);
    }

    // TERM PRICE TABLE -----------------------------------------------------

    // @abi action
    void addtmpricing(account_name author, uint64_t unit_id, uint64_t rent,
    uint64_t term, uint64_t start_date, uint64_t end_date, uint64_t created_at){
      require_auth(author);

      unit_index units(_self,author);
      auto iter = units.find(unit_id);
      eosio_assert(iter != units.end(), "Unit does not exist");

      termpricing_index termpricings(_self,author);
      termpricings.emplace(author, [&] (auto& row) {
        row.id         = termpricings.available_primary_key();
        row.unit_id    = unit_id;
        row.rent       = rent;
        row.term       = term;
        row.start_date = start_date;
        row.end_date   = end_date;
        row.created_at = created_at;
      });
    }

    // @abi action
    void modtmpricing(account_name author, uint64_t id, uint64_t unit_id, uint64_t rent,
    uint64_t term, uint64_t start_date, uint64_t end_date, uint64_t created_at) {
      require_auth(author);

      termpricing_index termpricings(_self,author);
      auto tpiter = termpricings.find(id);
      eosio_assert(tpiter != termpricings.end(), "Price term does not exist");

      unit_index units(_self,author);
      auto uiter = units.find(unit_id);
      eosio_assert(uiter != units.end(), "Unit does not exist");

      termpricings.modify(tpiter, 0, [&] (auto& row) {
        row.unit_id    = unit_id;
        row.rent       = rent;
        row.term       = term;
        row.start_date = start_date;
        row.end_date   = end_date;
        row.created_at = created_at;
      });
    }

    // @abi action
    void deltmpricing(account_name author, uint64_t id) {
      require_auth(author);
      
      termpricing_index termpricings(_self,author);
      auto iter = termpricings.find(id);
      eosio_assert(iter != termpricings.end(), "Price term does not exist");

      termpricings.erase(iter);
    }

    // UNIT IMG TABLE -----------------------------------------------------

    // @abi action
    void addunitimg(account_name author, uint64_t unit_id, checksum256 image_hash,
      string ipfs_address, uint64_t created_at){
      require_auth(author);

      unitimg_index unitimgs(_self,author);
      unitimgs.emplace(author, [&] (auto& row) {
        row.id           = unitimgs.available_primary_key();
        row.unit_id = unit_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at   = created_at;
      });
    }

    // @abi action
    void modunitimg(account_name author, uint64_t id, uint64_t unit_id,
      checksum256 image_hash, string ipfs_address, uint64_t created_at) {
      require_auth(author);

      unitimg_index unitimgs(_self,author);
      auto iter = unitimgs.find(id);
      eosio_assert(iter != unitimgs.end(), "Floor Plan Image does not exist");

      unitimgs.modify(iter, 0, [&] (auto& row) {
        row.unit_id = unit_id;
        row.image_hash   = image_hash;
        row.ipfs_address = ipfs_address;
        row.created_at = created_at;
      });
    }

    // @abi action
    void delunitimg(account_name author, uint64_t id) {
      require_auth(author);

      unitimg_index unitimgs(_self,author);
      auto iter = unitimgs.find(id);
      eosio_assert(iter != unitimgs.end(), "Floor Plan Image does not exist");

      unitimgs.erase(iter);
    }

    // GUEST TABLE -----------------------------------------------------------

    // @abi action
    void addguest(account_name author, string firstname, string lastname, string phone,
    string email, string status) {
      require_auth(author);

      guest_index guests(_self,author);
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

      guest_index guests(_self,author);
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

      guest_index guests(_self,author);      
      auto guestsitr = guests.find(id);
      eosio_assert(guestsitr != guests.end(), "Guest does not exist");

      event_index events(_self,author);
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

      event_index events(_self,author);
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

      event_index events(_self,author);
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

      event_index events(_self,author);
      auto iter = events.find(id);
      eosio_assert(iter != events.end(), "Event does not exist");

      events.erase(iter);
    }

  private:
    typedef multi_index<N(property), property> property_index;    

    typedef multi_index<N(propertyimg), propertyimg,
      indexed_by< N(property_id), const_mem_fun<propertyimg, uint64_t, &propertyimg::by_property>>
      > propertyimg_index;

    typedef multi_index<N(floorplan), floorplan,
      indexed_by< N(property_id), const_mem_fun<floorplan, uint64_t, &floorplan::by_property>>
      > floorplan_index;

    typedef multi_index<N(floorplanimg), floorplanimg,
      indexed_by< N(floorplan_id), const_mem_fun<floorplanimg, uint64_t, &floorplanimg::by_floorplan>>
      > flplanimg_index;

    typedef multi_index<N(unit), unit,
      indexed_by< N(property_id), const_mem_fun<unit, uint64_t, &unit::by_property>>,
      indexed_by< N(floorplan_id), const_mem_fun<unit, uint64_t, &unit::by_floorplan>>
      > unit_index;

    typedef multi_index<N(termpricing), termpricing,
      indexed_by< N(unit_id), const_mem_fun<termpricing, uint64_t, &termpricing::by_unit>>
      > termpricing_index;    

    typedef multi_index<N(unitimg), unitimg,
      indexed_by< N(unit_id), const_mem_fun<unitimg, uint64_t, &unitimg::by_unit>>
      > unitimg_index;

    typedef multi_index<N(guest), guest> guest_index;

    typedef multi_index<N(event), event,
      indexed_by< N(guest_id), const_mem_fun<event, uint64_t, &event::by_guest>>
      > event_index;
  };

  EOSIO_ABI(fsmanager,(addproperty)(modproperty)(delproperty)(addpropimg)(modpropimg)(delpropimg)(addfloorplan)(modfloorplan)(delfloorplan)(addflplanimg)(modflplanimg)(delflplanimg)(addunit)(modunit)(delunit)(addtmpricing)(modtmpricing)(deltmpricing)(addunitimg)(modunitimg)(delunitimg)(addguest)(modguest)(delguest)(addevent)(modevent)(delevent));
}
