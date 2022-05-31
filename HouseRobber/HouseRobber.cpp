// HouseRobber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

// 198. House Robber (Medium)
class Solution
{
public:

    // Recursive method will make Time Limit Exceeded.
    static int rob( vector<int>& nums )
    {
        // n house
        // pick ( n - 1 ), cannot pick n
        // pick ( n - 2 ), can pick n
        if (nums.size() == 1)
        {
            return nums[0];
        }

        nums[1] = max( nums[0], nums[1] );
        for( int i = 2; i < nums.size(); ++i )
        {
            nums[i] = max( nums[i- 1], nums[i] + nums[i - 2]);
        }

        return nums.back();
    }
};

// 213. House Robber II (Medium)
class Solution198
{
public:
    int rob(vector<int>& nums)
    {
        if (nums.size() == 1)
        {
            return nums[0];
        }

        return max(rob(nums, 0, nums.size() - 1), rob(nums, 1, nums.size()));
    }

    int rob(vector<int>& nums, int start, int end)
    {
        // The accumulated money from beginning to this house, robbing this house.
        int sumRobThis = 0;
        // The accumulated money from beginning to this house, not robbing this house.
        int sumNotRobThis = 0;

        // Q   Q   Q   Q
        // nr  r
        //     ^
        for (int i = start; i < end; ++i)
        {
            int preSumRobThis = sumRobThis;
            int preSumNotRobThis = sumNotRobThis;

            // Going to rob this one, meaning the previous house cannot be robbed.
            sumRobThis = preSumNotRobThis + nums[i];
            // Not going to rob this one, so we should pick the max from the previous rob/not-rob.
            sumNotRobThis = max(preSumRobThis, preSumNotRobThis);
        }

        return max(sumRobThis, sumNotRobThis);
    }
};

int main()
{
    // 198. House Robber (Medium)
    // Input: nums = [1,2,3,1]
    // Output: 4
    // Input: nums = [2,7,9,3,1]
    // Output: 12
    vector<int> testData = { 1,2,3,1 };
    //vector<int> testData = { 2, 1, 1, 2 };
    //vector<int> testData = { 114, 117, 207, 117, 235, 82, 90, 67, 143, 146, 53, 108, 200, 91, 80, 223, 58, 170, 110, 236, 81, 90, 222, 160, 165, 195, 187, 199, 114, 235, 197, 187, 69, 129, 64, 214, 228, 78, 188, 67, 205, 94, 205, 169, 241, 202, 144, 240 };
    std::cout << "198. House Robber: " << Solution::rob( testData ) << endl;

    // 213. House Robber II (Medium)
    // Input: nums = [2, 3, 2]
    // Output : 3
    // Input: nums = [1]
    // Output: 1
    testData = { 1 };
    Solution198 sol198;
    std::cout << "213. House Robber II: " << sol198.rob(testData) << endl;
}
