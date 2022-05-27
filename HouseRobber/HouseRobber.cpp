// HouseRobber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:

    // Recursive method will make Time Limit Exceeded.
    static int rob( vector<int>& nums )
    {
        // n house
        // pick ( n - 1 ), cannot pick n
        // pick ( n - 2 ), can pick n
        if( nums.size() == 1 )
        {
            return nums[0];
        }
        else if( nums.size() == 2 )
        {
            return max( nums[0], nums[1] );
        }

        nums[1] = max( nums[0], nums[1] );
        for( int i = 2; i < nums.size(); ++i )
        {
            nums[i] = max( nums[i- 1], nums[i] + nums[i - 2]);
        }

        return nums.back();
    }
};

int main()
{
    vector<int> testData = { 1, 2, 3, 1 };
    //vector<int> testData = { 2, 1, 1, 2 };

    //vector<int> testData = { 114, 117, 207, 117, 235, 82, 90, 67, 143, 146, 53, 108, 200, 91, 80, 223, 58, 170, 110, 236, 81, 90, 222, 160, 165, 195, 187, 199, 114, 235, 197, 187, 69, 129, 64, 214, 228, 78, 188, 67, 205, 94, 205, 169, 241, 202, 144, 240 };

    std::cout << "HouseRobber: " << Solution::rob( testData ) << endl;
}
