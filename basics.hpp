#ifndef basicshpp
#define basicshpp

//
#include "json.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <random>

// -------------------------------------------------------------------------------
//
namespace mawscpp {
    // ---------------------------------------------------------------------------
    //
    typedef std::string rstring;
    typedef std::vector<rstring> rstring_vec;
    
    // ---------------------------------------------------------------------------
    //
    using json = nlohmann::json;

    // ---------------------------------------------------------------------------
    //
    struct Document {
        //
        rstring version;
        rstring body;
    };

    // ---------------------------------------------------------------------------
    //
    void    to_json(json &jOut, const Document &val);

    // ---------------------------------------------------------------------------
    //
    struct Globals {
        //
        std::mt19937 _R;
        rstring _version = "v1";

        //
        Globals();

        // ---------------------------------------------------------------------------
        //
        rstring     uuid(int len);
        rstring     env(const rstring &envName, const rstring &defValue = "") const;
        
        //
        template<typename TTT>
        rstring     pack(const TTT val) const {
            //
            Document _out;

            //
            _out.version = _version;
            _out.body = json(val).dump();

            //
            return json(_out).dump(3);
        }
    };

    //
    extern Globals _G;
}

#endif /* basics.hpp*/