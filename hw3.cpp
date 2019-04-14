#include <iostream>
#include <thread>
#include <mutex>
#include <queue> 
#include <condition_variable>
#include <sys/wait.h>
#include <unistd.h>

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
	int totalRequest1 = 0;
	int totalRequest2 = 0;
	int waitAmount = 0;
	
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
	
	cout << "Total Requests:" << endl;
	cout << "Group 1: " << totalRequest1 << endl;
	cout << "Group 2: " << totalRequest2 << endl;
	cout << endl;
	
	cout << "Requests that waited: " << endl;
	cout << "Due to its group: " << //variable that counts which group waited  or not the first line of text file << endl;
	cout << "Due to a locked position: " << //variable calculate locked position << endl;
		
	return 0;
}
