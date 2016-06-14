#include <iostream>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <stdio.h> 
#include <stdlib.h>  //for ran
#include <pthread.h> 
//#include <mutex>
#include <vector>
#include <queue>
using namespace std;
 
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


/*剩下
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
   	int taWC;
   	bool busy;
};

int now;
int state = 0;
int sequence = 0;
int count = 0;
int temp = 2;

void printTime(int second);
void printIdentity(int id);
void tutor(int id, int num);
void leave(int id);
 
void* Threading(void* ptr) {
	Data *data = (Data *)ptr;

	pthread_mutex_lock(&mut);
	int id = data->student[state].ID;		//told to every thread it's responce Student
	Student *me = &data->student[id];

	state++;
	pthread_mutex_unlock(&mut);

	while(state != 31)					//wait until every thread been told
		;


	while(1)
	{
		pthread_mutex_lock(&mut);
		
		if (id == sequence)
		{

			if (data->numOfTutor<=60)
			{
				if ( id != 30 )										//Student
				{
					//stdent routine section

					//the time this student go to ask question
					if (me->questionTime == -1 && me->questionFrequency == 0)
						me->questionTime = rand()%31 + 2;	 	//	first asking
					else if (me->questionTime == -1 && me->questionFrequency != 0)
						me->questionTime = (rand()%21+10) + now; 	//not the first time asking.


					data->count[id]++;

					if (me->questionTime == now)
					{
						if (data->tutoring == -1)//now don't have any one asking
						{
							data->tutoring = id;

						}
						else if (data->waiting.size()<3 )
						{
							WaitingLine temp;				//add to waiting line
							temp.ID = id;
							temp.arriveTime = now;
							data->waiting.push_back(temp);
						}
						else						//come back after 5 second.
							me->questionTime += 5;
					}


				}
				else													//TA
				{
					if (data->tutoring == -1)
					{
						if (!data->busy)
						{	
							data->busy = rand()%2;	
							if (!data->busy && data->taWC == -1)
							{
								printTime(now);
								data->taWC = now+4;
								data->tutoring = -2;
								cout<<"-TA: go to WC"<<endl;
							}
							else
							{
								printTime(now);
								cout<<"-TA: Napping"<<endl;
							}
						}
							
					}
					else
					{
						if(data->tutoring != -2 || data->taWC == now )
						{
							data->busy = false;
							data->taWC = -1;
							if (temp == 2 && data->tutoring!= -2)//The first second the student tutoring.
							{

								tutor(data->tutoring, data->numOfTutor);
								data->numOfTutor++;
								temp--;
							}
							else if (temp > 0 && data->tutoring!= -2)//The second second the student tutoring.
								temp--;
							else				//The thirsd second the student tutoring.
							{
								if (data->tutoring != -2)
								{
									leave(data->tutoring);

									data->student[data->tutoring].questionFrequency++;
									//初始化
									data->student[data->tutoring].questionTime = -1;
								}

								if (data->waiting.size()>0)
								{
									if (data->tutoring == -2)
									{
										printTime(now);
										cout<<"-TA: back from WC"<<endl;
									}
									temp = 2;
									data->tutoring = data->waiting.front().ID;
									vector<WaitingLine>::iterator it = data->waiting.begin();
									data->waiting.erase(it);

									tutor(data->tutoring, data->numOfTutor);


									data->numOfTutor++;
									temp--;
								}
								else
									data->tutoring = -1;
							}
						}


						if (data->waiting.size()>0)
							for (size_t i = 0; i < data->waiting.size(); ++i)
								if (data->waiting[i].arriveTime == now)
								{
									printTime(now);
									cout<<"-Student";
									printIdentity(data->waiting[i].ID);
									cout<<": Sitting #"<<i+1<<endl;
								}
					}
					//sleep(1);
					now++;
				}

			}
			else
			{
				if (id == 30)
					{
						leave(data->tutoring);
						data->student[data->tutoring].questionFrequency++;
						printTime(now);
						cout<<"-TA: Exist."<<endl;
					}

				if (sequence != 30)
					sequence++;

				pthread_mutex_unlock(&mut);
				break;
			}

			if (sequence != 30)
				sequence++;
			else
				sequence = 0;

		}
		pthread_mutex_unlock(&mut);
   	}

   return NULL;
}
 
int main( )
{
	Data data;
	
	pthread_t *thread1 = new pthread_t[31] ;
	srand(clock());
	for (int i = 0; i < 31; ++i)
	{
		data.student[i].ID = i;
		data.student[i].questionTime = -1;
		data.student[i].questionFrequency = 0;
		data.count[i] = 0;	
	}

	data.numOfTutor = 0;
	data.tutoring = -1;
	data.taWC = -1;
	data.busy = false;

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

void tutor(int id, int numOfTutor)
{
	printTime(now);
	cout<<"-Student";
	printIdentity(id);
	cout<<": Asking ("<<numOfTutor<<")"<<endl;//<<data->numOfTutor<<endl;
}

void leave(int id)
{
	printTime(now);
	cout<<"-Student";
	printIdentity(id);
	cout<<": Exit to programming"<<endl;
}