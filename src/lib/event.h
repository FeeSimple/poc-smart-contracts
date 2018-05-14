using namespace eosio;
using namespace std;

namespace feesimple{

  // @abi table
  struct event {
      uint64_t id;
      uint64_t id_guest;

      string type;
      uint64_t date;
      string source;
      string agent;
      string comments;

      auto primary_key()const { return id; }
      uint64_t by_guest()const { return id_guest; }

      EOSLIB_SERIALIZE(event, (id)(id_guest)(type)(date)(source)(agent)(comments));
  };

}
