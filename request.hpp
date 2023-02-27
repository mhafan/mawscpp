#ifndef requesthpp
#define requesthpp

//
#include "basics.hpp"
#include "eman.hpp"
#include <curl/curl.h>

// ---------------------------------------------------------------------------
// Rozhrani MAWSCPP - napojeni na moji AWS Lambda -> S3
namespace mawscpp {
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
}

#endif /* requesthpp */