#ifndef UTIL_
#define  UTIL_

#define  THRESHOLD_ABSOLUTE 2
#define  THRESHOLD_ADAPTIVE 4
#define  THRESHOLD_INVERSE 1
#include "cv.h"

using namespace cv;
using namespace std;
typedef struct Segment
{
	int ID;
	CvScalar color;
	int area;
	CvPoint up_left;//bounding box
	CvPoint low_right;//bounding box
public:
	Segment(){ID=0;color=cvScalar(0);area=0;up_left=low_right=cvPoint(0,0);};
}Segment;
template<class T> class Img_
{
private:
	IplImage* imgp;
public:
	Img_(IplImage* img=0){imgp=img;}
	~Img_(){imgp=0;}
	void operator=(IplImage* img){imgp=img;}
	inline T* operator[](const int rowIndx)
	{
		return (T*)(imgp->imageData+rowIndx*imgp->widthStep);
	}
};
typedef struct  
{
	unsigned char b,g,r;
}RgbPixel;
typedef struct
{
	float b,g,r;
}RgbPixelFloat;
typedef Img_<RgbPixel> RgbImage_;
typedef Img_<RgbPixelFloat> RgbImageFloat_;
typedef Img_<unsigned char> BwImage_;
typedef Img_<float> BwImageFloat_;
inline bool isIdentical(CvPoint a,CvPoint b,BwImage_ img);
template<class T> class Img
{
private:
	Mat* imgp;
public:
	Img(Mat& img=0){imgp=&img;}
	~Img(){imgp=0;}
	void operator=(const Mat& img){imgp=&img;}
	inline T* operator[](const int rowIndx)
	{
		return (T*)(imgp->data+rowIndx*imgp->step);
	}
};

typedef Img<RgbPixel> RgbImage;
typedef Img<RgbPixelFloat> RgbImageFloat;
typedef Img<unsigned char> BwImage;
typedef Img<float> BwImageFloat;

//src should be gray scale image; segStack is the stack of the segment structure; segMap is the labels corresponding 
//to Segment::ID
void segment(IplImage* src,vector<Segment>* segStack,IplImage* segMap,int areaLB=30,int areaHB=-1);


//visualize the result, src should be gray scale, only white part will be segmented
void render_segment(Mat& src, Mat& dst);
void detectSkin(Mat& src, Mat& dst);
void threshold_ex(Mat& src,Mat& dst,double c,int mode=THRESHOLD_ABSOLUTE);


//distance of rectangle
#define OVERLAP 0
#define INCLUSTION 1
inline double getRectDist(Rect r1,Rect r2,int type)
{
	Rect op=r1&r2;
	switch (type)
	{
	case INCLUSTION:
		return 1-(double)op.area()/((double)r1.area());
	default:
		return 1-(double)op.area()/(double)(r1.area()+r2.area()-op.area());
	}
}
inline int string2int(const char* s)
{
	int i;
	if(sscanf(s, "%d", &i) == EOF )
	{
		cout<<"error reading integer"<<endl;
	}
	return i;
}
inline double string2float(const char* s)
{
	float d;
	if (sscanf(s,"%f",&d)==EOF)
	{
		cout<<"error reading float"<<endl;
	}
	return d;
}
inline double _string2double(string s)
{
	double d;
	if (sscanf(s.c_str(),"%lf",&d)==EOF)
	{
		cout<<"error reading float"<<endl;
	}
	return d;
}


inline string _double2string(double d)
{
	ostringstream s;
	s << d;
	return s.str();
}

inline int _char_p2int(const char* s)
{
	int i;
	if(sscanf(s, "%d", &i) == EOF )
	{
		cout<<"error reading integer"<<endl;
	}
	return i;
}

inline Rect scaleWin(Rect win, double scale)
{
	return Rect(win.x+0.5*(1-scale)*win.width,win.y+0.5*(1-scale)*win.height,win.width*scale,win.height*scale);
}

#endif