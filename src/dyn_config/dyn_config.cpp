#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dyn_config.h"
#include "glb_val.h"
#include "str_aux.h"

using std::ifstream;
using std::ofstream;


ConfItem::ConfItem()
{
    pthread_rwlock_init(&m_itLock, NULL);
}


ConfItem::~ConfItem()
{
    pthread_rwlock_destroy(&m_itLock);
}



bool
ConfItem::setConfVal(const string &confVal, const int &index)
{
    pthread_rwlock_wrlock(&m_itLock);
    map<int, VarVals>::iterator iter = m_varVals.find(index);
    if(iter == m_varVals.end()) {
        VarVals varVals;
        m_varVals[index] = varVals;
        iter = m_varVals.find(index);
    }
    if(confVal != iter->second.strVal) {
        iter->second.strVal = confVal;
        iter->second.validFlag = 0x01;
        
        const char *pStrStart = confVal.c_str();
        char *pStrEnd = NULL;
        iter->second.floatVal = strtof(pStrStart, &pStrEnd);
        if(pStrEnd - pStrStart == confVal.size()) {
            iter->second.validFlag |= 0x02;
            iter->second.validFlag |= 0x04;
            iter->second.intVal = (int)iter->second.floatVal;
        }
    }
    pthread_rwlock_unlock(&m_itLock);
    return true;
}


int
ConfItem::size()
{
    return m_varVals.size();
}


string
ConfItem::getValStr(const string &defVal)
{
    return getValStr(0, defVal);
}


string
ConfItem::getValStr(const int &index, const string &defVal)
{
    string rst = defVal;
    pthread_rwlock_rdlock(&m_itLock);
    map<int, VarVals>::iterator iter = m_varVals.find(index);
    if(iter != m_varVals.end()) {
        if(iter->second.validFlag & 0x01) {
            rst = iter->second.strVal;
        }
    }
    pthread_rwlock_unlock(&m_itLock);
    return rst;
}


int
ConfItem::getValInt(const int &defVal)
{
    return getValInt(0, defVal);
}


int
ConfItem::getValInt(const int &index, const int &defVal)
{
    int rst = defVal;
    pthread_rwlock_rdlock(&m_itLock);
    map<int, VarVals>::iterator iter = m_varVals.find(index);
    if(iter != m_varVals.end()) {
        if(iter->second.validFlag & 0x02) {
            rst = iter->second.intVal;
        }
    }
    pthread_rwlock_unlock(&m_itLock);
    return rst;
}


float
ConfItem::getValFloat(const float &defVal)
{
    return getValFloat(0, defVal);
}


float
ConfItem::getValFloat(const int &index, const float &defVal)
{
    float rst = defVal;
    pthread_rwlock_rdlock(&m_itLock);
    map<int, VarVals>::iterator iter = m_varVals.find(index);
    if(iter != m_varVals.end()) {
        if(iter->second.validFlag & 0x04) {
            rst = iter->second.floatVal;
        }
    }
    pthread_rwlock_unlock(&m_itLock);
    return rst;
}


bool
Config::isConfFUpdate(ConfFileInfo *pConfFInfo)
{
    struct stat fStatBuf;
    if(stat(pConfFInfo->path.c_str(), &fStatBuf) != 0) {
        PRINT_MSG("Get config file staus fail.");
        return false;
    }
    if(fStatBuf.st_mtime != pConfFInfo->updteTm) {
        pConfFInfo->updteTm = fStatBuf.st_mtime;
        return true;
    }
    return false;
}


bool
Config::loadConf(const string &confFPath)
{
    ifstream f_ih(confFPath.c_str());
    if(!f_ih.is_open()) {
        PRINT_MSG("Open config file fail.");
        return false;
    }

    string blockName = "";
    char buffer[512];
    while(!f_ih.eof()) {
        f_ih.getline(buffer, 512);
        string confLine = buffer;
        handleConfFLine(confLine, &blockName);
    }

    f_ih.close();
    return true;
}


bool
Config::handleConfFLine(const string &confLine, string *pBlockName)
{
    string tmpConfLine = trim(confLine);

    // 处理空行和注释
    if(tmpConfLine == "" || tmpConfLine[0] == '#') {
        return true;
    }

    // 处理块标识
    if(tmpConfLine[0] == '[' && tmpConfLine[tmpConfLine.size() - 1] == ']') {
        *pBlockName = tmpConfLine.substr(1, tmpConfLine.size() - 2);
        return true;
    }

    // 识别配置项
    vector<string> kvSlices = split(tmpConfLine, "=", true);
    if(kvSlices.size() != 2) {
        return false;
    }
    string subConfKey = kvSlices[0];
    string confVal = kvSlices[1];
    int index = 0;
    vector<string> kSlices1 = split(subConfKey, "[", true);
    if(kSlices1.size() == 2) {
        if(subConfKey[subConfKey.size() - 1] != ']') {
            return false;
        }
        subConfKey = kSlices1[0];
        string indexStr = kSlices1[1].substr(0, kSlices1[1].size() - 1);
        index = str2long(indexStr, -1);
        if(index < 0) {
            return false;
        }
    }
    else if(kSlices1.size() > 2) {
        return false;
    }

    // 写入cache
    string confKey = *pBlockName + "." + subConfKey;
    map<string, ConfItem>::iterator confIter = m_confCache.find(confKey);
    if(confIter == m_confCache.end()) {
        ConfItem confItem;
        m_confCache[confKey] = confItem;
        confIter = m_confCache.find(confKey);
    }
    confIter->second.setConfVal(confVal, index);

    return true;
}


bool
Config::init(const vector<string> &confFPaths)
{
    vector<string>::const_iterator iter;
    for(iter = confFPaths.begin(); iter != confFPaths.end(); iter++) {
        ConfFileInfo tmpConfFInfo = {*iter, 0};
        m_confFInfos.push_back(tmpConfFInfo);
    }
    return true;
}


void
Config::run()
{
    vector<ConfFileInfo>::iterator iter;
    while(1) {
        sleep(1);
        for(iter = m_confFInfos.begin(); iter != m_confFInfos.end(); iter++) {
            if(!isConfFUpdate(&(*iter))) {
                continue;
            }
            loadConf((*iter).path);
        }
    }
}


ConfItem*
Config::getConfItemPtr(const string &confKey)
{
    map<string, ConfItem>::iterator iter = m_confCache.find(confKey);
    if(iter != m_confCache.end()) {
        return &(iter->second);
    }
    return NULL;
}


void* confThreadFunc(void *param)
{
    Config::getInstance().run();
}
