#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <stdio.h>
#include "utils.h"

using namespace cv;
using namespace std;
//opencv bgr
//vector<Point3f> colors(512,Point3f(0,0,0));
static vector<Point3f> colors(512,Point3f(0,0,0));
vector<Point3f> initColors()
{
    for(int i =0; i<8; i++)
        for(int j =0; j<8; j++)
            for(int k =0; k<8; k++)
            {
                colors[64*i+8*j+k] = Point3f(16*(2*i+1), 16*(2*j+1), 16*(2*k+1));
                //cout<<"colors "<<64*i+8*j+k<<16*(2*i+1)<<16*(2*j+1)<<16*(2*k+1)<<endl;
            }
    return colors;
}
/*void calcDist(map<int,long>& dist,Vec3b& p )
{
    for(int i =0;i<512;i++)
    {
        dist[i]=(colors[i].x-int(p[0]))*(colors[i].x-int(p[0]))+(colors[i].y-int(p[1]))*(colors[i].y-int(p[1]))+(colors[i].z-int(p[2]))*(colors[i].z-int(p[2]));
    }
}*/
int getIndex(Vec3b& p)
{
    uchar b,g,r;
    r=p[2]>>5;
    g=p[1]>>5;
    b=p[0]>>5;
    return r*64+g*8+b;
}
int getIndex(int rgb)
{
    uchar b,g,r;
    b=(rgb&0xff)>>5;
    g=((rgb>>8)&0xff)>>5;
    r=((rgb>>16)&0xff)>>5;
    cout<<"rgb:"<<int(r)<<" "<<int(g)<<" "<<int(b)<<endl;
    return r*64+g*8+b;
}
int getIndex(const string& s)
{
    int rgb = hex2int(s);
    return getIndex(rgb);
}
vector<int> gendescriptor(string filename)
{
    //cout<<"generate descriptor for "<<argv[1]<<endl;
    Mat img = imread(filename.c_str());
    //cout<<"init colors"<<endl;
     //cout<<"after init colors"<<endl;
    vector<int> hist = vector<int>(512, 0);
    int r = img.rows, c = img.cols;
   // cout<<"width ans height:"<<r<<" "<<c<<endl;
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            map<int,long> dist;
            Vec3b p= img.at<Vec3b>(i,j);
            //cout<<"processing point "<<i<<" "<<j<<"("<<int(int(p[0]))<<","<<int(int(p[1]))<<","<<int(int(p[2]))<<")"<<endl;
            //calcDist(dist, p);
            //cout<<"map size"<<dist.size()<<endl;
            //int minloc = dist.begin()->first;
            int minloc = getIndex(p);
            hist[minloc]++;
        }
    }

    for(vector<int>::iterator it = hist.begin(); it !=hist.end(); ++it)
    {
        //cout<<*it<<" ";
        *it=int((double(*it)/double(r*c))*100);
        //cout<<*it<<" ";
    }
    /*int s = std::accumulate(hist.begin(), hist.end(), 0);
    cout<<"sum: "<<s<<endl;*/
    return hist;
}
