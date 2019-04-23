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
	* wait for all request threads to be ready by calling waitForGroupCond()
		- then the main thread can trigger broadcastGroupCond()
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
		
		pthread_mutex_t cond_mutex;
		pthread_cond_t cond;

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
		pthread_mutex_init(&(data->cond_mutex), &shared_mutexattr);
		pthread_mutex_init(&(data->numUsersWaitedForLock_mutex), &shared_mutexattr);

		pthread_condattr_t shared_condattr;
		pthread_condattr_init(&shared_condattr);
		pthread_condattr_setpshared(&shared_condattr, PTHREAD_PROCESS_SHARED);
		pthread_cond_init(&(data->cond), &shared_condattr);
	}
	~Shared()
	{
		shmdt(data);
	}

	// code to ensure only one instance exists
	static Shared &getInstance()
	{
		static Shared instance;
		return instance;
	}
	Shared(Shared const &) = delete;
	void operator=(Shared const &) = delete;

	// call this in main thread after everything that is shared is done being used
	void destroy()
	{
		pthread_mutex_destroy(&(data->print_mutex));
		pthread_mutex_destroy(&(data->numUsersWaitedForLock_mutex));
		shmctl(shmid, IPC_RMID, NULL);
	}

	void print(string s)
	{
		pthread_mutex_lock(&(data->print_mutex));
		cout << s << endl;
		cout.flush();
		pthread_mutex_unlock(&(data->print_mutex));
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

	void waitForGroupCond() {
		// you need a condition variable added to the SharedData struct
		// only children threads call this
		pthread_mutex_lock(&(data->cond_mutex));
		pthread_cond_wait(&(data->cond), &(data->cond_mutex));
		pthread_mutex_unlock(&(data->cond_mutex));
	}

	void broadcastGroupCond() {
		// only called by the main thread
		pthread_mutex_lock(&(data->cond_mutex));
		pthread_cond_broadcast(&(data->cond));
		pthread_mutex_unlock(&(data->cond_mutex));
	}
};

struct Requestdata
{
	int userNumber;
	int position;
	int arrival;
	int duration;

	Requestdata(int u, int p, int a, int d)
	{
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
	queue<Requestdata> requests;

	Requestdata dequeueRequest()
	{
		Requestdata r = requests.front;
		requests.pop();
		return r;
	}

  public:
	UserGroup(int n)
	{
		groupNumber = n;
	}

	void addRequest(int u, int p, int a, int d)
	{
		requests.push(Requestdata(u, p, a, d));
	}

	int getNumberOfRequests()
	{
		return requests.size();
	}

	void *processRequest(int startingGroup, Shared *shared) {
		if (startingGroup == groupNumber)
		{
			// process starting group

			// if this is the last request of the first group
			// shared->broadcastGroupCond();
		}
		else
		{
			shared->waitForGroupCond();

			// process ending group
		}
		// also remember to print when necessary
	}
};

int main(int argc, char *argv[])
{
	Shared *shared = &Shared::getInstance();

	// g[0] is group 1, g[1] is group 2
	UserGroup g[2] { UserGroup(1), UserGroup(2) };

	int startingGroup;
	int currentGroup, pos, a, d;
	int userNumber = 1;
	int numRequests[2] = {0, 0};

	cin >> startingGroup;

	while (cin >> currentGroup >> pos >> a >> d)
	{
		g[currentGroup - 1].addRequest(userNumber, pos, a, d);
		userNumber++;
	}

	numRequests[0] = g[0].getNumberOfRequests();
	numRequests[1] = g[1].getNumberOfRequests();
	
	// wait for all child processes to acquire the shared->data->cond
	sleep(1);

	// process requests

	// wait for child processes to finish

	// print totals

	shared->destroy();

	return 0;
}
