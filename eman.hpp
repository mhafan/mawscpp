#ifndef manhpp
#define manhpp

//
#include "basics.hpp"

// -------------------------------------------------------------------------------
// Definice modelu:
// -------------------------------------------------------------------------------
// Projekt
// - globalne vytvarim nejaky "projekt"
// - jeho vystupy chci smerovat do uloziste
// -------------------------------------------------------------------------------
// Experiment
// - je vystup v ramci projektu
namespace mawscpp {
    // ---------------------------------------------------------------------------
    // na techto ENV ocekavam spojeni na EGate
    const rstring ENV_EGATE_ENDPOINT = "ENV_EGATE_ENDPOINT";
    const rstring ENV_EGATE_APIKEY = "ENV_EGATE_APIKEY";

    // ---------------------------------------------------------------------------
    // Konfigurace manazera experimentu
    struct EManCFG {
        // -----------------------------------------------------------------------
        // Napojeni do uloziste
        rstring APIEndPoint;
        rstring APIKey;

        // -----------------------------------------------------------------------
        // Projekt
        rstring projectID;

        //
        rstring defaultS3Bucket;

        //
        EManCFG();
    };

    // ---------------------------------------------------------------------------
    //
    struct EManFile {
        // -----------------------------------------------------------------------
        //
        rstring bucket;
        rstring filename;
        rstring textContent;

        // -----------------------------------------------------------------------
        //
        EManFile(const rstring &fn, const rstring &tc) : filename(fn), textContent(tc) {}
        EManFile(const rstring &buk, const rstring &fn, const rstring &tc) : filename(fn), textContent(tc), bucket(buk) {}
    };

    //
    typedef std::vector<EManFile> EManFiles;

    // ---------------------------------------------------------------------------
    //
    struct EManJSON {
        // -----------------------------------------------------------------------
        // Identifikace okolnosti
        // - je to v ramci nejakeho projektu
        // - experiment ma pojmenovani
        // - je indexovany (volitelne)
        rstring     projectID;
        rstring     experimentID;
        int         experimentIndex = -1;

        //
        rstring     cmd = "store";

        // -----------------------------------------------------------------------
        //
        EManFiles   files;
    };

    // ---------------------------------------------------------------------------
    //
    void    to_json(json &jOut, const EManJSON &val);
    void    to_json(json &jOut, const EManFile &val);

    // ---------------------------------------------------------------------------
    // Manazer experimentu se zadanou konfiguraci
    class EMan {
        // -----------------------------------------------------------------------
        //
        EManCFG     cfg;
        EManJSON    _json;
        //
    public:
        // -----------------------------------------------------------------------
        //
        EMan(const EManCFG &ecfg);

        // -----------------------------------------------------------------------
        //
        inline EManJSON &_j() { return _json; }
        inline const EManJSON &j() const { return _json; }

        // -----------------------------------------------------------------------
        //
        void    experimentStarted();
        void    experimentEnded();

        // -----------------------------------------------------------------------
        // files
        void    addFile(const EManFile &fl);

        // -----------------------------------------------------------------------
        //
        rstring meJSON() { return json(j()).dump(3); }
        bool    post();
    };
}

#endif /* manhpp */