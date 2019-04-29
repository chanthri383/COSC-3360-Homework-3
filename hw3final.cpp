//g++ Chanthri_So_threads.cpp hw3 -lpthread
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

static pthread_cond_t condition[10] = {PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,
PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,
PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER};

static int positionBeingUsed;
static pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
static pthread_mutex_lock bsem;
static int numUsersWaitedForLock;
static pthread_mutex_t numUsersWaitedForLock_mutex;
static pthread_mutex_t decrementRequest;
static bool isDatabaseBeingUsed = false;

using namespace std;

struct RequestData
{
	int startGroup;
	int secondGroup;
	int userNumber;
	int userGroup;
	int position;
	int arrival;
	int duration;
	int totalRequest1;
	int totalRequest2;
	int currentUserGroup;
};
	
void *processRequest(void *request_void_ptr)
{
	pthread_mutex_lock(&bsem);
	int firstUser = 0;
	RequestData *rd;
	if((rd->startingGroup) == 1)
	{
		rd->secondGroup = 2;
	}
	else
	{
		rd->secondGroup = 1;
	}
	int userHoldingPosition;
	struct( rd*)request_void_ptr;
	cout << "User " << rd->userNumber << " arrives to the DBMS\n";
	if(rd->userNumber != rd->startingGroup)
	{
		cout << "User " << rd->userNumber << "is waiting due to its group\n";
		pthread_cond_wait(&empty, &bsem);
	}
	pthread_mutex_unlock(&bsem);
	
	pthread_mutex_lock(&bsem);
	if(firstUser > 0 && (rd->position) == positionBeingUsed)
	{
		cout << "User " << rd->userNumber << "is waiting because position " << rd->position << "is being used by " << userHoldPosition << endl;
		pthread_mutex_lock(&numUsersWaitedForLock_mutex);
		numUsersWaitedForLock++;
		pthread_mutex_unlock(&numUsersWaitedForLock_mutex);
		pthread_cond_wait(&condition[(rd->position - 1)],&position_mutex[(rd->position) - 1]);
	}
	pthread_mutex_lock(position_mutex[(rd->position) - 1]);
	cout << "User " << rd->userNumber << "is using position " << rd->position << "for " << rd->duration << "seconds\n";
	firstUser++;
	userHoldPosition = rd->userNumber;
	sleep(rd->duration);
	pthread_cond_signal(condition[(rd->position) - 1]);
	pthread_unlock(&bsem);
	
	
	pthread_mutex_lock(&bsem);
	cout << "User " << rd->userNumber << "finished its execution\n";
	if((rd->startGroup) == 1)
	{
		pthread_mutex_lock(&decrementRequest);
		totalRequest1--;
		pthread_mutex_unlock(&decrementRequest);
		if((rd->totalRequest1) == 0)
		{
			cout << "All users from Group 1 finished its execution\n";
			cout << "The users from Group 2 start their execution\n";
			pthread_cond_broadcast(&empty);
		}
	}
	else if((rd->startGroup == 2))
	{
		pthread_mutex_lock(&decrementRequest);
		totalRequest2--;
		pthread_mutex_unlock(&decrementRequest);
		if((rd->totalRequest2) <= 0)
		{
			cout << "All users from Group 2 finished its execution\n";
			cout << "The users grom Group 1 start their execution\n";
			pthread_cond_broadcast(&empty);	
		}	
	}
	if((rd->secondGroup) == 1)
	{
		pthread_mutex_lock(&decrementRequest);
		totalRequest1--;
		pthread_mutex_unlock(&decrementRequest);
		if(rd->totalRequest1 <= 0)
		{
			cout << "All users from Group 1 finished their execution\n";
		}		
	}
	else if((rd->secondGroup) == 2)
	{
		pthread_mutex_lock(&decrementRequest);
		totalRequest2--;
		pthread_mutex_unlock(&decrementRequest);
		if((rd->totalRequest2) == 0)
		{
			cout << "All users from Group 2 finished their execution\n";
		}
	}
	pthread_mutex_unlock(&bsem);
	return NULL;
}

int main()
{
	pthread_mutex_init(&bsem, NULL);
	int startingGroup = 0;
	int secondGroup = 0;
	RequestData rq[1000];
	queue<int> currentG;
	queue<int> userNum;
	queue<int> resourceUse; 
	queue<int> timeSpawn;
	queue<int> timeUseResource;
	int resourceUsing = 0;
	int time = 0;
	int timeUsingResource = 0;
	int userNumber = 0;
	int numRequest1 = 0;
	int numRequest2 = 0;
	
	cin >> startingGroup;
	if(startingGroup == 1)
	{
		second_Group = 2;
	}
	else
	{
		second_Group = 1;
	}
	
	while (cin >> currentGroup >> resourceUsing >> time >> timeUsingResource)
	{
		
		currentG.push(currentGroup);
		resourceUse.push(resourceUsing);
		timeSpawn.push(time);
		timeUseResource.push(timeUsingResource);
		userNumber++;
		userNum.push(userNumber);
		if(currentGroup == 1)
		{
			numRequest1++;
		}
		else
		{
			numRequest2++;
		}
	}
	for(int i = 0; i < userNumber; i++)
	{
		rq[i].secondGroup = second_Group;
		rq[i].startGroup = startingGroup;
		rq[i].userGroup = currentG.front();
		rq[i].userNumber = userNum.front();
		rq[i].position = resourceUse.front();
		rq[i].arrival = timeSpawn.front();
		rq[i].duration = timeUseResource.front();
		
		if(!userNum.isEmpty())
		{
			userNum.pop();	
		}
		
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
	sleep(1); 
	pthread_t *tid = new pthread_t[userNumer];
	for(int i = 0; i < userNumber; i++)
	{
		sleep(rq[i].arrival);
		if(pthread_create(&tid[i], NULL, processRequest,(void *)&rq[i]) 
		{
			throw runtime_error("Error creating thread\n");
			return 1;
		}
		sleep(rq[i].arrival);
	}
	
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
	
	cout << "Due to a locked position: " << numUsersWaitedForLock << endl;

	return 0;
}
