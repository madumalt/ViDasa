#ifndef DATA_READER_H
#define DATA_READER_H

#include "opencv2/opencv.hpp"
#include "stdio.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/encoding.h"
#include "libxml/xmlwriter.h"
#include <iostream>

#include "util.h"
using namespace cv;
using namespace std;
#define  THERMAL 0
#define PETS09 1
#define PETS06 2
#define TUDCROSS 4
#define VIDEO 3
#define  HOCKEY 5
#define  CAMPUS 6
#define TUDST 7
#define PETS09S2L1 8
#define PETS09S2L2 9
#define PETS09S2L3 10
#define PETS09S1L1 11
#define PETS09S1L2 12

class DataReader
{
protected:
	int frameCount;
public:
	DataReader():frameCount(0){};
	virtual void readImg(Mat& frame)=0;
};

typedef struct Result2D
{
	int id;
	float xc;
	float yc;
	float w;
	float h;
	double response;
	Result2D(int i,float x_,float y_,float w_,float h_,double res=1):id(i),xc(x_),yc(y_),w(w_),h(h_),response(res){}
	Result2D(){}
}Result2D;

class GTReader
{
public:
	virtual bool getNextFrameResult(vector<Result2D>& result)=0;
};
class GTWriter
{
public:
	virtual bool putNextFrameResult(vector<Result2D>& result)=0;
};
class VideoReader:public DataReader
{
	VideoCapture capture;
public:
	VideoReader():capture("C:\\Users\\jmzhang\\Downloads\\video_data\\elan_files\\RA021_2011_09_28_child.avi")
	{
		capture.set(CV_CAP_PROP_POS_MSEC,1500000);
	}
	virtual void readImg(Mat& frame)
	{
		capture>>frame;
	}
};
class ThermalDataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\4b\\img_";
		char _count[10];
		sprintf(_count,"%d",frameCount*2);
		string count=_count;
		count.insert(count.begin(),5-count.length(),'0');
		count=count+".bmp";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S2\\L3\\Time_14-41\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09S2L1DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S2\\L1\\Time_12-34\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09S2L2DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S2\\L2\\Time_14-55\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09S2L3DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S2\\L3\\Time_14-41\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09S1L1DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S1\\L1\\Time_13-59\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09S1L2DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S1\\L2\\Time_14-06\\View_001\\frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS06DataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\S2-T3-C\\video\\pets2006\\S2-T3-C\\4\\S2-T3-C.";
		char _count[10];
		sprintf(_count,"%d",frameCount+500);
		string count=_count;
		count.insert(count.begin(),5-count.length(),'0');
		count=count+".jpeg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class TUDCrossDataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\tud-crossing-sequence\\DaSide0811-seq7-";
		char _count[10];
		sprintf(_count,"%d",frameCount+1);
		string count=_count;
		count.insert(count.begin(),3-count.length(),'0');
		count=count+".png";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class TUDCampusDataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\tud-campus-sequence\\DaSide0811-seq6-";
		char _count[10];
		sprintf(_count,"%d",frameCount+90);
		string count=_count;
		count.insert(count.begin(),3-count.length(),'0');
		count=count+".png";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class TUDSTDataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jmzhang\\Downloads\\video_data\\cvpr10_tud_stadtmitte\\cvpr10_tud_stadtmitte\\DaMultiview-seq";
		char _count[10];
		sprintf(_count,"%d",frameCount+7022);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".png";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class HockeyDataReader:public DataReader
{
public:
	virtual void readImg(Mat& frame)
	{
		string fileName="C:\\Users\\jimmie\\Downloads\\video_data\\hockey\\BPF\\data\\h";
		char _count[10];
		sprintf(_count,"%d",frameCount+700);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};
class PETS09GTReader:public GTReader
{
	xmlDocPtr file;
	xmlNodePtr frame;
	xmlChar* temp;
	bool open_success;
public:
	PETS09GTReader(const char* filename)
	{
		open_success=true;
		file=xmlReadFile(filename,"UTF-8",XML_PARSE_RECOVER);
		if (file == NULL)
		{
			cout<<"fail to open"<<endl;
			open_success=false;
		}
		if (open_success)
		{
			frame=xmlDocGetRootElement(file);
			if (frame==NULL)
			{
				cout<<"empty file"<<endl;
				open_success=false;
			}
			if (xmlStrcmp(frame->name,BAD_CAST"dataset"))
			{
				cout<<"bad file"<<endl;
				open_success=false;
			}
			frame=frame->children;
		}				
	}
	~PETS09GTReader()
	{
		xmlFreeDoc(file);
	}
	inline bool getOpenSuc(){return open_success;}
	virtual bool getNextFrameResult(vector<Result2D>& result)
	{
		bool rt=false;
		result.clear();
		while (frame!=NULL)
		{
			if (!xmlStrcmp(frame->name,BAD_CAST"frame"))
			{
				rt=true;//get the successive frame
				xmlNodePtr objectList;
				objectList=frame->children;
				while (objectList!=NULL)//objectlist level
				{
					if (!xmlStrcmp(objectList->name,BAD_CAST"objectlist"))
					{
						xmlNodePtr object=objectList->children;
						while (object!=NULL)//object level
						{
							if (!xmlStrcmp(object->name,BAD_CAST"object"))
							{
								Result2D res;
								temp=xmlGetProp(object,BAD_CAST"id");
								res.id=string2int((char*)temp);
								xmlFree(temp);
								xmlNodePtr box=object->children;
								while (box!=NULL)
								{
									if (!xmlStrcmp(box->name,BAD_CAST"box"))
									{
										temp=xmlGetProp(box,BAD_CAST"h");
										res.h=string2float((char*)temp);
										xmlFree(temp);
										temp=xmlGetProp(box,BAD_CAST"w");
										res.w=string2float((char*)temp);
										xmlFree(temp);
										temp=xmlGetProp(box,BAD_CAST"xc");
										res.xc=string2float((char*)temp);
										xmlFree(temp);
										temp=xmlGetProp(box,BAD_CAST"yc");
										res.yc=string2float((char*)temp);
										xmlFree(temp);
										break;
									}
									box=box->next;
								}
								result.push_back(res);
							}
							object=object->next;
						}
						break;
					}	
					objectList=objectList->next;
				}
				break;
			}
			frame=frame->next;
		}
		if (frame!=NULL)
		{
			frame=frame->next;
		}		
		return rt;
	}	

private:

};
#define  ENCODING "UTF-8"
class ResultGTWriter: public GTWriter
{
	int rc;
	xmlTextWriterPtr writer;
	xmlChar *tmp;
	bool open_success;
	int frameCount;
public:
	ResultGTWriter(const char* filename):frameCount(0)
	{
		open_success=true;
		writer = xmlNewTextWriterFilename(filename, 0);
		if (writer == NULL) {
			printf("testXmlwriterFilename: Error creating the xml writer\n");
			open_success=false;
		}
		rc=xmlTextWriterStartDocument(writer,NULL,ENCODING,NULL);
#ifdef DEBUG_
		if (rc<0)
		{
			cout<<"testXmlwriterFilename: Error at xmlTextWriterStartDocument"<<endl;
		}
#endif
		rc=xmlTextWriterStartElement(writer, BAD_CAST "dataset");
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterStartElement"<<endl;
		}
#endif
	}
	~ResultGTWriter()
	{
		rc = xmlTextWriterEndDocument(writer);
#ifdef DEBUG_
		if (rc < 0) {
			printf
				("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
			return;
		}
#endif
		xmlFreeTextWriter(writer);
	}
	virtual bool putNextFrameResult(vector<Result2D>& result)
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "frame");
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterStartElement"<<endl;
		}
#endif
		rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "number",
			"%d",frameCount);
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
		}
#endif
		rc=xmlTextWriterStartElement(writer,BAD_CAST"objectlist");
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterStartElement"<<endl;
		}
#endif
		vector<Result2D>::iterator it;
		for (it=result.begin();it<result.end();it++)
		{
			rc = xmlTextWriterStartElement(writer, BAD_CAST "object");
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterStartElement"<<endl;
			}
#endif
			rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id",
				"%d",(*it).id);
			rc=xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "confidence",
				"%lf",(*it).response);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
			}
#endif
			rc = xmlTextWriterStartElement(writer, BAD_CAST "box");
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterStartElement"<<endl;
			}
#endif
			rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "h",
				"%f",(*it).h);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
			}
#endif
			rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "w",
				"%f",(*it).w);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
			}
#endif
			rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xc",
				"%f",(*it).xc);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
			}
#endif
			rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "yc",
				"%f",(*it).yc);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterWriteAttribute"<<endl;
			}
#endif

			//end box
			rc = xmlTextWriterEndElement(writer);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterEndElement"<<endl;
			}
#endif
			//end object
			rc = xmlTextWriterEndElement(writer);
#ifdef DEBUG_
			if (rc < 0) {
				cout<<"testXmlwriterFilename: Error at xmlTextWriterEndElement"<<endl;
			}
#endif
		}
		//end objectlist
		rc = xmlTextWriterEndElement(writer);
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterEndElement"<<endl;
		}
#endif
		//end frame
		rc = xmlTextWriterEndElement(writer);
#ifdef DEBUG_
		if (rc < 0) {
			cout<<"testXmlwriterFilename: Error at xmlTextWriterEndElement"<<endl;
		}
#endif

		frameCount++;
		return true;
	}
	inline bool getOpenSuc(){return open_success;}

};


#endif