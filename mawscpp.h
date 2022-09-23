
#include <stdio.h>
#include <curl/curl.h>

#include <string>
#include <sstream>
#include <vector>

// ---------------------------------------------------------------------------
// Rozhrani MAWSCPP - napojeni na moji AWS Lambda -> S3
namespace mawscpp {
    // -----------------------------------------------------------------------
    //
    typedef std::string rstring;
    typedef std::vector<rstring> rstring_vec;

    // -----------------------------------------------------------------------
    // prebira ze systemovych promennych
    // MAWSGO_EXP_URL
    // MAWSGO_EXP_KEY
    // -----------------------------------------------------------------------
    struct Connection {
        //
        rstring url;
        rstring appKey;

        // -------------------------------------------------------------------
        //
        Connection() {
            //
            auto _url = std::getenv("MAWSGO_EXP_URL");
            auto _key = std::getenv("MAWSGO_EXP_KEY");

            //
            if (_url != nullptr) {
                //
                url = rstring(_url);
            }

            //
            if (_key != nullptr) {
                //
                appKey = rstring(_key);
            }
        }

        // -------------------------------------------------------------------
        //
        Connection(const rstring &anURL, const rstring &anKey) : url(anURL), appKey(anKey) {}

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

    // -----------------------------------------------------------------------
    //
    template<typename COSI>
    rstring __senco(const COSI &ins, bool apos = false) {
        //
        std::stringstream _out;

        //
        rstring _apos = (apos) ?  "\\\"" : "\"";

        //
        _out << _apos << ins << _apos;

        //
        return _out.str();
    }

    // -----------------------------------------------------------------------
    //
    template<typename COSI>
    rstring __jsonEnco(const rstring &key, const COSI &ins, bool apos = false) {
        //
        std::stringstream _out;

        //
        _out << __senco(key, apos) << ": " << __senco(ins, apos); 

        //
        return _out.str();
    }

    // -----------------------------------------------------------------------
    //
    rstring __json(const rstring_vec &inv) {
        //
        std::stringstream _out;
        auto _it = inv.begin();
        
        //
        _out << "{";
        //
        while (_it != inv.end()) {
            //
            _out << *_it;
            _it++;

            //
            if (_it != inv.end()) {
                //
                _out << ",";
            }
        }

        //
        _out << "}";

        //
        return _out.str();
    }

    // -----------------------------------------------------------------------
    //
    rstring __embr(const rstring &ee) {
        //
        std::stringstream _sout;

        //
        return _sout.str();
    }

    // -----------------------------------------------------------------------
    // Zakladni udalost nasi mile Lambdy
    struct S3Export {
        //
        rstring project;
        rstring prefix;
        rstring name;
        rstring content;

        //
        rstring json() const {
            //
            return __json({
                //
                __jsonEnco("Project", project),
                __jsonEnco("Prefix", prefix),
                __jsonEnco("Name", name),
                __jsonEnco("Content", content),
            });
        }
    };
}
