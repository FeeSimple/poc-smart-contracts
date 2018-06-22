using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct dapp {
      uint64_t id;

      string name;
      string category;
      string account;

      auto primary_key()const { return id; }

      EOSLIB_SERIALIZE(dapp, (id)(name)(category)(account));
  };

}
