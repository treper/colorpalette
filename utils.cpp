#include <string>
#include <cstdlib>
#include <math.h>
#include "utils.h"
using namespace std;
float dist(feature_t *F1, feature_t *F2)
{
	int dX = F1->X - F2->X, dY = F1->Y - F2->Y, dZ = F1->Z - F2->Z;
	return sqrt(dX*dX + dY*dY + dZ*dZ); 
}

long hex2int(const string& hexStr )
{
    char* offset;
    if(hexStr.length() >2)
    {
        if(hexStr[0]=='0' && hexStr[1]=='x')
        {
            return strtol(hexStr.c_str(),&offset,0);
        }
        return strtol(hexStr.c_str(),&offset,16);
    }
}
int myhex2int(const string& hexStr)
{
    return atoi(hexStr.c_str());
}
void split(const string& s, char c, vector<string>& v)
{
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while(j != string::npos)
    {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c,j);
        if(j == string::npos)
        {
            v.push_back(s.substr(i,s.length()));
        }
    }
}
