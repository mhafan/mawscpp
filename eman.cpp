#include "eman.hpp"
#include "request.hpp"

namespace mawscpp {
    // ---------------------------------------------------------------------------
    //
    EManCFG::EManCFG() {
        //
        APIEndPoint = _G.env(ENV_EGATE_ENDPOINT);
        APIKey = _G.env(ENV_EGATE_APIKEY);
    }

    // ---------------------------------------------------------------------------
    //
    void    to_json(json &jOut, const EManJSON &val) {
        //
        jOut = json {
            { "projectID", val.projectID },
            { "experimentID", val.experimentID },
            { "experimentIndex", val.experimentIndex },
            { "cmd", val.cmd },
            { "files", val.files }
        };
    }

    // ---------------------------------------------------------------------------
    void    to_json(json &jOut, const EManFile &val) {
        //
        jOut = json {
            //
            { "bucket", val.bucket },
            { "filename", val.filename },
            { "textContent", val.textContent }
        };
    }

    // ---------------------------------------------------------------------------
    //
    EMan::EMan(const EManCFG &ecfg) : cfg(ecfg) {
        //
        _json.experimentID = _G.uuid(8);

        //
        experimentStarted();
    }

    // ---------------------------------------------------------------------------
    //
    void    EMan::experimentStarted() {
        //
        _json.projectID = cfg.projectID;
    }

    // ---------------------------------------------------------------------------
    //
    void    EMan::experimentEnded() {
        //
    }

    // ---------------------------------------------------------------------------
    //
    void    EMan::addFile(const EManFile &fl) {
        //
        EManFile _wc = fl;

        //
        if (_wc.bucket.empty()) {
            //
            _wc.bucket = cfg.defaultS3Bucket;
        }

        //
        _json.files.push_back(_wc);
    }

    // ---------------------------------------------------------------------------
    //
    bool    EMan::post() {
        //
        return Connection(cfg).post(meJSON());
    }
}