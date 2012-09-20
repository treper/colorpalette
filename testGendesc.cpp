#include "gendescriptor.h"
#include <string>
using namespace std;
int main(int argc, char** argv)
{
    char* filename = argv[1];
    vector<int> desc = gendescriptor(string(filename));
    for(vector<int>::iterator it = desc.begin(); it != desc.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    return 0;
}
