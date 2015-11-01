/*
Author:         Jianming Zhang
Note:           This application computes CLEAR MOT metric for 2D tracking.
                Objects swapping IDs will be counted as 2 ID switches. If you
                specify a sequence path for 'calMOT()', the matching results will
                be displayed frame by frame. 
                                        
                Green thin and thick boxes: good matches;
                Red thin boxes: missed targets; 
                Red thick boxes: false alarms;
                Yellow thin and thick boxes: ID swithes; 
*/



#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>

#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xmlmemory.h"

#include "clearMOT.h"
#include "dataReader.h"

#define GT_XML_FILE "PETS2009-S2L1.xml"
#define HP_XML_FILE "PETS09S2L1_result_opencv.xml"
#define SEQUENCE_PATH "C:\\Users\\jmzhang\\Downloads\\video_data\\Crowd_PETS09\\S2\\L1\\Time_12-34\\View_001\\" // optional


class SeqReader
{
	int frameCount;
	string path;
public:
	SeqReader(const string p):frameCount(0),path(p){}
	void readImg(Mat& frame)
	{
		string fileName=path+"frame_";
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

int calMOT(const char* path=NULL)
{
	ResultParser gt(GT_XML_FILE,1.0);
	ResultParser hp(HP_XML_FILE,    1.0,                    0.9,                    1.2);//you may scale the result bounding box: w=w*r*w_r, h=h*r*h_r
	//                              [ratio]					[width_ratio]			[height_ratio]
	C_Mot mot(1.0);//1.0: IOU threshold     

	if (path==NULL)
	{
		while(!gt.isEnd() && !hp.isEnd())
			mot.dealWith(gt.readNextFrame(),hp.readNextFrame());

		mot.getMOT();
		getchar();
		return 0;
	}

	SeqReader reader=SeqReader(string(path));
	Mat frame;
	namedWindow("multiTrack",CV_WINDOW_AUTOSIZE);

	reader.readImg(frame);
	if (frame.data==NULL)
	{
		cerr<<"fail to read sequence!"<<endl;
		return 0;
	}
	mot.dealWith(gt.readNextFrame(),hp.readNextFrame());
	while(!gt.isEnd() && !hp.isEnd())
	{
		mot.paintFrame(frame);
		imshow("multiTrack",frame);
		waitKey(1);
		mot.dealWith(gt.readNextFrame(),hp.readNextFrame());
		reader.readImg(frame);
	}
	mot.getMOT();
	getchar();
	return 0;
}

int main()
{
	calMOT(SEQUENCE_PATH);
	//calMOT();
	return 0;
}