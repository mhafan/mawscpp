#include <mawsgocpp.hpp>

// -------------------------------------------------------------------------------
//
using namespace mawscpp;

mawscpp::Globals mawscpp::_G;

//
int main() {
    //
    EManCFG cfg;

    cfg.defaultS3Bucket = "mawsgo";
    cfg.projectID = "cpp-testing";

    //
    EMan eman(cfg);

    //
    eman().experimentID = "uzfunguje-2";

    //
    eman.addFile(EManFile("pes.csv", "tralaala,1,koza"));

    //
    if (eman.post() == false) {
        //
        printf("Nejaky errror\n");
    }

    //
    return 0;
}