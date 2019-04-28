g++ Chanthri_So_threads.cpp hw3 -Ipthread
#include <iostream>
#include <string>
#include <sstream>
#include <semaphore.h>
#include <pthread.h>
#include <cstring> 
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>
#include <queue>


using namespace std;


pthread_mutex_t position_mutex[10];
pthread_mutex_t print_mutex;
int numUsersWaitedForLock;
pthread_mutex_t numUsersWaitedForLock_mutex;

struct RequestData
{
	int startGroup;
	int userNumber;
	int position;
	int arrival;
	int duration;
	int totalRequest1;
	int totalRequest2;
};
	
void *processRequest(void *request_void_ptr)
{
	RequestData *rd;
	struct( rd*)request_void_ptr;
	pthread_mutex_lock(&(data->print_mutex); 
	cout << "User " << rd->userNumber << "is entering the database\n";
	pthread_mutex_unlock(&(data->print_mutex);
	
	if(rd->userNumber != rd->startingGroup)
	{
		pthread_mutex_lock(&(data->print_mutex); 
		cout << "User " << rd->userNumber << "is waiting due to its group\n";
		pthread_mutex_unlock(&(data->print_mutex);
		shared->waitForGroupCond();
	}
	
	switch(rd->position)
	{
		/*
		case 1:
			pthread_mutex_lock(&(data->position_mutex[0]));
			
			pthread_mutexLock
			break;
			
		case 2:
			pthread_mutex_lock(&(data->position_mutex[1]));
			break;
			
		case 3:
			pthread_mutex_lock(&(data->position_mutex[2]));
			break;
			
		case 4:
			pthread_mutex_lock(&(data->position_mutex[3]));
			break;
			
		case 5:
			pthread_mutex_lock(&(data->position_mutex[4]));
			break;
			
		case 6:
			pthread_mutex_lock(&(data->position_mutex[5]));
			break;
			
		case 7:
			pthread_mutex_lock(&(data->position_mutex[6]));
			break;
			
		case 8:
			pthread_mutex_lock(&(data->position_mutex[7]));
			break;
			
		case 9:
			pthread_mutex_lock(&(data->position_mutex[8]));
			break;
		case 10:
			pthread_mutex_lock(&(data->position_mutex[9]));
			break;
	}
	*/
	pthread_mutex_lock(&(data->print_mutex); 
	cout << "User " << rd->userNumber << "finished its execution\n";
	pthread_mutex_unlock(&(data->print_mutex);
	if((rd->totalRequest1) == 0)
	{
		
	}
		
	pthread_exit(NULL);
	return NULL;
}

//can only pass one parameter for processRequest so must cast to struct
//need to encapsulate the data so we can allow processRequest() to have
//one parameter in which we cast it into a struct
int main(int argc, char *argv[])
{
	int startingGroup;
	Shared *shared = &Shared::getInstance();
	int currentGroup = 0;
	RequestData rq[1000];
	RequestData totalR;
	queue<int> groupCurrently;
	queue<int> resourceUsing; 
	queue<int> timeSpawn;
	queue<int> timeUseResource;
	int resourceUsing = 0;
	int time = 0;
	int timeUsingResource = 0;
	int userNumber = 0;
	int numRequest1 = 0;
	int numRequest2 = 0;
	
	cin >> startingGroup;
	
	while (cin >> currentGroup >> resourceUsing >> time >> timeUsingResource)
	{
		pthread_mutex_init(&bsem, NULL);
		groupCurrently.push(currentGroup);
		resourceUse.push(resourceUsing);
		timeSpawn.push(time);
		timeUseResource.push(timeUsingResource);
		userNumber++;
		if(currentGroup == 1)
		{
			numRequest1++;
			totalR.totalRequest1++;
		}
		else
		{
			numRequest2++;
			totalR.totalRequest2++;
		}
	}
	


	for(int i = 0; i < userNumber; i++)
	{
		rq[i].startGroup = startingGroup;
		rq[i].userNumber = groupCurrently.front();
		rq[i].position = resourceUse.front();
		rq[i].arrival = timeSpawn.front();
		rq[i].duration = timeUseResource.front();
		if(!groupCurrently.isEmpty())
		{
			groupCurrently.pop();
		}
		
		if(!resourceUse.isEmpty())
		{
			resourceUse.pop();
		}
		
		if(!timeSpawn.isEmpty())
		{
			timeSpawn.pop();
		}
		
		if(!timeUseResource.isEmpty())
		{
			timeUseResource.pop();
		}
	}
	sleep(1); //sleep just to ensure data was stored successfully
	pthread_t *tid = new pthread_t[userNumer]; // is it okay to do dynamically 
	
	for(int i = 0; i < userNumber; i++)
	{
		
		if(pthread_create(&tid[i], NULL, processRequest,(void *)&rq[i]) 
		{
			throw runtime_error("Error creating thread\n");
			return 1;
		}
		
		sleep(rq[i].arrival);
		//this ensures we access the struct and sleep based on each index of array struct and easy access
	}
	
	//wait for other threads to finish 
	for(int i = 0; i < userNumber - 1; i++)
	{
		pthread_join(tid[i], NULL]);	
	}	
	
	cout << "Total Requests: " << endl;
	cout << "Group 1: " << 	numRequest1 << endl;
	cout << "Group 2: " << 	numRequest2 << endl;
	cout << endl;

	cout << "Requests that waited: " << endl;
	if (startingGroup == 1)
	{
		cout << "Due to its group: " << numRequest2 << endl;
	}
	else
	{
		cout << "Due to its group: " << numRequest1 << endl;
	}

	delete[] tid;
	return 0;
}
