using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct floor {
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

    auto primary_key()const { return id; }

    EOSLIB_SERIALIZE(floor, (id)(property_id)(name)(bedrooms)(bathrooms)(sq_ft_min)(sq_ft_max)(rent_min)(rent_max)(deposit));
  };

  // @abi table
  struct floorimage {
    uint64_t id;
    uint64_t floor_id;

    checksum256 hash;
    string ipfs_address;

    auto primary_key()const { return id; }

    EOSLIB_SERIALIZE(floorimage, (id)(floor_id)(hash)(ipfs_address));
  };

}
