
#include <stdio.h>
#include <curl/curl.h>

#include <string>
#include <sstream>
#include <vector>

// ---------------------------------------------------------------------------
//
namespace mawscpp {
    // -----------------------------------------------------------------------
    //
    typedef std::string rstring;
    typedef std::vector<rstring> rstring_vec;

    // -----------------------------------------------------------------------
    // prebira ze systemovych promennych
    // MAWSGO_EXP_URL
    // MAWSGO_EXP_KEY
    struct Connection {
        //
        rstring url;
        rstring appKey;

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

        //
        Connection(const rstring &anURL, const rstring &anKey) : url(anURL), appKey(anKey) {}

        //
        bool    post(const rstring &body) const  {
        //
        bool retCode = false;
        CURL *curl = nullptr;
        CURLcode res = CURLE_FAILED_INIT;
        struct curl_slist *headers = NULL;
        char errbuf[CURL_ERROR_SIZE] = { 0, };
        long response_code = 0;
        FILE *devnull = fopen("/dev/null", "w+");

        //
        auto _xapi = rstring("x-api-key: ") + appKey;

        //
        curl = curl_easy_init();
        if (curl == nullptr) {
            //
            goto cleanup;
        }

        //
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, devnull);

        //
        headers = curl_slist_append(headers, "Expect:");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, _xapi.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        //
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);

        //
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        //
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        //
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            //
            goto cleanup;
        }

        //
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        //
        retCode = (response_code == 200);

        //
        cleanup:
        //
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        fclose(devnull);
    
        //
        return retCode;
    }
    };

    // -----------------------------------------------------------------------
    //
    template<typename COSI>
    rstring __senco(const COSI &ins) {
        //
        std::stringstream _out;

        //
        _out << "\"" << ins << "\"";

        //
        return _out.str();
    }

    // -----------------------------------------------------------------------
    //
    template<typename COSI>
    rstring __jsonEnco(const rstring &key, const COSI &ins) {
        //
        std::stringstream _out;

        //
        _out << __senco(key) << ": " << __senco(ins); 

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
