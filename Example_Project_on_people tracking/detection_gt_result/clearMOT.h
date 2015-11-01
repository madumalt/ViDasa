#ifndef CLEAR_MOT
#define CLEAR_MOT

#include "util.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "munkres.h"

#include "stdio.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/encoding.h"
#include "libxml/xmlwriter.h"

#define STA_FILE "sta.txt"


typedef struct ResultWindow 
{
	int ID;
	Rect window;
	double response;
	ResultWindow(int id,Rect wind,double r=1):ID(id),window(wind),response(r){}

}ResultWindow;

typedef struct MOTResult
{
	double MOTA;
	double MOTP;//average overlap over union, bigger the better
	int missing;
	int false_positive;
	int ID_switch;
}MOTResult;

class C_Mot
{
	double thresh_validation;
	double error_sum;
	double M_sum;
	double FP_sum;
	double IS_sum;
	double match_count;
	double gt_count;//object present
	int frame_count;
	int tra_count;

	map<int,int> gt_book_keeping;//keep the last match ID
	map<int,int> hp_book_keeping;//keep the last match ID

	//for debugging
	vector<ResultWindow> matched_couple;
	vector<ResultWindow>switched_couple;
	vector<ResultWindow> miss_detection;
	vector<ResultWindow> false_alarm;


	ofstream sta_writer;


public:
	C_Mot(double thresh = 1.0):thresh_validation(thresh),frame_count(0),error_sum(0),M_sum(0),FP_sum(0),IS_sum(0),match_count(0),gt_count(0),sta_writer(STA_FILE,ios::out),tra_count(0){}
	~C_Mot()
	{
		sta_writer.close();
	}
	void dealWith(vector<ResultWindow> gt,vector<ResultWindow> hp);
	void dealWithDetection(vector<ResultWindow> gt,vector<ResultWindow> hp);
	inline MOTResult getMOT(bool Det=false)
	{
		MOTResult rt;
		rt.MOTP=match_count>0 ? 1-error_sum/match_count:0;
		if (!Det)
			rt.MOTA=gt_count>0 ? 1-(M_sum+FP_sum+IS_sum)/gt_count:0;
		else
			rt.MOTA=gt_count>0 ? 1-(M_sum+FP_sum)/gt_count:0;
		rt.missing=M_sum;
		rt.false_positive=FP_sum;
		rt.ID_switch=IS_sum;
		cout<<"MOTP: "<<rt.MOTP<<endl;
		cout<<"MOTA: "<<rt.MOTA<<endl;
		cout<<"missing: "<<rt.missing<<endl;
		cout<<"fp: "<<rt.false_positive<<endl;
		//if (!Det)
			cout<<"IS: "<<rt.ID_switch<<endl;
		//else
		{
			cout<<"Precision: "<<match_count/(match_count+FP_sum)<<endl;
			cout<<"Recall: "<<match_count/gt_count<<endl;
		}
		int mt=0;
		int ml=0;
		cout<<"GT: "<<tra_count<<endl;
		return rt;
	}
	
	void paintFrame(Mat& frame)
	{
		for (int i=0;i<matched_couple.size();i++)
		{
			rectangle(frame,matched_couple[i].window,Scalar(0,255,0),2);
			i++;
			rectangle(frame,matched_couple[i].window,Scalar(0,255,0),1);
		}
		for (int i=0;i<switched_couple.size();i++)
		{
			rectangle(frame,switched_couple[i].window,Scalar(0,255,255),2);
			i++;
			rectangle(frame,switched_couple[i].window,Scalar(0,255,255),1);
		}
		for (int i=0;i<miss_detection.size();i++)
		{
			rectangle(frame,miss_detection[i].window,Scalar(255,0,255),2);
		}
		for (int i=0;i<false_alarm.size();i++)
		{
			rectangle(frame,false_alarm[i].window,Scalar(0,0,255),1);
		}
	}
private:
	inline double getDistance(ResultWindow w1,ResultWindow w2)
	{
		return getRectDist(w1.window,w2.window,OVERLAP);
	}
};

class ResultParser
{
	xmlDocPtr file;
	xmlNodePtr frame;
	xmlChar* temp;
	bool open_success;
	double ratio;
	double h_ratio;
	double w_ratio;
public:
	ResultParser(const char* filename,double r,double w=1.0,double h=1.0):ratio(r),h_ratio(h),w_ratio(w)
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
			/*if (xmlStrcmp(frame->name,BAD_CAST"dataset"))
			{
				cout<<"bad file"<<endl;
				open_success=false;
			}**/
			frame=frame->children;
			while (xmlStrcmp(frame->name,BAD_CAST"frame"))
			{
				frame=frame->next;
			}			
		}	
	}
	~ResultParser()
	{
		xmlFreeDoc(file);
		xmlCleanupParser();
		xmlMemoryDump();
	}
	inline bool isEnd()
	{
		return frame==NULL ? true:false;
	}
	vector<ResultWindow> readNextFrame()
	{
		vector<ResultWindow> output;
		if (frame!=NULL)
		{
			xmlNodePtr objectList=frame->children;
			while (xmlStrcmp(objectList->name,BAD_CAST"objectlist"))
			{
				objectList=objectList->next;
			}
			xmlNodePtr object=objectList->children;
			while (object!=NULL && xmlStrcmp(object->name,BAD_CAST"object"))
			{
				object=object->next;
			}
			while (object!=NULL)//object level
			{
				int ID;
				Rect res;
				double response;

				temp=xmlGetProp(object,BAD_CAST"confidence");
				if (temp!=NULL)
					response=string2float((char*)temp);
				else
					response=1;
				xmlFree(temp);

				temp=xmlGetProp(object,BAD_CAST"id");
				if(temp!=NULL)
					ID=string2int((char*)temp);
				else
					ID=-1;
				xmlFree(temp);

				

				xmlNodePtr box=object->children;
				while (xmlStrcmp(box->name,BAD_CAST"box") )
				{
					box=box->next;
				}
				temp=xmlGetProp(box,BAD_CAST"h");
				res.height=string2float((char*)temp)*ratio*h_ratio+0.5;
				xmlFree(temp);
				temp=xmlGetProp(box,BAD_CAST"w");
				res.width=string2float((char*)temp)*ratio*w_ratio+0.5;
				xmlFree(temp);
				temp=xmlGetProp(box,BAD_CAST"xc");
				res.x=(string2float((char*)temp)*ratio-0.5*res.width)+0.5;
				xmlFree(temp);
				temp=xmlGetProp(box,BAD_CAST"yc");
				res.y=(string2float((char*)temp)*ratio-0.5*res.height)+0.5;
				xmlFree(temp);

				output.push_back(ResultWindow(ID,res,response));
				object=object->next;
				while (object!=NULL && xmlStrcmp(object->name,BAD_CAST"object"))
				{
					object=object->next;

				}
			}
		}		
		if (frame!=NULL)
		{
			frame=frame->next;
		}
		while (frame!=NULL && xmlStrcmp(frame->name,BAD_CAST"frame"))
		{
			frame=frame->next;
		}
		return output;
	}

};


#endif