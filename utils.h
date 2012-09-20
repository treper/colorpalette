#include <vector>
#include <string>
#include "emd.h"
using namespace std;
long hex2int(const string& hexStr );
int myhex2int(const string& hexStr);
void split(const string& s, char c, vector<string>& v);
float dist(feature_t *F1, feature_t *F2);
