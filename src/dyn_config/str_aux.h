#include <string>
#include <vector>

using std::string;
using std::vector;

string trim(const string& str);
vector<string> split(const string& str, const string& delimiter,
                     bool neetTrim = false);
long str2long(const string& str, const long &defVal = 0);