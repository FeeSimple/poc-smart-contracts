using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct property {
      uint64_t id;

      string name;
      string address_1;
      string address_2;
      string city;
      string region;
      string postal_code;
      uint64_t unit_count;

      auto primary_key()const { return id; }

      EOSLIB_SERIALIZE(property, (id)(name)(address_1)(address_2)(city)(region)(postal_code)(unit_count));
  };

  // @abi table
  struct propertyimg {
    uint64_t id;
    uint64_t property_id;

    checksum256 image_hash;
    string ipfs_address;

    auto primary_key()const { return id; }
    uint64_t by_property()const { return property_id; }

    EOSLIB_SERIALIZE(propertyimg, (id)(property_id)(image_hash)(ipfs_address));
  };

}
