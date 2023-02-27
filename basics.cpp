#include "basics.hpp"

//
namespace mawscpp {
    // ---------------------------------------------------------------------------
    //
    Globals _G;

    //
    void    to_json(json &jOut, const Document &val) {
        //
        jOut = json {
            { "version", val.version },
            { "body", val.body }
        };
    }

    // ---------------------------------------------------------------------------
    //
    Globals::Globals() {
        //
        auto _rndseed = time(nullptr);
    
        //
        _R.seed((std::mt19937::result_type) _rndseed);
    }

    // ---------------------------------------------------------------------------
    //
    rstring Globals::env(const rstring &envName, const rstring &defValue) const {
        //
        auto _get = std::getenv(envName.c_str());

        //
        if (_get == nullptr) {
            //
            return defValue;
        }

        //
        return rstring(_get);
    }

    // ---------------------------------------------------------------------------
    rstring Globals::uuid(int len) {
        //
        std::stringstream _str;
        std::uniform_int_distribution<> _d(0, 255);
        
        //
        for (int base = 0; base < len; base++) {
            //
            for (int nn = 0; nn < 2; nn++) {
                //
                auto _nn = _d(_R);
                std::stringstream _nnf;
                
                //
                _nnf << std::hex << _nn;
                
                //
                if (_nnf.str().length() < 2) {
                    //
                    _str << "0" << _nnf.str();
                } else {
                    //
                    _str << _nnf.str();
                }
            }
            
            //
            if (base + 1 < len) {
                //
                _str << "-";
            }
        }
        
        //
        return _str.str();
    }
}