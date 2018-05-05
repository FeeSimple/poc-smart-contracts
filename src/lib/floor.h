using namespace eosio;
using namespace std;

namespace floor{

  // @abi table
  struct Floors {
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

    EOSLIB_SERIALIZE(Floors, (id)(property_id)(name)(bedrooms)(bathrooms)(sq_ft_min)(sq_ft_max)(rent_min)(rent_max)(deposit));
  };

  // @abi table
  struct FloorImages {
    uint64_t id;
    uint64_t floor_id;

    checksum256 hash;
    string ipfs_address;

    auto primary_key()const { return id; }

    EOSLIB_SERIALIZE(FloorImages, (id)(floor_id)(hash)(ipfs_address));
  };

}
