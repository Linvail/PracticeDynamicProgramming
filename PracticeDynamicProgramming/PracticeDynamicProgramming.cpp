// PracticeDynamicProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "PracticeDynamicProgramming.h"

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;
using namespace DynamicProgramming;

//---------------------------------------------------------------------------------------
// 91. Decode Ways (Medium)
//---------------------------------------------------------------------------------------
int numDecodings(string s)
{
    // --------------
    // 6: (6)
    // 86: (8 6)
    // --------------
    // 7: (7)
    // 27: (2 7)
    // --------------
    // 6 : (6)
    // 26 : (26) (2 6)
    // 226:  (2 2 6) (22 6) (2 26)
    // This is similar with: f(n) = f(n - 1) + f(n - 2)
    // --------------
    // 826: (8 26) (8 2 6)
    // --------------
    // 0 : put 0
    // 10 : (10)  put 1
    // 310: (3 10)
    // --------------
    // 6: (6)
    // 06: put 0
    // 206: (20 6)
    // --------------
    // 26: (26) (2 6)
    // 026 : put 0
    // 1026: (10 26) (10 2 6)
    // --------------
    // 9: (9)
    // 89: (8 9)
    // 289: (2 8 9)
    // --------------
    // dp[i] = dp[i + 1] + dp[i + 2] if 1 < s[i] <=2
    //       = dp[i + 1] if s[i] > 2
    //       = 0 if s[i] = 0
    //

    const int len = s.size();

    if (len == 0)
    {
        return 0;
    }
    else if (len == 1)
    {
        return s[0] == '0' ? 0 : 1;
    }

    vector<int> dp(len, 0);

    if (s[len - 1] == '0' && s[len - 2] != '0')
    {
        dp[len - 1] = 0;
        dp[len - 2] = s[len - 2] > '2' ? 0 : 1;
    }
    else if(s[len-2] == '0' && s[len - 1] != '0')
    {
        dp[len - 1] = 1;
        dp[len - 2] = 0;
    }
    else if(s[len - 1] != '0' && s[len - 2] != '0')
    {
        dp[len - 1] = 1;
        // If the number of two digits is greater than 26, then it is 1, otherwise 2.
        dp[len - 2] = s[len - 2] > '2' || ( s[len - 2] == '2' && s[len - 1] > '6' ) ? 1 : 2;
    }

    for (int i = len - 3; i >= 0; --i)
    {
        // If number of the last digits is not greater than 26
        if (s[i] == '0')
        {
            dp[i] = 0;
        }
        else if (s[i] <= '2' && ( s[i] < '2' || s[i + 1] < '7' )  )
        {
            dp[i] = dp[i + 1] + dp[i + 2];
        }
        else
        {
            dp[i] = dp[i + 1];
        }
    }

    return dp[0];
}

//---------------------------------------------------------------------------------------
// 139. Word Break (Medium)
// Topic: Hash table, DP, trie, memorization
//
//---------------------------------------------------------------------------------------
// Solution 1: Recursion with memorization
class Solution139
{
public:
    bool wordBreak(string s, vector<string>& wordDict)
    {
        // Must use memorization, otherwise time limit exceeded.
        // No reason to use trie, a set is enough.
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        // memo[i] means whether the substring s[i~n] can be segmented.
        // -1: unset. 1: true. 0: false.
        vector<int> memo(s.size(), -1);
        return checkSegment(s, wordSet, 0, memo);
    }

    bool checkSegment(string s, unordered_set<string>& wordSet, int start, vector<int>& memo)
    {
        if (start >= s.size())
        {
            return true;
        }
        if (memo[start] != -1)
        {
            return memo[start];
        }

        // "applepenapple"
        //       ^...
        for (int i = start + 1; i <= s.size(); ++i)
        {
            if (wordSet.find(s.substr(start, i - start)) != wordSet.end())
            {
                bool segmented = checkSegment(s, wordSet, i, memo);
                if (segmented)
                {
                    memo[start] = 1;
                    return segmented;
                }
            }
        }

        memo[start] = 0;
        return false;
    }
};

// Solution 2: DP
class Solution139_dp
{
public:
    bool wordBreak(string s, vector<string>& wordDict)
    {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        //  applepenapple
        // ^    ^  ^
        //      j  i
        // Define dp[i] : whether s.substr(0, i) can be segmented.
        // dp[i] = dp[j] && substr(j, i - j). j < i.
        // i starts from 1. dp[0] = true.
        vector<bool> dp(s.size() + 1, false);
        dp[0] = true;

        for (int i = 1; i <= s.size(); ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (dp[j] && wordSet.count(s.substr(j, i - j)) > 0)
                {
                    dp[i] = true;
                }
            }
        }

        return dp.back();
    }
};

//---------------------------------------------------------------------------------------
// 152. Maximum Product Subarray
//---------------------------------------------------------------------------------------
int maxProduct(vector<int>& nums)
{
    // This looks like a dynamic programming question but it seems that the space
    // complexity can be reduced to two variables.
    if (nums.size() == 0)
    {
        return 0;
    }

    int result = nums[0];
    // The max product ever seen when reaching i, position i included.
    int maxProduct = result;
    // The min product ever seen when reaching i, position i included.
    int minProduct = result;

    // [2  3  -2  4]
    //        ^
    for (int i = 1; i < nums.size(); ++i)
    {
        // This is a smart optimization.
        if (nums[i] < 0)
        {
            swap(maxProduct, minProduct);
        }

        // There is an important catch. When meeting a new number (nums[i]),
        // the max value could be maxProduct * nums[i] or nums[i].
        // For example, minProduct = -2, maxProduct = -1, nums[i] = 2.
        minProduct = min(nums[i], minProduct * nums[i]);
        maxProduct = max(nums[i], maxProduct * nums[i]);

        result = max(result, maxProduct);
    }

    return result;
}

int main()
{
    cout << "Practice of dynamic programming!\n";

    TestPalindromicSubstrings();

    // 91. Decode Ways (Medium)
    // Input: s = "226"
    // Output: 3
    // Explanation : "226" could be decoded as "BZ" ( 2 26 ), "VF" ( 22 6 ), or "BBF" ( 2 2 6 ).
    // Input: 227 2839 1201234
    cout << "\n91. Decode Ways: " << numDecodings("1201234") << endl;

    // 139. Word Break (Medium)
    // Input: s = "applepenapple", wordDict = ["apple","pen"]
    // Output: true
    // Input: s = "catsandog", wordDict = ["cats","dog","sand","and","cat"]
    // Output: false
    // "aaaaaaa", { "aaaa","aaa" }
    // Output: true
    Solution139_dp sol139;
    string inputStr = "aaaaaaa";
    vector<string> inputVS = { "aaaa","aaa" };
    cout << "\n139. Word Break: " << sol139.wordBreak(inputStr, inputVS);
    cout << endl;

    // 152. Maximum Product Subarray
    // Input: nums = [2,3,-2,4]
    // Output: 6
    vector<int> inputVI = { 2,3,-2,4 };
    cout << "\n152. Maximum Product Subarray: " << maxProduct(inputVI) << endl;
}
