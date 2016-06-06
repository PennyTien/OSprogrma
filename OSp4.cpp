#include <iostream>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <stdio.h> 
#include <stdlib.h>  //for ran
#include <pthread.h> 
#include <mutex>
#include <vector>
using namespace std;
 
mutex mutex1;
mutex mutex2;

struct TimeLine
{
	int time;
	vector<int> waiting;
	int tutring;
};

struct Person
{
	int ID;
	int schedual[100000];
};

struct Data {
	int now;
	TimeLine timeline[100000];
	Person person[31];
   	int numOfTutor;
   	int count[31];
};

int state = 0;
 
void* Threading(void* ptr) {
	Data *data = (Data *)ptr;

	mutex.lock();
	int *now = &data->now;
	TimeLine *timeline = data->timeline;

	int id = data->person[state].ID;		//told to every thread it's responce person
	Person *me = &data->person[id];

	state++;
	mu.unlock();

	while(state != 31)					//wait until every thread been told
		;

	while(1)
	{
		mu.lock();
		if (data->numOfTutor>=0)
		{
			//stdent routine section
			int randomtime = rand()%31 + *now;	 	//	random asking time
			// check the personal schedual first

			if (data->timeline[data->now].tutring == 0)	//if ta is naping
			{
				//just get in.
				//tutor is your id
				//update personal schedual
				//remember its 3 second
			}
			else if (data->timeline[data->now].waiting.size()<3)// the waiting line
			{
				//update personal scheual, timline, 
				//
			}
			else if (data->timeline[data->now].waiting.size()>=3)
			{
				//personal schedual from now to now+5 is -1
			}
		}
		else
		{
			mu.unlock();
			break;
		}
		mu.unlock();
   	}
   mutex.lock();


   cout<<id<<" : "<<data->count[id]<<endl;			//count how many time the thread be in lock
   mu.unlock();
   return NULL;
}

void* teachingAssistant(void* ptr) 
{
	mutex2.lock();
	while(sum!=15)
		;
	if  (sum == 15)
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
		data.count[i] = 0;	
	}
	data.numOfTutor = 100;
	data.now = 0;

	pthread_create (&thread1[0], NULL, teachingAssistant, &data); 
	for (int i = 1; i < 31; )
		 pthread_create (&thread1[i++], NULL, Threading, &data); 
	


	for (int i = 0; i < 30; ++i)
	{
		pthread_join(thread1[i], NULL);
	}

    return 0;
}