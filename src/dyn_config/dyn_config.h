#ifndef __DYN_CONFIG_H__
#define __DYN_CONFIG_H__

#include <map>
#include <string>
#include <time.h>
#include <vector>
#include "singleton.h"

using std::map;
using std::string;
using std::vector;


typedef struct
{
    string strVal;
    int intVal;
    float floatVal;

    /** 各值的有效性标志
     * 以掩码的形式表示，从地位到高位依次表示strVal、intVal、floatVal值是否有效
     */
    char validFlag;
} VarVals;


class ConfItem
{
  protected:
    pthread_rwlock_t m_itLock;
    map<int, VarVals> m_varVals;

  public:
    ConfItem();
    ~ConfItem();
    int size();
    bool setConfVal(const string &confVal, const int &index = 0);
    string getValStr(const string &defVal);
    string getValStr(const int &index, const string &defVal);
    int getValInt(const int &defVal);
    int getValInt(const int &index, const int &defVal);
    float getValFloat(const float &defVal);
    float getValFloat(const int &index, const float &defVal);
};


typedef struct
{
    string path;
    time_t updteTm;
} ConfFileInfo;


class Config : public SingletonBase<Config>
{
  private:
    vector<ConfFileInfo> m_confFInfos;
    map<string, ConfItem> m_confCache;

    bool isConfFUpdate(ConfFileInfo *pConfFInfo);  /**< 判断配置文件是否有更新 */
    bool loadConf(const string &confFPath);        /**< 加载配置文件入内存 */
    /** 处理配置文件中的一行 */
    bool handleConfFLine(const string &confLine, string *pBlockName);
    
  public:
    bool init(const vector<string> &confFPaths);
    void run();
    ConfItem* getConfItemPtr(const string &confKey);
};


void* confThreadFunc(void *param);


#endif
