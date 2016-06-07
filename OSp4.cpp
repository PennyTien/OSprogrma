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

struct WaitingLine{
	int ID;
	int arriveTime;
};

struct Data {
	int tutoring;
	vector<WaitingLine> waiting;
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
		if (data->numOfTutor<=60)
		{
			if ( id != 30)										//Student
			{
				//stdent routine section

				if (me->questionTime == 0 && me->questionFrequency == 0)
					me->questionTime = rand()%31 + now;	 	//	first asking
				else if (me->questionTime == 0 && me->questionFrequency != 0)
					me->questionTime = (rand()%21+10) + now; //rand()%21+10 = 10 - 30 sec

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
						WaitingLine temp;
						temp.ID = id;
						temp.arriveTime = now;
						data->waiting.push_back(temp);

						me->questionTime = rand()%30 + 3*(data->waiting.size()+1) + now;
					}
					else
						me->questionTime += 5;

				}
				// check the Studental schedual first
				//data->numOfTutor--;
			}
			else													//TA
			{
				//cout<<"______________________________"<<endl;
				//cout<<now<<": "<<endl;
				if (data->tutoring == 0)
				{
					printTime(now);
					cout<<"-TA: Napping"<<endl;
				}
				else
				{
					if (temp == 2)
					{
						printTime(now);
						cout<<"-Student"<<data->tutoring<<": Asking--"<<data->numOfTutor<<endl;
						data->student[data->tutoring].questionFrequency++;
						data->numOfTutor++;
						temp--;
					}
					else if (temp > 0)
						temp--;
					else
					{
						printTime(now);
						cout<<"-Student"<<data->tutoring<<" Exit to programming"<<endl;
						temp = 2;
						if (data->waiting.size()>0)
						{
							data->tutoring = data->waiting.front().ID;
							vector<WaitingLine>::iterator it = data->waiting.begin();
							data->waiting.erase(it);
							printTime(now);
							cout<<"-Student"<<data->tutoring<<": Asking--"<<data->numOfTutor<<endl;
							data->student[data->tutoring].questionFrequency++;
							data->numOfTutor++;
							temp--;
						}
					}


					if (data->waiting.size()>0)
						for (int i = 0; i < data->waiting.size(); ++i)
							if (data->waiting[i].arriveTime == now)
							{
								printTime(now);
								cout<<"-Student"<<data->waiting[i].ID<<" Sitting #"<<i+1<<endl;
							}
					term = 0;
				}
				sleep(1);
				now++;
			}

		}
		else
		{
			mutex1.unlock();
			break;
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

	data.numOfTutor = 0;

	for(int i = 0; i < 31; )
		 pthread_create (&thread1[i++], NULL, Threading, &data); 
	
	for (int i = 0; i < 31; ++i)
		pthread_join(thread1[i], NULL);
	

    return 0;
}

void printTime(int now)
{
	int hour = now / 3600;
	int minute = (now / 60) % 60 ;
	int second = now % 60;

	
	cout<<12+hour<<":";

	if (minute < 10)
		cout<<0<<minute<<":";
	else
		cout<<minute<<":";

	if (second<10)
		cout<<0<<second;
	else
		cout<<second;

}


