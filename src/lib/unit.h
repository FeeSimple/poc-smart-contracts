using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct unit {
    uint64_t id;
    uint64_t property_id;
    uint64_t floorplan_id;

    string name;
    uint64_t bedrooms;
    uint64_t bathrooms;
    uint64_t sq_ft_min;
    uint64_t sq_ft_max;
    uint64_t rent_min;
    uint64_t rent_max;
    string status;
    uint64_t date_available; //unix timestamp
    uint64_t created_at; //unix timestamp

    auto primary_key()const { return id; }
    uint64_t by_property()const { return property_id; }
    uint64_t by_floorplan()const { return floorplan_id; }

    EOSLIB_SERIALIZE(unit, (id)(property_id)(name)(bedrooms)(bathrooms)(sq_ft_min)(sq_ft_max)(rent_min)(rent_max)(status)(date_available)(created_at));
  };

  // @abi table
  struct termpricing {
    uint64_t id;
    uint64_t unit_id;
    
    // This is inserted as a dummy element so that the generated ABI
    // won't treat the "rent" as an unexpected foreign key
    string dummy; 
    
    uint64_t rent;
    uint64_t term;
    uint64_t start_date; //unix timestamp
    uint64_t end_date;   //unix timestamp
    uint64_t created_at; //unix timestamp


    auto primary_key()const { return id; }
    uint64_t by_unit()const { return unit_id; }

    EOSLIB_SERIALIZE(termpricing, (id)(unit_id)(dummy)(rent)(term)(start_date)(end_date)(created_at));
  };

  // @abi table
  struct unitimg {
    uint64_t id;
    uint64_t unit_id;
    uint64_t created_at; //unix timestamp

    checksum256 image_hash;
    string ipfs_address;

    auto primary_key()const { return id; }
    uint64_t by_unit()const { return unit_id; }

    EOSLIB_SERIALIZE(unitimg, (id)(unit_id)(image_hash)(ipfs_address)(created_at));
  };

}
