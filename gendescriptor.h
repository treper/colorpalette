#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <vector>
#include <string>
#include <numeric>
using namespace cv;
using namespace std;
//opencv bgr
//vector<Point3f> colors(512,Point3f(0,0,0));
vector<Point3f> initColors();
void calcDist(map<int,long>& dist,Vec3b& p );
int getIndex(Vec3b& p);
int getIndex(const string& s);
int getIndex(int rgb);
vector<int> gendescriptor(string filename);


