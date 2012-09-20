#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <assert.h>
#include "utils.h"
#include "palette_index.h"

using namespace std;
using namespace ColorPalette;
map<int,int> getPalette(vector<string>& v)
{
    map<int,int> queryPaletter;
    for(vector<string>::iterator it = v.begin(); it !=v.end(); ++it)
    {
        //cout<<"split "<<*it<<endl;
        vector<string> p;
        split(*it,':',p);
        //for(vector<string>::iterator it = p.begin(); it!=p.end(); it++)
        //cout<<*it<<" ";
        assert(p.size()==2);
        int rgb=hex2int(p[0]);
        int percent=myhex2int(p[1]);
        cout<<"rgb,percent:"<<rgb<<","<<percent<<endl;
        queryPaletter[rgb]=percent;
    }
    return queryPaletter;
}
int main(int argc, char** argv)
{
    PaletteIndex pi;
    pi.loadPics("pics.txt");
    pi.buildIndex();
    pi.printDescriptors();
    cout<<"image search image"<<endl;
    string filename = argv[1];
    pi.search(filename);
    cout<<"hist search"<<endl;
    string querylist="./test_result/query.txt";
    ifstream fin(querylist.c_str(),ios::in);
    string line;
    while(getline(fin, line))
    {
        if(line.find(':')!=string::npos)
        {
         cout<<"query palette is: "<<line<<endl;
        vector<string> v;
        split(line,';',v);
        map<int,int> q = getPalette(v);
        cout<<"query palette:"<<endl;
        for(map<int,int>::iterator it = q.begin(); it != q.end(); ++it)
        {
            cout<<it->first<<":"<<it->second<<";"<<endl;
        }
        pi.search(q);
        }
    }
    return 0;
}
