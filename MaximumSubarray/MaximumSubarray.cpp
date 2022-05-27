// MaximumSubarray.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:

    static int maxSubArray( vector<int>& nums )
    {
        // sum[i] is the sum of nums[0] ~ num[i].
        vector<int> sum( nums.size(), INT_MIN );
        sum[0] = nums[0];
        int maxSum = sum[0];

        for( int i = 1; i < nums.size(); ++i )
        {
            // If sum[i - 1] < 0, it means all previous numbers are useless. They contribute nothing.
            // We should just set nums[i] as sum[i].
            if( sum[i - 1] > 0 )
            {
                sum[i] = sum[i - 1] + nums[i];
            }
            else
            {
                sum[i] = nums[i];
            }

            maxSum = std::max( maxSum, sum[i] );
        }

        return maxSum;
    }

    // Use less space. Try to use the original nums as the space for sum[].
    // O( N )
    static int maxSubArray_space_v1( vector<int>& nums )
    {
        // nums[i - 1] is the sum of nums[0] ~ num[i - 1].
        int maxSum = nums[0];

        for( int i = 1; i < nums.size(); ++i )
        {
            // If sum[i - 1] < 0, it means all previous numbers are useless. They contribute nothing.
            // We should just set nums[i] as sum[i].
            if( nums[i - 1] > 0 )
            {
                nums[i] = nums[i - 1] + nums[i];
            }

            maxSum = std::max( maxSum, nums[i] );
        }

        return maxSum;
    }

    // O( N )
    static int maxSubArray_v2( vector<int>& nums )
    {
        int maxSum = INT_MIN;
        int sum = 0;
        for( auto num : nums )
        {
            sum = max( sum + num, num );
            maxSum = max( maxSum, sum );
        }

        return maxSum;
    }

    // Divide and conquer approach. O(N log N)
    static int maxSubArray_v3( vector<int>& nums )
    {
        // Divide into two parts - left and right.
        // 1 1 2 2 3 3
        // If the count is even, don't consider the middle 2 numbers [2, 2]. Let the left array be [1, 1], the right array be [3, 3]
        // Procedure:
        // Call maxSubArray_v3_util recursively on the left/right array.
        // Calculate the maximum subarray that contains the middle number(s).
        // Return the maximum of the left, the right, and the maximum subarray that contains the middle number(s).

        return maxSubArray_v3_util( nums, 0, nums.size() - 1 );
    }

    static int maxSubArray_v3_util( const vector<int>& nums, int aLeft, int aRight )
    {
        if( aLeft >= aRight )
        {
            return nums[aRight];
        }

        int mid = ( aLeft + aRight ) / 2;

        // Find maximum subarray sum for the left subarray,
        // including the middle element
        int maxLeftFromMid = INT_MIN;
        int sum = 0;
        for( int i = mid; i >= 0; i-- )
        {
            sum += nums[i];
            if( sum > maxLeftFromMid )
            {
                maxLeftFromMid = sum;
            }
        }

        // Find maximum subarray sum for the right subarray,
        // excluding the middle element
        int maxRightFromMid = INT_MIN;
        sum = 0;
        for( int i = mid + 1; i <= aRight; i++ )
        {
            sum += nums[i];
            if( sum > maxRightFromMid )
            {
                maxRightFromMid = sum;
            }
        }

        int maxLeft = mid != 0 ? maxSubArray_v3_util( nums, aLeft, mid - 1 ) : nums[0];
        int maxRight = mid != aRight ? maxSubArray_v3_util( nums, mid + 1, aRight ) : nums[aRight];

        // re-use sum to calcualte the final max.
        sum = max( maxLeft, maxRight );

        return max( sum, maxRightFromMid > 0 ? maxRightFromMid + maxLeftFromMid : maxLeftFromMid );
    }

};

int main()
{
    vector<int> testData = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    //vector<int> testData = { 5, 4, -1, 7, 8 };
    //vector<int> testData = { -1, 0, -2 };

    //cout << "MaximumSubarray: " << Solution::maxSubArray( testData ) << endl;
    //cout << "MaximumSubarray: " << Solution::maxSubArray_space_v1( testData ) << endl;
    cout << "MaximumSubarray: " << Solution::maxSubArray_v2( testData ) << endl;
    cout << "MaximumSubarray: " << Solution::maxSubArray_v3( testData ) << endl;
}
