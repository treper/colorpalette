#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <vector>
#include <map>
#include <string>

namespace ColorPalette
{
using namespace std;
using namespace cv;
struct ColorPercent
{
    //100 percent
    int colorPercent;
    int last;
};
struct SingleColor
{
    int colorPercent;
    int fileId;
};
class PaletteIndex
{
public:
    PaletteIndex();
    ~ PaletteIndex();
    void init();
    void loadPics(const string& filelist);
    void buildIndex();
    void printDescriptors();
	float histMatchDist(vector<int>& a, vector<int>& b);
    double histIntersectionDist(vector<int>& a, vector<int>& b);
    vector<int> getVectorIntersection(vector<int>& a, vector<int>& b);
    void printResult(vector<int>& desc, vector<int>& final);
    void search(const string& filename);
    void search(map<int,int>& dict);
    void searchHist(vector<int>& desc, vector<int>& final);
    static bool myfunction(const SingleColor& a, const SingleColor& b);
private:
    //color quantization
    //static vector<Point3f> colors;
    //group by 512 color, inverted index for each color
	vector<Vec3b> colors;
    vector<vector<ColorPercent> > colorIndex;
    vector<vector<SingleColor> > picIndex;
    //all descriptors fileId,descriptor histogram
    vector<vector<int> > descriptors;
    vector<string> fileNames;
    int total_colors;
    int group;
};
//vector<Point3f> PaletteIndex::colors = initColors();
}

