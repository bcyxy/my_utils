#include "str_aux.h"
#include <stdio.h>


string
trim(const string& str)
{
    string tmpStr = str;
    if(!tmpStr.empty())
    {
        tmpStr.erase(0, tmpStr.find_first_not_of(" \n\r\t"));
        tmpStr.erase(tmpStr.find_last_not_of(" \n\r\t") + 1, tmpStr.size()-1);
    }
    return tmpStr;
}


vector<string>
split(const string& str, const string& delimiter, bool neetTrim)
{
    vector<string> res;
    int delimiterLen = delimiter.size();

    if(delimiterLen == 0) {
        res.push_back(str);
        return res;
    }
    
    int startPos = 0;
    while(1) {
        int pos = str.find(delimiter.c_str(), startPos);
        if(pos == string::npos) {
            string partStr = str.substr(startPos, str.size() - startPos);
            if(neetTrim) {
                partStr = trim(partStr);
            }
            res.push_back(partStr);
            break;
        }
        string partStr = str.substr(startPos, pos-1);
        if(neetTrim) {
            partStr = trim(partStr);
        }
        res.push_back(partStr);
        startPos = pos + delimiterLen;
    }
    return res;
}


long
str2long(const string& str, const long &defVal)
{
    if(str == "") {
        return defVal;
    }
    
    int sign = 1;
    long longVal = 0;
    int strLen = str.size();
    for(int i = 0; i < strLen; i++) {
        if(i == 0 && str[i] == '-') {  // 处理负号
            sign = -1;
            continue;
        }
        if(str[i] > '9' || str[i] < '0') {
            return defVal;
        }
        longVal = longVal * 10 + (str[i] - '0');
    }
    return longVal * sign;
}
