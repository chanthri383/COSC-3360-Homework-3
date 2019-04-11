#include <iostream>
#include <thread>
#include <mutex>
#include <queue> 
#include <condition_variable>

pthread_mutex_t lock;

using namespace std;

int main()
{
	int startingGroup = 0;
	int currentGroup = 0;
	int resourceNumber = 0;
	int accessTime = 0;
	int timeOfActivity = 0;
	int userNumber = 1;
	queue<int> currentG;
	queue<int> resourceNum;
	queue<int> accessT;
	queue<int> user;
	int userUsingResource = 0;
	int currentResource = 0;
	int timeActivity = 0;
	int user
	int error; //not sure if this is needed 
	int i = 0; //for loop initializer
	
	while(cin >> currentGroup >> resourceNumber >>
				accessTime >> timeOfActivity)
				{
					
					currentG.push(currentGroup);
					resouceNum.push(resourceNumber);
					accessT.push(accessTime);
					
					userNumber++; //since there are n users accessing shared resouce 
							//also used to determine how many request threads
				}
	
	//does creating each thread happen during input redirection?
	pthread_t tid[userNumber]; //because we determine how many users based on input redirection
	
	return 0;
}
