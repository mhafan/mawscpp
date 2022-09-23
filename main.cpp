#include "mawscpp.h"

 
int main(void)
{
    //
    mawscpp::S3Export _e;
    mawscpp::Connection _cn;

    //
    _e.project = "pokus";
    _e.name = "jjj";
    _e.content = "1,2,3";

    //
    if (_cn.post(_e.json()) == false) {
        //
        printf("Dosralo se to\n");
    }

    //
    return 0;
}
