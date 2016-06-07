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
1.助教上廁所
2.助教兩位
*/

//我假設 學生出來與下一位學生來為同時間 所以椅子上依然為3人 所以無法排！


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
int temp = 2;

void printTime(int second);
void printIdentity(int id);
void tutor(int id);
void leave(int id);
 
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
		if (data->numOfTutor<=60)//！！這個放這邊 ta 收尾麻煩！
		{
			if ( id != 30 )										//Student
			{
				//stdent routine section

				//the time this student go to ask question
				if (me->questionTime == 0 && me->questionFrequency == 0)
					me->questionTime = rand()%31 + now;	 	//	first asking
				else if (me->questionTime == 0 && me->questionFrequency != 0)
					me->questionTime = (rand()%21+10) + now; 	//not the first time asking.

				//cout<<id<<" questionTime is "<<me->questionTime<<endl;
				data->count[id]++;

				if (me->questionTime == now)
				{
					if (data->tutoring == -1)//now don't have any one asking
					{
						data->tutoring = id;
						//me->questionTime = 0;
						//！突然想到，questionTime 的初始化得在問完問題再做！
						//me->questionFrequency++;
					}
					else if (data->waiting.size()<3)
					{
						WaitingLine temp;				//add to waiting line
						temp.ID = id;
						temp.arriveTime = now;
						data->waiting.push_back(temp);
						//me->questionTime = rand()%30 + 3*(data->waiting.size()+1) + now;
						//！突然想到，questionTime 的初始化得在問完問題再做！
						//現在就讓它停在 now 反正助教一定在這後面，下一次跑到這個 thead 一定已經跑過這一輪助教
					}
					else						//come back after 5 second.
						me->questionTime += 5;
				}


			}
			else													//TA
			{
				if (data->tutoring == -1)
				{
					printTime(now);
					cout<<"-TA: Napping"<<endl;
					//這裏做廁所時間 rand ％2來決定是否廁所
					//好像得做一個開關先在助教沒空得排隊也
				}
				else
				{
					if (temp == 2)//The first second the student tutoring.
					{
						//跟下面重複想辦法合併
						tutor(data->tutoring);
						/*printTime(now);
						cout<<"-Student";
						printIdentity(data->tutoring);
						cout<<": Asking--"<<data->numOfTutor<<endl;*/

						data->numOfTutor++;
						temp--;
					}
					else if (temp > 0)//The second second the student tutoring.
						temp--;
					else				//The thirsd second the student tutoring.
					{
						/*printTime(now);
						cout<<"-Student";
						printIdentity(data->tutoring);
						cout<<": Exit to programming"<<endl;*/
						leave(id);

						data->student[data->tutoring].questionFrequency++;
						//初始化
						data->student[data->tutoring].questionTime = 0;
						temp = 2;
						if (data->waiting.size()>0)
						{
							data->tutoring = data->waiting.front().ID;
							vector<WaitingLine>::iterator it = data->waiting.begin();
							data->waiting.erase(it);

							tutor(data->tutoring);
							/*printTime(now);
							cout<<"-Student";
							printIdentity(data->tutoring);
							cout<<": Asking--"<<data->numOfTutor<<endl;*/


							data->numOfTutor++;
							temp--;
						}
						else
							data->tutoring = -1;
					}


					if (data->waiting.size()>0)
						for (int i = 0; i < data->waiting.size(); ++i)
							if (data->waiting[i].arriveTime == now)
							{
								printTime(now);
								cout<<"-Student";
								printIdentity(data->waiting[i].ID);
								cout<<": Sitting #"<<i+1<<endl;
							}
				}
				//int m = 0;
				//cin>>m;
				sleep(1);
				now++;
			}

		}
		else
		{
			if (id == 30)
				{
						/*printTime(now);
						cout<<"-Student";
						printIdentity(data->tutoring);
						cout<<": Exit to programming"<<endl;*/
						leave(id);
						data->student[data->tutoring].questionFrequency++;
						printTime(now);
						cout<<"-TA: Exist."<<endl;
				}
			mutex1.unlock();
			break;
		}
		mutex1.unlock();
   	}

   /*mutex1.lock();
   cout<<id<<" : "<<data->count[id]<<", ask: "<<me->questionFrequency<<" times."<<endl;			//count how many time the thread be in lock
   mutex1.unlock();*/
   return NULL;
}
 
int main( )
{
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
	data.tutoring = -1;

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
void printIdentity(int id)
{
	if (id<10)
		cout<<0<<id;
	else
		cout<<id;
}

void tutor(int id)
{
	printTime(now);
	cout<<"-Student";
	printIdentity(id);
	cout<<": Asking"<<endl;//<<data->numOfTutor<<endl;
}

void leave(int id)
{
	printTime(now);
	cout<<"-Student";
	printIdentity(id);
	cout<<": Exit to programming"<<endl;
}


