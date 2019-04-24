#include <iostream>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>
#include <queue>

static int numRequest1;
static int numRequest2;

using namespace std;
read in all data at the beginning before doing any work
		- order elements in each queue by request arrival time
processing requests:
	* wait for all request threads to be ready by calling waitForGroupCond()
		- then the main thread can trigger broadcastGroupCond()
    * process all requests from the group that starts before doing the next group
		- only requests from the same group have the potential to lock resources from each other
    * each request is handled by a new thread

	struct SharedData {
		pthread_mutex_t position_mutex[10];
		pthread_mutex_t print_mutex;

		pthread_mutex_t cond_mutex;
		pthread_cond_t cond;

		int numUsersWaitedForLock;
		pthread_mutex_t numUsersWaitedForLock_mutex;
	};

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
	
		pthread_mutex_lock(&(data->cond_mutex));
		pthread_cond_broadcast(&(data->cond),&(data->cond_mutex));
		pthread_mutex_unlock(&(data->cond_mutex));
	}
};


struct Requestdata
{
	int userNumber;
	int position;
	int arrival;
	int duration;

	RequestInfo(int u, int p, int a, int d)
	Requestdata(int u, int p, int a, int d)
	{
		userNumber = u;
		position = p;
	}
class UserGroup
{
  private:
	int groupNumber;
	queue<RequestInfo> requests;
	queue<Requestdata> requests;

	Requestdata dequeueRequest()
	{
		Requestdata r = requests.front;
		requests.pop();
		return r;
	}

  public:
	UserGroup(char n)
	UserGroup(int n)
	{
		groupNumber = n;
	}

	void addRequest(int u, int p, int a, int d)
	{
		requests.push(RequestInfo(u, p, a, d));
		requests.push(Requestdata(u, p, a, d));
	}

	RequestInfo dequeueRequest()
	int getNumberOfRequests()
	{
		RequestInfo r = requests.front;
		requests.pop();
		return r;
		return requests.size();
	}

	void *processRequest(void *process) 
	{
		if (startingGroup == groupNumber)
		{
			// process starting group
			// if this is the last request of the first group
			// shared->broadcastGroupCond();
			pthread_mutex_lock(); //critical section
			
			if(groupNumber == 1)
			{
				if(numRequest1 == 0)
				{
					broadcastGroupCond();
				}
			}
			else if(groupNumber == 2)
			{
				if(numRequest2 == 0)
				{
					broadcastGroupCond();
				}
			}
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
	UserGroup g1('1'), g2('2');
	UserGroup *sg, *eg;
	Shared *shared = &Shared::getInstance();

	// g[0] is group 1, g[1] is group 2
	UserGroup g[2] { UserGroup(1), UserGroup(2) };

	int startingGroup;
	int currentGroup, pos, a, d;
	int userNumber = 1;
	int numRequests[2] = {0, 0};

	cin >> startingGroup;
	if (startingGroup == 1)

	while (cin >> currentGroup >> pos >> a >> d)
	{
		g[currentGroup - 1].addRequest(userNumber, pos, a, d);
		userNumber++;
	}
	else

	numRequests[0] = g[0].getNumberOfRequests();
	numRequests[1] = g[1].getNumberOfRequests();

	// wait for all child processes to acquire the shared->data->cond
	sleep(1);

	pthread_t *tid = new pthread_t[userNumber - 1];
	
	for (int i = 0; i < userNumber - 1; i++)
	{
		sg = &g2;
		eg = &g1;
		if (pthread_create(&tid[i], NULL, (function), (void *)&family[i]))
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
	}

	// add requests
	// wait for child processes to finish

	// process requests

	// print totals

	shared->destroy();
	delete [] tid;

	return 0;
}
