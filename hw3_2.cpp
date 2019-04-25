#include <iostream>
#include <string>
#include <sstream>
#include <semaphore.h>
#include <pthread.h>
#include <cstring> // memset
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>
#include <queue>

static int sizeGroup1 = 0; 
static int sizeGroup2 = 0;


using namespace std;

/*
specifications:
get data ready:
* 10 different positions in the database ==> use a mutex for each
read in all data at the beginning before doing any work
* input comes from input redirection
* file format:
- first line: which user group starts; values: {1, 2}
- the rest of the lines: these are requests
* each request contains:
- user group number; values: {1, 2}
- database position; values: 1 through 10
- request arrival time; positive integer values
- request processing time; positive integer values
- userNumber is implied and perhaps unnecesary
* store requests into 2 queues, one for each user group
- each element of the queue contains: position, arrival, duration, and perhaps userNumber
- order elements in each queue by request arrival time
processing requests:
* process all requests from the group that starts before doing the next group
- only requests from the same group have the potential to lock resources from each other
* each request is handled by a new thread
* request processing is simulated with sleep()
* print messages whenever a request:
a) arrives at the DBMS
b) waits for the first group to finish (only printed for each message in the second group)
c) waits for a database position (if it was locked)
d) uses a database position
e) finishes execution
* preserve order when multiple requests are waiting for a single position
* print whenever the first group finishes and the second group begins
print summary after all requests have been processed
* this is when both queues are empty
* summary consists of:
- total number of requests per group
- number of requests waited due to:
* the group they belong to
* a position that was locked by another user
missing:
need counters for totals
need specifics on how processes are sharing memory
*/

// contains all variables that are shared between processes
class Shared
{
private:
	int shmid;

	struct SharedData {
		pthread_mutex_t position_mutex[10];
		pthread_mutex_t print_mutex;
		int numUsersWaitedForLock;
		pthread_mutex_t numUsersWaitedForLock_mutex;
	};
	SharedData *data;

public:
	Shared()
	{
		shmid = shmget(56785, sizeof(SharedData), 0600 | IPC_CREAT);
		data = (SharedData *)shmat(shmid, 0, 0);

		data->numUsersWaitedForLock = 0;

		// need to make sure the mutexes and condition variables are sharable in between processes
		pthread_mutexattr_t shared_mutexattr;
		pthread_mutexattr_init(&shared_mutexattr);
		pthread_mutexattr_setpshared(&shared_mutexattr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(&(data->print_mutex), &shared_mutexattr);
		pthread_mutex_t cond_mutex;
		pthread_mutex_init(&(data->cond_mutex), &shared_mutexattr);
		pthread_mutex_init(&(data->numUsersWaitedForLock_mutex), &shared_mutexattr);
		
		pthread_condattr_t shared_condattr;
		pthread_condattr_init(&shared_condattr);
		pthread_condattr_setshared(&shared_condattr, PTHREAD_PROCESS_SHARED);
		pthread_condattr_init(&(data->cond), &shared_condattr);
	}
	~Shared()
	{
		shmdt(data);
	}
	static Shared &getInstance() //makes sure there is one instance of this class
	{
		static Shared Instance;
		return instance;
	}
	Shared(Shared const &) = delete //overrides copy operator
	void operator=(Shared const &) = delete; //overrides equal operator 

	void destroy()
	{
		pthread_mutex_destroy(&(data->print_mutex));
		pthread_mutex_destroy(&(data->numUsersWaitedForLock_mutex));
		shmctl(shmid, IPC_RMID, NULL);	
	}
	
	string print(string s)
	{
		pthread_mutex_lock(&(data->print_mutex));
		pthread_mutex_unlock(&(data->print_mutex));
		return s;
	}

	void incrementNumUsersWaitedForLock()
	{
		pthread_mutex_lock(&(data->numUsersWaitedForLock_mutex));
		data->numUsersWaitedForLock++;
		pthread_mutex_unlock(&(data->numUsersWaitedForLock_mutex));
	}

	int getNumUsersWaitedForLock()
	{
		pthread_mutex_lock(&(data->numUsersWaitedForLock_mutex));
		int t = data->numUsersWaitedForLock;
		pthread_mutex_unlock(&(data->numUsersWaitedForLock_mutex));
		return t;
	}

	void waitForGroupCond(startingGroup, UserGroup group1, UserGroup group2)
	{
		pthread_mutex_lock(&(data->cond_mutex));
		pthread_cond_wait(&(data->cond), &(data->cond_mutex));
		pthread_mutex_lock(&(data->cond_mutex));
	}

	void broadcastGroupCond()
	{
		pthread_mutex_lock(&(data->cond_mutex));
		pthread_cond_wait(&(data->cond), &(data->cond_mutex));
		pthread_mutex_lock(&(data->cond_mutex));
	}
};

struct RequestData
{
	int startingGroup;
	int userNumber;
	int position;
	int arrival;
	int duration;
	int numRequestG1;
	int numRequestG2;

	RequestData(int b, int u, int p, int a, int d)
	{
		startingGroup = b;
		userNumber = u;
		position = p;
		arrival = a;
		duration = d;
	}
};

class UserGroup
{
private:
	int groupNumber;
	queue<RequestData> requests;
		RequestInfo dequeueRequest()
	{
		RequestInfo r = requests.front;
		requests.pop();
		return r;
	}

public:
	UserGroup(int n)
	{
		groupNumber = n;
	}

	void addRequest(int b, int u, int p, int a, int d)
	{
		requests.push(RequestData(b, u, p, a, d));
	}
	
	int getNumberOfRequests()
	{
		return requests.size();
	}

	
	void *processRequest(void *request_void_ptr)
	{
		//this function needs to know which group is starting to implement waitGroupCond()
		//instead of cast to char, must cast to struct 
		//because we are working with multiple different
		//we are locking a database position based on the user request
		RequestData rd;
		pthread_mutex_lock();
		//instead of cast to char, must cast to struct 
		//because we are working with multiple different
		if(rd.groupNumber != rd.startingGroup)
		{
			shared->waitForGroupCond();	
		}
		//if data is empty
		pthread_mutex_lock(&(data->print_mutex); //not sure if parameters are right
		cout << "User: " << rd.userNumber << "is entering the database\n";
		pthread_mutex_unlock(&(data->print_mutex);
		
		return NULL;
	}
};
//can only pass one parameter for processRequest so must cast to struct
//need to encapsulate the data so we can allow processRequest() to have
//one parameter in which we cast it into a struct
int main(int argc, char *argv[])
{
	UserGroup g[2]{ UserGroup(1), UserGroup(2) };
	int startingGroup;
	Shared *shared = &Shared::getInstance();
	pthread_mutex_init(&bsem, NULL); //initialize access to 1
	
	int currentGroup = 0;
	int resourceUsing = 0;
	int time = 0;
	int timeUsingResource = 0;
	int userNumber = 0;
	int numRequest[2] = { 0, 0 };
	cin >> startingGroup;
	
	while (cin >> currentGroup >> resourceUse >> time >> timeUsingResource)
	{
		g[currentGroup - 1].addRequest(startingGroup, currentGroup, resourceUse, time, timeUsingResource);
		userNumber++;
	}
	numRequest[0] = g[0].getNumberOfRequests();
	numRequest[1] = g[1].getNumberOfRequests();
	
	//wait for all child processes to acquire the shared->data->cond
	sleep(1);
	pthread_t *tid = new pthread_t[userNumer - 1]; // is it okay to do dynamically 
	
	for(int i = 0; i < userNumber - 1; i++)
	{
		
		if(pthread_create(&tid[i], NULL, processRequest,(void *)&family[i])) 
		{
			throw runtime_error("Error creating thread\n");
			return 1;
		}
		//we need to sleep based on which group and how many seconds
		sleep(1);
	}
	
	//wait for other threads to finish 
	for(int i = 0; i < userNumber - 1; i++)
	{
		pthread_join(tid[i], NULL]);	
	}	
	
	cout << "Total Requests: " << endl;
	cout << "Group 1: " << numRequest[0] << endl;
	cout << "Group 2: " << numRequest[1] << endl;
	cout << endl;

	cout << "Requests that waited: " << endl;
	if (startingGroup == 1)
	{
		cout << "Due to its group: " << numRequest[1].getNumberOfRequests << endl;
	}
	else
	{
		cout << "Due to its group: " << numRequest[0].getNumberOfRequests << endl;
	}

	shared->destroy();
	delete[] tid;
	return 0;
}
