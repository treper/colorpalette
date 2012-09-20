#include "utils.h"
#include "emd.h"
#include "gendescriptor.h"
#include "palette_index.h"
#include <fstream>
#include <algorithm>
#include <time.h>

namespace ColorPalette
{
using namespace std;
PaletteIndex::PaletteIndex()
{
    total_colors =512;
    group = 512;
	for(int i=0; i <8; i++)
	{
		for(int j = 0 ; j < 8;j++)
		{
			for (int k=0; k<8; k++)
			{
				Vec3b t(16*(2*i+1), 16*(2*j+1), 16*(2*k+1));
				colors.push_back(t);
			}
			
		}
	}
    //initColors();
}
PaletteIndex::~PaletteIndex()
{

}
bool PaletteIndex::myfunction(const SingleColor& a, const SingleColor& b)
{
    return a.colorPercent<b.colorPercent;
}
void PaletteIndex::loadPics(const string& filelist)
{
    ifstream fin(filelist.c_str());
    if (fin.is_open() == false)
    {
        cout<<"error reading file list file"<<endl;
        exit(0);
    }
    string line;
    while(getline(fin, line))
    {
        fileNames.push_back(line);
    }
}
void PaletteIndex::printDescriptors()
{
    cout<<"descriptors size "<<descriptors.size()<<endl;
    /*for(vector<vector<int> >::iterator i = descriptors.begin(); i !=descriptors.end(); i++)
    {
        cout<<"============================"<<endl;
        for(vector<int>::iterator j = i->begin(); j!=i->end(); j++)
        {
            cout<<*j<<" ";
        }
        cout<<endl;
    }*/
}
void PaletteIndex::buildIndex()
{
    cout<<"building index..."<<endl;
    clock_t start,end;
    start=clock();
    cout<<"generating descriptors for each image"<<endl;
    for(vector<string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
    {
        vector<int> hist = gendescriptor(*it);
        descriptors.push_back(hist);
    }

    end=clock();
    double duration =  (end-start)/CLOCKS_PER_SEC;
    cout<<"generating descriptors cost "<<duration<<" seconds"<<endl;
    for(int i =0; i < group; i++)
    {
        vector<SingleColor> sortedSingleColor;
        for(int j = 0; j < descriptors.size(); j++)
        {
            SingleColor c ;
            vector<int>& desc = descriptors[j];
            c.fileId = j;
            c.colorPercent = desc[i];
            assert(c.colorPercent>=0);
            sortedSingleColor.push_back(c);
        }
        assert(sortedSingleColor.size()==fileNames.size());
        sort(sortedSingleColor.begin(), sortedSingleColor.end(), myfunction);
        picIndex.push_back(sortedSingleColor);
        //generate color index
        int count=0, ct = 0;
        vector<ColorPercent> sortedColorPercent;
        for(vector<SingleColor>::iterator it = sortedSingleColor.begin(); it != sortedSingleColor.end();)
        {
            count = it->colorPercent;
            if(count == (it+1)->colorPercent)
            {
                ++it;
                ct++;
                continue;
            }
            else
            {
                ColorPercent pc;
                pc.colorPercent = it->colorPercent;
                pc.last=ct;
                sortedColorPercent.push_back(pc);
                ++it;
                ct++;
            }
        }
        colorIndex.push_back(sortedColorPercent);
    }
    cout<<picIndex.size()<<endl;
    assert(picIndex.size()==group);
    cout<<colorIndex.size()<<endl;
     assert(colorIndex.size()==group);
    end = clock();
    duration = (end-start)/CLOCKS_PER_SEC;
    cout<<"index builded in "<<duration<<" seconds"<<endl;
}

vector<int> PaletteIndex::getVectorIntersection(vector<int>& a, vector<int>& b)
{
    assert(a.size()>0 || b.size()>0);
    vector<int> intersection;
    //empty
    if(a.size()==0 && b.size()!=0)
    {
        /*for(vector<SingleColor>::iterator it = b.begin(); it !=b.end(); ++it)
        {
            intersection.push_back(it->fileId);
        }
        return intersection;*/
        return b;
    }
    else if(b.size()==0 && a.size()!=0)
    {
        return a;
    }
    //a b is not sorted
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    vector<int>::iterator ia = a.begin(), ib = b.begin();
    while(ia!=a.end() && ib !=b.end())
    {
        if(*ia == *ib)
        {
            intersection.push_back(*ia);
            ++ia;
            ++ib;
            continue;
        }
        else if(*ia < *ib)
        {
            intersection.push_back(*ia);
            ++ia;
            continue;
        }
        else
        {
            intersection.push_back(*ib);
            ++ib;
            continue;
        }
        if(ia==a.end())
        {
            while(ib!=b.end())
            {
                intersection.push_back(*ib);
                ++ib;
            }
        }
        if(ib == b.end())
        {
              while(ia!=a.end())
              {
                  intersection.push_back(*ia);
                  ++ia;
              }
        }
    }
    return intersection;
}
//earth move distance
float PaletteIndex::histMatchDist(vector<int>& a, vector<int>& b)
{
	//convert vector to signature
	feature_t fa[512],fb[512];
	float wa[512],wb[512];
	int i = 0;
	for (vector<int>::iterator it = a.begin(); it != a.end(); ++it)
	{
		fa[i].X=colors[i][0];
		fa[i].Y=colors[i][1];
		fa[i].Z=colors[i][2];
		wa[i]=*it;
		i++;
	}
	i = 0;
	for (vector<int>::iterator it = b.begin(); it != b.end(); ++it)
	{
		fb[i].X=colors[i][0];
		fb[i].Y=colors[i][1];
		fb[i].Z=colors[i][2];
		wb[i]=*it;
		i++;
	}	
	signature_t sa={512,fa,wa},sb={512,fb,wb};
	float e = emd(&sa, &sb, dist, 0, 0);
	return e;
	
}
double PaletteIndex::histIntersectionDist(vector<int>& a, vector<int>& b)
{
    /*cout<<"a"<<endl;
    for(vector<int>::iterator it = a.begin(); it !=a.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl<<"b"<<endl;
    for(vector<int>::iterator it = b.begin(); it !=b.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl;*/
    assert(a.size()==b.size());
    //histogram intersection distance
    vector<int> c(a.size(),0);
    int i=0;
    while(i < a.size() && i < b.size())
    {
        if(a[i] < b[i])
        {
            c[i]=a[i];
        }
        else
        {
            c[i]=b[i];
        }
        i++;
    }
    int s = std::accumulate(c.begin(), c.end(),0);
    int sa=std::accumulate(a.begin(), a.end(),0);
    int sb=std::accumulate(b.begin(),b.end(),0);
    cout<<"sc:"<<s<<"sa:"<<sa<<"sb:"<<sb<<endl;
    //return double(min(sa,sb))/double(s);
    return double(s)/min(sa,sb);
}
void PaletteIndex::printResult(vector<int>& desc, vector<int>& final)
{
    //print final file names for human check
    cout<<"candidate images are: "<<endl;
    map<double,int> searchResult;
    for(vector<int>::iterator fit=final.begin(); fit!=final.end();fit++)
    {
        double dist = histMatchDist(desc, descriptors[*fit]);
        cout<<fileNames[*fit]<<" "<<dist<<endl;
        searchResult[dist]=*fit;
    }
    cout<<"similar images are: "<<endl;
    for(map<double,int>::iterator it = searchResult.begin(); it != searchResult.end(); ++it)
    {
        cout<<fileNames[it->second]<<" "<<it->first<<endl;
    }
}
/*vector<int> PaletteIndex::getNeighborColor(int color)
{
	vector<int> neighbor;
	int r,g,b;
	r=color
}*/
void PaletteIndex::searchHist(vector<int>& desc, vector<int>& final)
{
    vector<vector<int> > results;
    int c = 0;//group iterator
    for(vector<int>::iterator it=desc.begin(); it != desc.end(); ++it)
    {
        if(*it!=0)
        {
            //c is color group, *it is color percent
			//vector<int> c = getNeighborColor(*it);
            vector<ColorPercent>& ci=colorIndex[c];
            cout<<"query percent "<<*it<<endl;
            int last = 0;
            for(vector<ColorPercent>::iterator ic = ci.begin(); ic !=ci.end();++ic)
            {
                cout<<ic->colorPercent<<endl;
				if(*it >= ic->colorPercent)
				{   
					if(ic+1 != ci.end())
					{   
						if(*it < (ic+1)->colorPercent)
						{   
							last = ic->last;
							cout<<"get it"<<endl;
							break;
						}   
						else
						{   
							last=ic->last;
							continue;
						}   
					}   
					last = ic->last;
				}
                else
                {
                    last=ic->last;
                    cout<<"get it"<<endl;
                    break;
                }
            }
            vector<int> singleColorResult;
            for(int i = last-2; i<last+2; i++)
            {
                if(i>=0)
                {
                    singleColorResult.push_back(picIndex[c][i].fileId);
                }
                else
                {
                    singleColorResult.push_back(picIndex[c][0].fileId);
                }
            }
            results.push_back(singleColorResult);
        }
        c++;
    }
    cout<<"candidate groups:"<<results.size()<<endl;
    //get intersection of all the candidate file ids
    vector<vector<int> >::iterator it = results.begin();
    while( it != results.end())
    {
        /*int ic=0;
        for(vector<int>::iterator i=it->begin(); i!=it->end(); i++)
        {
            cout<<fileNames[ic]<<",";
            ic++;
        }
        cout<<endl;*/
        final=getVectorIntersection(final,*it);
        ++it;
    }
    printResult(desc,final);
}

void PaletteIndex::search(const string& fileName)
{
    assert(colorIndex.size()>0 && picIndex.size()>0);
    vector<int> desc=gendescriptor(fileName);
    cout<<"query descriptors:"<<endl;
    for(vector<int>::iterator it = desc.begin(); it != desc.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    vector<int> final;
    searchHist(desc, final);
}
void PaletteIndex::search(map<int,int>& q)
{
    assert(q.size()>0);
    vector<int> final;
    vector<int> desc(512,0);
    for(map<int,int>::iterator it = q.begin(); it!=q.end(); it++)
    {
        int index = getIndex(it->first);
        desc[index]=it->second;
    }
    searchHist(desc, final);
}
}

