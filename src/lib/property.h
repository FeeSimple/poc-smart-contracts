using namespace eosio;
using namespace std;

namespace property{

  // @abi table
  struct Properties {
      uint64_t id;

      string name;
      string address_1;
      string address_2;
      string city;
      string region;
      string postal_code;
      uint64_t unit_count;

      auto primary_key()const { return id; }

      EOSLIB_SERIALIZE(Properties, (id)(name)(address_1)(address_2)(city)(region)(postal_code)(unit_count));
  };

}
