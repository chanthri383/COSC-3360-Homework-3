#include <iostream>
#include <thread>
#include <mutex>
#include <queue> //maybe have a queue for the condition variables
#include <condition_variable>

using namespace std;

int main()
{
	int startingGroup = 0;
	int currentGroup = 0;
	int resourceNumber = 0;
	int accessTime = 0;
	int timeOfActivity = 0;
	int userNumber = 1;
	//queue<int> currentG;
	//queue<int> resourceNum;
	//queue<int> accessT;
	//queue<int> user;
	//int userUsingResource = 0;
	//int currentResource = 0;
	//int timeActivity = 0;
	//int user
	
	
	
	while(cin >> currentGroup >> resourceNumber >>
				accessTime >> timeOfActivity)
				{
					
					user.push(userNumber); //user starts with user 1
					currentG.push(currentGroup);
					resouceNum.push(resourceNumber);
					accessT.push(accessTime);
					
					userNumber++;
				}
	
	return 0;
}
