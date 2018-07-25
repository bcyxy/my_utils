#include <unistd.h>
#include <stdio.h>
#include "dyn_config.h"

int main()
{
    vector<string> confFilePaths;
    confFilePaths.push_back("./test.conf");
    Config::getInstance().init(confFilePaths);

    pthread_t threadHandle;
    pthread_create(&threadHandle, NULL, confThreadFunc, NULL);

    sleep(2);

    ConfItem* pConfItem = Config::getInstance().getConfItemPtr("aaa.str_test");
    if(pConfItem != NULL) {
        printf("%s\n", pConfItem->getValStr("NULL").c_str());
    }

    pthread_join(threadHandle, NULL);
    
    return 0;
}
