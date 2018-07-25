/** Add some common auxiliary function for std::string */
#ifndef __STR_AUX_H__
#define __STR_AUX_H__

#include <string>
#include <vector>

class StrAux
{
private:
    static const bool sm_fastStripTb[128];

public:
    static std::string strip(const std::string& origStr);
    static bool split(const std::string& origStr,
                      const std::string& delimiter,
                      std::vector<std::string>& output);
};

#endif
