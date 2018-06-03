#include <iostream>
using namespace std;
#include "str_aux.h"


// ASCII: '\t'=9, '\n'=10, '\r'=13, ' '=32
const bool StrAux::sm_fastStripTb[128] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  // 0
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 11
    0, 0, 0, 0, 0, 0, 0, 0         // 12
};


std::string
StrAux::strip(const std::string& origStr)
{
    size_t len = origStr.size();
    int stripStart = 0;
    int stripEnd = len - 1;
    for(int i = 0; i < len; i++) {
        stripStart = i;
        if(!sm_fastStripTb[origStr[i]]) {
            break;
        }
    }
    for(int i = len - 1; i >= 0; i--) {
        stripEnd = i;
        if(!sm_fastStripTb[origStr[i]]) {
            break;
        }
    }
    if(stripStart >= stripEnd) {
        return "";
    }
    return origStr.substr(stripStart, stripEnd - stripStart + 1);
}


bool
StrAux::split(const std::string& origStr,
              const std::string& delim,
              std::vector<std::string>& output)
{
    const size_t origStrLen = origStr.size();
    const size_t delimLen = delim.size();
    if(origStrLen == 0 || delimLen == 0) {
        output.push_back(origStr);
        return true;
    }

    size_t index = origStr.find(delim);
    if(index == std::string::npos) {
        output.push_back(origStr);
        return true;
    }
    output.push_back(origStr.substr(0, index));
    size_t preIndex = index;

    while(1) {
        index = origStr.find(delim, preIndex + delimLen);
        if(index == std::string::npos) {
            break;
        }
        output.push_back(origStr.substr(preIndex + delimLen,
                                        index - preIndex - delimLen));
        preIndex = index;
    }
    output.push_back(origStr.substr(preIndex + delimLen,
                                    origStrLen - preIndex - delimLen));
    return true;
}
