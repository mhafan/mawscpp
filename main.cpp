//
#include "eman.hpp"
#include "request.hpp"

// -------------------------------------------------------------------------------
//
using namespace mawscpp;
 
// -------------------------------------------------------------------------------
//
int main(void) {
    //
    EManCFG cfg;

    cfg.defaultS3Bucket = "mawsgo";
    cfg.projectID = "cpp-testing";

    //
    EMan eman(cfg);

    //
    eman._j().experimentID = "uzfunguje";

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
