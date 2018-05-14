using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct guest {
      uint64_t id;

      string firstname;
      string lastname;
      string phone;
      string email;
      string status;

      auto primary_key()const { return id; }

      EOSLIB_SERIALIZE(guest,(id)(firstname)(lastname)(phone)(email)(status));
  };

}
