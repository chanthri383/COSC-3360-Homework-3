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
		pthread_mutex_init(&(data->numUsersWaitedForLock_mutex), &shared_mutexattr);
	}
	~Shared()
	{
		shmdt(data);
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
};

struct RequestInfo
{
	int userNumber;
	int position;
	int arrival;
	int duration;

	RequestInfo(int u, int p, int a, int d)
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
	queue<RequestInfo> requests;

  public:
	UserGroup(char n)
	{
		groupNumber = n;
	}

	void addRequest(int u, int p, int a, int d)
	{
		requests.push(RequestInfo(u, p, a, d));
	}

	RequestInfo dequeueRequest()
	{
		RequestInfo r = requests.front;
		requests.pop();
		return r;
	}
};

int main(int argc, char *argv[])
{
	UserGroup g1('1'), g2('2');
	UserGroup *sg, *eg;

	int startingGroup;
	cin >> startingGroup;
	if (startingGroup == 1)
	{
		sg = &g1;
		eg = &g2;
	}
	else
	{
		sg = &g2;
		eg = &g1;
	}

	// add requests

	// process requests

	// print totals

	return 0;
}
