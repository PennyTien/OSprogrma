#include <iostream>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <stdio.h> 
#include <stdlib.h>  //for ran
#include <pthread.h> 
#include <mutex>
#include <vector>
using namespace std;
 
mutex mu;

struct TimeLine
{
	int time;
	vector<int> waiting;
	int tutring;
};

struct Person
{
	int ID;
	vector<int> schedual;
};

struct Data {
	int now;
	vector<TimeLine> timeline;
	Person person[31];
   	int numOfTutor;
   	int count[31];
};

int state = 0;
 
void* Threading(void* ptr) {
	Data *data = (Data *)ptr;

	mu.lock();
	int id = data->person[state].ID;		//told to every thread it's responce id
	state++;
	mu.unlock();

	while(state != 31)					//wait until every thread been told
		;

	while(1)
	{
		mu.lock();
		if (data->numOfTutor>=0)
		{
			cout<<data->numOfTutor<<": "<<pthread_self()<<": "<<id<<endl;
			data->count[id]++;
			data->numOfTutor--;
		}
		else
		{
			mu.unlock();
			break;
		}
		mu.unlock();
   	}
   mu.lock();


   cout<<id<<" : "<<data->count[id]<<endl;			//count how many time the thread be in lock
   mu.unlock();
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

	for (int i = 0; i < 31; )
		pthread_create (&thread1[i++], NULL, Threading, &data);  
	


	for (int i = 0; i < 30; ++i)
	{
		pthread_join(thread1[i], NULL);
	}

    return 0;
}