#include "clearMOT.h"
using namespace std;
//#define  _PETS01_
void C_Mot::dealWith(vector<ResultWindow> gt,vector<ResultWindow> hp)
{
	frame_count++;
	matched_couple.clear();
	switched_couple.clear();
	miss_detection.clear();
	false_alarm.clear();
	//update gt_count
	gt_count+=gt.size();
	//book keeping of hypothesis
	map<int,int>::iterator hp_book_it;
	for (vector<ResultWindow>::iterator hp_it=hp.begin();hp_it!=hp.end();hp_it++)
	{
#ifdef _PETS01_
		if ((*hp_it).ID==8 || (*hp_it).ID==27)
		{
			continue;
		}
#endif
		hp_book_it=hp_book_keeping.find((*hp_it).ID);
		if (hp_book_it==hp_book_keeping.end())
		{
			hp_book_keeping.insert(pair<int,int>((*hp_it).ID,-1));
		}
	}	
	//first associate the already tracked objects
	vector<ResultWindow> gt_to_match;
	vector<ResultWindow> hp_to_match;
	map<int,int>::iterator gt_book_it;
	for (vector<ResultWindow>::iterator gt_it=gt.begin();gt_it!=gt.end();gt_it++)
	{
		bool flag_matched=false;
		gt_book_it=gt_book_keeping.find((*gt_it).ID);
		if (gt_book_it!=gt_book_keeping.end())
		{
			
			int last_match=(*gt_book_it).second;
			if (last_match>=0)
			{
				for (vector<ResultWindow>::iterator hp_it=hp.begin();hp_it!=hp.end();hp_it++)
				{
#ifdef _PETS01_
					if ((*hp_it).ID==8 || (*hp_it).ID==27)
					{
						continue;
					}
#endif
					if ((*hp_it).ID==last_match && getDistance((*hp_it),(*gt_it))<thresh_validation)
					{
						//match found
						match_count++;
						error_sum+=getDistance((*hp_it),(*gt_it));
						matched_couple.push_back((*gt_it));
						matched_couple.push_back((*hp_it));
						//*********************
						Rect r=(*gt_it).window;
						sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(*hp_it).response<<","<<1<<endl;
						//********************
						hp.erase(hp_it);
						flag_matched=true;
						break;						
					}						
				}
			}
		}
		else//new object, add to book keeping
		{
			gt_book_keeping.insert(pair<int,int>((*gt_it).ID,-1));
			tra_count++;
		}
		if (!flag_matched)//not matched, add to matching list
		{
			gt_to_match.push_back((*gt_it));
		}
	}
	for (int i=0;i<hp.size();i++)
	{
#ifdef _PETS01_
		if (hp[i].ID==8 || hp[i].ID==27)
		{
			continue;
		}
#endif
		hp_to_match.push_back(hp[i]);
	}
	//make the matrix for hungarian algorithm
	int gt_size=gt_to_match.size();
	int hp_size=hp_to_match.size();
	if (gt_size*hp_size>0)
	{
		Matrix<double> matrix(gt_size, hp_size+gt_size);
		for (int i=0;i<matrix.rows();i++)
		{
			for (int j=0;j<matrix.columns();j++)
			{
				if (j<hp_size)
				{
					double dis=getDistance(gt_to_match[i],hp_to_match[j]);
					matrix(i,j)=dis<thresh_validation ? dis:INFINITY;
				}
				else
				{
					matrix(i,j)=2.0;
				}
			}
		}
		//solve the bipartite matching
		Munkres m;
		m.solve(matrix);

		//arrange the outcome
		for (int i=0;i<matrix.rows();i++)
		{
			bool flag_matched=false;
			for (int j=0;j<hp_size;j++)
			{
				if (matrix(i,j)==0)
				{
					//match found
					flag_matched=true;
					match_count++;
					error_sum+=getDistance(gt_to_match[i],hp_to_match[j]);

					//check ID switch
					int gt_last_match=gt_book_keeping[gt_to_match[i].ID];
					if (!(gt_last_match==hp_to_match[j].ID || (gt_last_match==-1 && hp_book_keeping[hp_to_match[j].ID]==-1)))
					{
						IS_sum++;
						//cout<<"is"<<endl;
						//waitKey();
						switched_couple.push_back(gt_to_match[i]);
						switched_couple.push_back(hp_to_match[j]);
						//*********************
						Rect r=(hp_to_match[j]).window;
						sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<1<<endl;
						//*********************
					}
					else
					{
						matched_couple.push_back(gt_to_match[i]);
						matched_couple.push_back(hp_to_match[j]);
					}
					gt_book_keeping[gt_to_match[i].ID]=hp_to_match[j].ID;
					hp_book_keeping[hp_to_match[j].ID]=gt_to_match[i].ID;
					break;
				}
			}
			if (!flag_matched)
			{
				M_sum++;
				//cout<<"ms"<<endl;
				//waitKey();
				miss_detection.push_back(gt_to_match[i]);
			}		
		}
		for (int j=0;j<hp_size;j++)
		{
			bool flag_matched=false;
			for (int i=0;i<matrix.rows();i++)
			{
				if (matrix(i,j)==0)
				{
					flag_matched=true;
					break;
				}
			}
			if (!flag_matched)
			{
				FP_sum++;
				//cout<<"fp"<<endl;
				//waitKey();
				false_alarm.push_back(hp_to_match[j]);
				//*********************
				Rect r=(hp_to_match[j]).window;
				sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//*********************
			}
		}
	}
	else
	{
		if (gt_size==0)
		{
			for (int j=0;j<hp_size;j++)
			{
				FP_sum++;
				//cout<<"fp"<<endl;
				//waitKey();
				false_alarm.push_back(hp_to_match[j]);
				//*********************
				Rect r=(hp_to_match[j]).window;
				sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//*********************
			}
		}
		else
		{
			for (int i=0;i<gt_size;i++)
			{
				M_sum++;
				//cout<<"missing"<<endl;
				//waitKey();
				miss_detection.push_back(gt_to_match[i]);
			}
		}
	}
}


void C_Mot::dealWithDetection(vector<ResultWindow> gt,vector<ResultWindow> hp)
{
	frame_count++;
	matched_couple.clear();
	switched_couple.clear();
	miss_detection.clear();
	false_alarm.clear();
	//update gt_count
	gt_count+=gt.size();
	//book keeping of hypothesis
	//first associate the already tracked objects
	vector<ResultWindow> gt_to_match;
	vector<ResultWindow> hp_to_match;
	map<int,int>::iterator gt_book_it;
	for (vector<ResultWindow>::iterator gt_it=gt.begin();gt_it!=gt.end();gt_it++)
	{
		{
			gt_to_match.push_back((*gt_it));
		}
	}
	hp_to_match=hp;
	//make the matrix for hungarian algorithm
	int gt_size=gt_to_match.size();
	int hp_size=hp_to_match.size();
	if (gt_size*hp_size>0)
	{
		Matrix<double> matrix(gt_size, hp_size+gt_size);
		for (int i=0;i<matrix.rows();i++)
		{
			for (int j=0;j<matrix.columns();j++)
			{
				if (j<hp_size)
				{
					double dis=getDistance(gt_to_match[i],hp_to_match[j]);
					matrix(i,j)=dis<thresh_validation ? dis:INFINITY;
				}
				else
				{
					matrix(i,j)=2.0;
				}
			}
		}
		//solve the bipartite matching
		Munkres m;
		m.solve(matrix);

		//arrange the outcome
		for (int i=0;i<matrix.rows();i++)
		{
			bool flag_matched=false;
			for (int j=0;j<hp_size;j++)
			{
				if (matrix(i,j)==0)
				{
					//match found
					flag_matched=true;
					match_count++;
					error_sum+=getDistance(gt_to_match[i],hp_to_match[j]);

					//check ID switch
					
					//IS_sum++;
					//switched_couple.push_back(gt_to_match[i]);
					//switched_couple.push_back(hp_to_match[j]);
					//*********************
					Rect r=(hp_to_match[j]).window;
					sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<1<<endl;
					//cout<<frame_count<<"--"<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<1<<endl;
					//*********************
					matched_couple.push_back(gt_to_match[i]);
					matched_couple.push_back(hp_to_match[j]);
					
					break;
				}
			}
			if (!flag_matched)
			{
				M_sum++;
				miss_detection.push_back(gt_to_match[i]);
			}		
		}
		for (int j=0;j<hp_size;j++)
		{
			bool flag_matched=false;
			for (int i=0;i<matrix.rows();i++)
			{
				if (matrix(i,j)==0)
				{
					flag_matched=true;
					break;
				}
			}
			if (!flag_matched)
			{
				FP_sum++;
				false_alarm.push_back(hp_to_match[j]);
				//*********************
				Rect r=(hp_to_match[j]).window;
				sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//cout<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//getchar();
				//*********************
			}
		}
	}
	else
	{
		if (gt_size==0)
		{
			for (int j=0;j<hp_size;j++)
			{
				FP_sum++;
				false_alarm.push_back(hp_to_match[j]);
				//*********************
				Rect r=(hp_to_match[j]).window;
				sta_writer<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//cout<<frame_count<<"--"<<(r.x+0.5*r.width)<<","<<(r.y+r.height)<<","<<(hp_to_match[j]).response<<","<<-1<<endl;
				//getchar();
				//*********************
			}
		}
		else
		{
			for (int i=0;i<gt_size;i++)
			{
				M_sum++;
				miss_detection.push_back(gt_to_match[i]);
			}
		}
	}
}