#include <iostream>
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

class Shared {
	// ...
};

struct RequestInfo {
	int userNumber;
	int position;
	int arrival;
	int duration;

	RequestInfo(int u, int p, int a, int d) {
		userNumber = u;
		position = p;
		arrival = a;
		duration = d;
	}
};

class UserGroup {
  private:
  	int groupNumber;
	queue<RequestInfo> requests;

  public:
	UserGroup(char n) {
		groupNumber = n;
	}

	void addRequest(int u, int p, int a, int d) {
		requests.push(RequestInfo(u, p, a, d));
	}

	RequestInfo dequeueRequest() {
		RequestInfo r = requests.front;
		requests.pop();
		return r;
	}
};


int main(int argc, char* argv[])
{
	UserGroup g1('1'), g2('2');
	UserGroup* sg;

	int startingGroup;
	cin >> startingGroup;
	if (startingGroup == 1) {
		sg = &g1;
	} else {
		sg = &g2;
	}

	// add requests

	// process requests

	// print totals

	return 0;
}
