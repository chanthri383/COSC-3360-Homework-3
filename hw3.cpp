#include <iostream>
#include <queue>

using namespace std;

/*
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
    * each request is handled by a new thread
    * request processing is simulated with sleep()
    * print messages whenever a request:
        a) arrives at the DBMS
        b) waits for the first group to finish (only printed for each message in the second group)
        c) waits for a database position (if it was locked)
        d) uses a database position
        e) finishes execution
    * print whenever the first group finishes and the second group begins

print summary after all requests have been processed
	* this is when both queues are empty
	* summary consists of:
		- total number of requests per group
		- number of requests waited due to:
			* the group they belong to
			* a position that was locked by another user

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
  	char groupLabel;
	queue<RequestInfo> requests;

  public:
	UserGroup(char label) {
		groupLabel = label;
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

	// add requests

	// process requests

	// print totals

	return 0;
}
