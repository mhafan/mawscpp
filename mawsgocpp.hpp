// -------------------------------------------------------------------------------
// Knihovna mhafan-AWS-cpp podpory pro moje simulacni vypocty (unor 23)
// - napojeni na AWS
// -------------------------------------------------------------------------------
#ifndef mawsgocpp__hpp
#define mawsgocpp__hpp

// -------------------------------------------------------------------------------
// nejaky ten c++ zaklad, ze ano...
#include <sstream>
#include <string>
#include <vector>
#include <random>

// abych mohl delat ty sitove sracky
// pak nutno dal -lcurl do linkovani vysledku
#include <curl/curl.h>

// prebiram knihovnu na JSON zpracovani
#include "json.hpp"

// -------------------------------------------------------------------------------
// mhafan-AWS-cpp
namespace mawscpp {
    // ---------------------------------------------------------------------------
    // rstring je moje klasika
    typedef std::string rstring;
    //typedef std::vector<rstring> rstring_vec;
    
    // ---------------------------------------------------------------------------
    // pro jednoduchost si interne zjednodusim namespan na knihovnu JSON
    using json = nlohmann::json;

    // ---------------------------------------------------------------------------
    // TODO: zatim nepouzivano. Smyslem je zapouzdrit zpravu do Lambdy.
    // - verze dokumentu
    // - JSON kod tela dokumentu
    // - lambda - cilova lambda nebo jeji fronta
    struct Document {
        //
        rstring version;
        rstring body;
        rstring lambda;
    };

    // ---------------------------------------------------------------------------
    // export struktury Document -> 
    void    to_json(json &jOut, const Document &val) {
        //
        jOut = json {
            { "version", val.version },
            { "body", val.body }, 
            { "lambda", val.lambda }
        };
    }

    // ---------------------------------------------------------------------------
    // !!! Je treba mit nejaky vnitrni stav v teto knihovne, tj. singleton instanci 
    // teto tridy zapouzdrujici vsechna globalni data knihovny.
    // ---------------------------------------------------------------------------
    // V navaznem kodu pak:
    // mawscpp::Globals mawscpp::_G;
    struct Globals {
        // -----------------------------------------------------------------------
        // generator pseudonahodnosti
        std::mt19937 _R;
        // toto by spis melo byt konst
        rstring _version = "v1";

        // -----------------------------------------------------------------------
        // Inicializuj globalni datovy segment knihovny
        Globals() {
            // RND seed pro RND gen
            _R.seed((std::mt19937::result_type) time(nullptr));
        }

        // ---------------------------------------------------------------------------
        //
        rstring     uuid(int len = 8) {
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


        // ---------------------------------------------------------------------------
        // Abstrakce nad system ENVs
        rstring     env(const rstring &envName, const rstring &defValue = "") const  {
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
        // TODO: az budu pouzivat Document
        template<typename TTT> rstring     pack(const TTT val) const {
            //
            Document _out { _version, json(val).dump(), "lambda" };

            //
            return json(_out).dump(3);
        }
    };

    // ---------------------------------------------------------------------------
    // !!! Je treba mit nejaky vnitrni stav v teto knihovne, tj. singleton instanci 
    // teto tridy zapouzdrujici vsechna globalni data knihovny.
    // ---------------------------------------------------------------------------
    // V navaznem kodu pak:
    // mawscpp::Globals mawscpp::_G;
    extern Globals _G;

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
        EManCFG() {
            //
            APIEndPoint = _G.env(ENV_EGATE_ENDPOINT);
            APIKey = _G.env(ENV_EGATE_APIKEY);
        }
    };

    // -----------------------------------------------------------------------
    // prebira ze systemovych promennych
    // MAWSGO_EXP_URL
    // MAWSGO_EXP_KEY
    // -----------------------------------------------------------------------
    struct Connection {
        //
        rstring url, appKey;

        // -------------------------------------------------------------------
        //
        Connection(const rstring &anURL, const rstring &anKey) : url(anURL), appKey(anKey) {}

        // -------------------------------------------------------------------
        //
        Connection(const EManCFG &cfg) : url(cfg.APIEndPoint), appKey(cfg.APIKey) {}

        // -------------------------------------------------------------------
        // HTTP POST na url+apiKey
        bool    post(const rstring &body) const  {
            // ---------------------------------------------------------------
            //
            bool retCode = false;
            CURL *curl = nullptr;
            CURLcode res = CURLE_FAILED_INIT;
            struct curl_slist *headers = NULL;
            char errbuf[CURL_ERROR_SIZE] = { 0, };
            long response_code = 0;

            // ---------------------------------------------------------------
            // toto se prdne do hlavicky dotazu
            auto _xapi = rstring("x-api-key: ") + appKey;

            // ---------------------------------------------------------------
            // nastartuj kontext dotazu
            curl = curl_easy_init();
            if (curl == nullptr) {
                //
                goto cleanup;
            }

            // ---------------------------------------------------------------
            // beztak to pise na obrazovku. Tady nutno rict, ze vsechny me
            // pokusy potlacit vypisy knihovny curl na obrazovku "zlyhaly".
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

            // ---------------------------------------------------------------
            // hlavicka
            headers = curl_slist_append(headers, "Expect:");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, _xapi.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // ---------------------------------------------------------------
            // telicko
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);

            // kam to pujde
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // ---------------------------------------------------------------
            // proved tu srandu
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                //
                goto cleanup;
            }

            // ---------------------------------------------------------------
            // zjisteni vysledneho http response kodu
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            //
            retCode = (response_code == 200);

            //
            cleanup:
            //
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        
            //
            return retCode;
        }
    };

    // ---------------------------------------------------------------------------
    // Struktura pro preneseni souboru
    struct EManFile {
        // -----------------------------------------------------------------------
        // s3 bucket, nazev, obsah
        // -> ulozi se na misto dane popisem experimentu
        rstring bucket;
        rstring filename;
        rstring textContent;

        // -----------------------------------------------------------------------
        //
        EManFile(const rstring &fn, const rstring &tc) : filename(fn), textContent(tc) {}
        EManFile(const rstring &buk, const rstring &fn, 
                 const rstring &tc) : filename(fn), textContent(tc), bucket(buk) {}
    };

    //
    typedef std::vector<EManFile> EManFiles;

    // ---------------------------------------------------------------------------
    // Struktura pro popis vysledku experimentu
    struct EManJSON {
        // -----------------------------------------------------------------------
        // Identifikace okolnosti
        // - je to v ramci nejakeho projektu
        // - experiment ma pojmenovani
        // - je indexovany (volitelne)
        rstring     projectID;
        rstring     experimentID;
        int         experimentIndex = -1;

        // -----------------------------------------------------------------------
        // pokyn do cilove Lambdy
        // - store - uloz vysledek
        rstring     cmd = "store";

        // -----------------------------------------------------------------------
        // attachments: soubory
        EManFiles   files;
    };

    // ---------------------------------------------------------------------------
    // Export do JSON
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
    // Export do JSON
    void    to_json(json &jOut, const EManJSON &val) {
        //
        jOut = json {
            { "projectID", val.projectID },
            { "experimentID", val.experimentID },
            { "experimentIndex", val.experimentIndex },
            { "cmd", val.cmd },
            { "files", val.files },
        };
    }

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
        // ...
        EMan(const EManCFG &ecfg) : cfg(ecfg) {
            // implicitne zavadim jmeno experimentu, lze prepsat
            _json.experimentID = _G.uuid(8);
            _json.projectID = cfg.projectID;

            //
            experimentStarted();
        }

        // -----------------------------------------------------------------------
        // getters - mutable/immutable
        inline EManJSON &_j() { return _json; }
        inline EManJSON &operator()() { return _json; }
        inline const EManJSON &j() const { return _json; }

        // -----------------------------------------------------------------------
        // TODO: sem nejake statistiky o experimentu
        // - doba behu apod
        // - host...
        void    experimentStarted() {}
        void    experimentEnded() {}

        // -----------------------------------------------------------------------
        // files
        void    addFile(const EManFile &fl) {
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

        // -----------------------------------------------------------------------
        // experiment exportovany do JSON
        rstring meJSON() const { return json(j()).dump(3); }

        // -----------------------------------------------------------------------
        // odesli vysledek
        bool    post() const{
            //
            return Connection(cfg).post(meJSON());
        }
    };
}



#endif /* mawsgocpp__hpp */