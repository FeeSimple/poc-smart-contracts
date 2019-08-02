using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct floorplan {
    uint64_t id;
    uint64_t property_id;

    string name;
    uint64_t bedrooms;
    uint64_t bathrooms;
    uint64_t sq_ft_min;
    uint64_t sq_ft_max;
    uint64_t rent_min;
    uint64_t rent_max;
    uint64_t deposit;
    uint64_t created_at; //unix timestamp

    auto primary_key()const { return id; }
    uint64_t by_property()const { return property_id; }

    EOSLIB_SERIALIZE(floorplan, (id)(property_id)(name)(bedrooms)(bathrooms)(sq_ft_min)(sq_ft_max)(rent_min)(rent_max)(deposit)(created_at));
  };

  // @abi table
  struct floorplanimg {
    uint64_t id;
    uint64_t floorplan_id;
    checksum256 image_hash;
    string ipfs_address;
    uint64_t created_at; //unix timestamp

    auto primary_key()const { return id; }
    uint64_t by_floorplan()const { return floorplan_id; }

    EOSLIB_SERIALIZE(floorplanimg, (id)(floorplan_id)(image_hash)(ipfs_address)(created_at));
  };

}
