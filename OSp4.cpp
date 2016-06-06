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

/*struct TimeLine
{
	int time;
	vector<int> waiting;
	int tutring;
};*/

struct Person
{
	int questionTime;
	int ID;
};

struct Data {
	//TimeLine timeline[10000];
	int tutoring;
	queue<int> waiting;
	Person person[31];
   	int numOfTutor;
   	int count[31];
};

int now;
int state = 0;
int count = 0;
int term = 0;
 
void* Threading(void* ptr) {
	Data *data = (Data *)ptr;

	mutex1.lock();
	int id = data->person[state].ID;		//told to every thread it's responce person
	Person *me = &data->person[id];

	state++;
	mutex1.unlock();

	while(state != 31)					//wait until every thread been told
		;

	while(1)
	{
		mutex1.lock();
		if (term < 31)
		{
			if (data->numOfTutor>=0)
			{
				//stdent routine section

				if (me->questionTime == 0)
					me->questionTime = rand()%31 + now;	 	//	random asking time
				//cout<<id<<" questionTime is "<<me->questionTime<<endl;
				//data->count[id]++;
				if (me->questionTime == now)
				{
					if (data->tutoring == 0)
						data->tutoring = id;
					else if (data->waiting.size()<3)
						data->waiting.push(id);
					else
						me->questionTime += 5;
				}
				// check the personal schedual first
				data->numOfTutor--;
			}
			else
			{
				mutex1.unlock();
				break;
			}
		}

		mutex1.unlock();
   	}

   	mutex1.lock();
   cout<<id<<" : "<<data->count[id]<<endl;			//count how many time the thread be in lock
   mutex1.unlock();
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
		data.person[i].ID = i;
		data.person[i].questionTime = 0;
		data.count[i] = 0;	
	}

	data.numOfTutor = 30;

	for(int i = 0; i < 31; )
		 pthread_create (&thread1[i++], NULL, Threading, &data); 
	
	for (int i = 0; i < 31; ++i)
		pthread_join(thread1[i], NULL);
	

    return 0;
}