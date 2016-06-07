#include <iostream>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <stdio.h> 
#include <stdlib.h>  //for ran
#include <pthread.h> 
#include <mutex>
#include <vector>
#include <queue>
using namespace std;
 
mutex mutex1;
mutex mutex2;

/*剩下
1.waiting 只有一入隊伍時cout，目前每秒 cout 一次
	想改將 vector<int> waiting 改ㄓㄥ vector<Waiting> waiting
	waiting 裡有 int id 以及 int arriveTime
2.助教上廁所
3.助教兩位
4.排版*/

/*struct TimeLine
{
	int time;
	vector<int> waiting;
	int tutring;
};*/

struct Student
{
	int questionTime;
	int questionFrequency;
	int ID;
};

struct Data {
	//TimeLine timeline[10000];
	int tutoring;
	vector<int> waiting;
	Student student[31];
   	int numOfTutor;
   	int count[31];
};

int now;
int state = 0;
int count = 0;
int term = 0;
int temp = 2;

void printTime(int second);
 
void* Threading(void* ptr) {
	Data *data = (Data *)ptr;

	mutex1.lock();
	int id = data->student[state].ID;		//told to every thread it's responce Student
	Student *me = &data->student[id];

	state++;
	mutex1.unlock();

	while(state != 31)					//wait until every thread been told
		;

	while(1)
	{
		mutex1.lock();
		if (term < 30)					//Student
		{
			if (data->numOfTutor>=0)
			{
				//stdent routine section

				if (me->questionTime == 0 && me->questionFrequency == 0)
					me->questionTime = rand()%31 + now;	 	//	random asking time
				else if (me->questionTime == 0 && me->questionFrequency != 0)
					me->questionTime = (rand()%21+10) + now;

				//cout<<id<<" questionTime is "<<me->questionTime<<endl;
				data->count[id]++;
				if (me->questionTime == now)
				{
					if (data->tutoring == 0)
					{
						data->tutoring = id;
						me->questionTime = rand()%30 + now + 3;
					}
					else if (data->waiting.size()<3)
					{
						data->waiting.push_back(id);
						me->questionTime = rand()%30 + 3*(data->waiting.size()+1) + now;
					}
					else
						me->questionTime += 5;

				}
				// check the Studental schedual first
				//data->numOfTutor--;
			}
			else
			{
				mutex1.unlock();
				break;
			}
			term++;
		}
		else									//TA
		{
			cout<<"______________________________"<<endl;
			//cout<<now<<": "<<endl;
			if (data->tutoring == 0)
				cout<<"Nap . . .. "<<endl;
			else
			{
				if (temp == 2)
				{
					printTime(now);
					cout<<": "<<data->tutoring<<" is tutoring."<<endl;
					data->student[data->tutoring].questionFrequency++;
					data->numOfTutor--;
					temp--;
				}
				else if (temp > 0)
					temp--;
				else
				{
					printTime(now);
					cout<<": "<<data->tutoring<<"is finished. "<<endl;
					temp = 2;
					if (data->waiting.size()>0)
					{
						data->tutoring = data->waiting.front();
						vector<int>::iterator it = data->waiting.begin();
						data->waiting.erase(it);
						printTime(now);
						cout<<": "<<data->tutoring<<" is tutoring."<<endl;
						data->student[data->tutoring].questionFrequency++;
						data->numOfTutor--;
						temp--;
					}
				}


				if (data->waiting.size()>0)
					for (int i = 0; i < data->waiting.size(); ++i)
					{
						printTime(now);
						cout<<": "<<data->waiting[i]<<" is waiting"<<endl;
					}
				
				now++;
				term = 0;
			}
			cout<<"______________________________"<<endl;
		}

		mutex1.unlock();
   	}

   	//mutex1.lock();
   //cout<<id<<" : "<<data->count[id]<<endl;			//count how many time the thread be in lock
   //mutex1.unlock();
   return NULL;
}

void* teachingAssistant(void* ptr) 
{
	mutex2.lock();
	//while(sum!=15)
	//	;
	//if  (sum == 15)
	{
		cout<<"middle~~"<<endl;
		mutex2.unlock();
	}
	
    return NULL;
}
 
int main( ) {

	Data data;
	
	pthread_t *thread1 = new pthread_t[31] ;

	for (int i = 0; i < 31; ++i)
	{
		data.student[i].ID = i;
		data.student[i].questionTime = 0;
		data.student[i].questionFrequency = 0;
		data.count[i] = 0;	
	}

	data.numOfTutor = 60;

	for(int i = 0; i < 31; )
		 pthread_create (&thread1[i++], NULL, Threading, &data); 
	
	for (int i = 0; i < 31; ++i)
		pthread_join(thread1[i], NULL);
	

    return 0;
}

void printTime(int second)
{
	if (second<60)
	{
		cout<<"12:00:";
		if (second<10)
			cout<<0<<second;
		else
			cout<<second;
	}
	else if (second < 3600)
	{
		int minute = second/60;
		int s = second%60;
		cout<<"12:";
		if (minute<10)
			cout<<0<<minute<<":";
		else
			cout<<minute<<":";

		if (s<10)
			cout<<0<<s;
		else
			cout<<s;
	}
}












