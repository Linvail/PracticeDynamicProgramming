// PracticeDynamicProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "PracticeDynamicProgramming.h"
#include "LeetCodeUtil.h"

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
    // 1  = {[1]}
    // 11 = {[1] [1]} [11]}
    // 31 = {[3] [1]}
    //  ^
    // Define dp[i] : the number of decode ways of the string from s[0] ~ s[i].
    // 331
    //   ^
    // dp[i] = dp[i-1]. If s[i] != 0, s[i - 1] and s[i] cannot form 10 ~ 26.
    // 321
    //   ^
    // dp[i] = dp[i-1] + dp[i-2]. If s[i] != 0, s[i - 1] and s[i] can form 10 ~ 26.
    // 130
    //   ^
    // dp[i] = 0 if s[i] == 0, and s[i - 1] and s[i] cannot form 10 ~ 26.
    // 310
    //   ^
    // dp[i] = dp[i-2]. If s[i] == 0, s[i - 1] and s[i] can form 10 ~ 26.
    if (s.empty() || s[0] == '0') return 0;

    vector<int> dp(s.size() + 1);
    dp[0] = 1; // To facilitate the case 11. dp[2] = dp[1] + dp[0].

    for (int i = 1; i <= s.size(); ++i)
    {
        // Note that s is 0-indexed.
        dp[i] = s[i - 1] == '0' ? 0 : dp[i - 1];
        if (i > 1 && ( s[i - 2] == '1' || ( s[i - 2] == '2' && s[i - 1] <= '6' ) ))
        {
            // If s[i-1] and s[i-2] can form 10~26.
            dp[i] += dp[i - 2];
        }
    }

    return dp.back();
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
// 152. Maximum Product Subarray (Medium)
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

//---------------------------------------------------------------------------------------
// 140. Word Break II (Hard)
//---------------------------------------------------------------------------------------
// Build a trie (prefix tree) for those words.
// In a recursion function, scan letter on the input string, use trie to detect whether we get a word.
// If yes, insert that word into the wordPieces vector. Call recursion function on the remaining string
// (from i + 1 to the end).
// When the recursion function returns, pop back that word we just put, so that we can move on to process
// the next possible sentence.
// In the beginning of the recursion function, we check the starting index. If it reaches the end, it means
// we've scanned all letters. We got all word pieces for a sentence. So, put this vector into the
// sentence's vector (vector<vector<string>>).
class Solution140
{
public:

    // Trie, aka prefix tree.
    class TrieNode
    {
    public:
        inline TrieNode* getChild(char c)
        {
            return m_children[c - 'a'];
        }

        void createChild(char c)
        {
            m_children[c - 'a'] = new TrieNode();
        }

        void setIsWord(bool val, const string& s)
        {
            m_isWord = val;
            m_word = s;
        }

        bool isWord() const
        {
            return m_isWord;
        }

        string getWord() const
        {
            return m_word;
        }

    private:
        vector<TrieNode*> m_children{ 26, nullptr };
        bool m_isWord = false;
        string m_word;
    };

    vector<string> wordBreak(string s, vector<string>& wordDict)
    {
        m_trieRoot = new TrieNode;
        BuildTrie(wordDict);

        vector<vector<string>> setnVector;
        vector<string> wordPieces;
        vector<string> result;

        helper(s, 0, wordPieces, setnVector);

        for (const auto& setnV : setnVector)
        {
            string setn;
            for (int i = 0; i < setnV.size(); ++i)
            {
                setn += setnV[i];
                if (i != setnV.size() - 1)
                {
                    setn += " ";
                }
            }
            result.push_back(setn);
        }

        return result;
    }

private:

    void helper(const string& s, int start, vector<string>& wordPieces, vector<vector<string>>& setnVector)
    {
        if (start == s.size())
        {
            setnVector.push_back(wordPieces);
            return;
        }

        TrieNode* currNode = m_trieRoot;
        for (int i = start; i < s.size(); i++)
        {
            currNode = currNode->getChild(s[i]);
            if (!currNode)
            {
                break;
            }
            // catsanddog
            //   ^^
            if (currNode->isWord())
            {
                wordPieces.push_back(currNode->getWord());
                helper(s, i + 1, wordPieces, setnVector);
                // This is very important!
                // If we don't do so, the next possible sentence will be appended to the 1st sentence.
                // For example, when we get 'cats', we're going to insert 'cats'.
                // At the time, the wordPieces must be empty. The 'cat', 'sand', 'dog' must be popped.
                wordPieces.pop_back();
            }
        }
    }

    void BuildTrie(vector<string>& wordDict)
    {
        for (const auto& word : wordDict)
        {
            TrieNode* cur = m_trieRoot;
            for (int i = 0; i < word.size(); ++i)
            {
                if (!cur->getChild(word[i]))
                {
                    cur->createChild(word[i]);
                }
                cur = cur->getChild(word[i]);
            }
            cur->setIsWord(true, word);
        }
    }

    TrieNode* m_trieRoot = nullptr;
};


int main()
{
    cout << "Practice of dynamic programming!\n";

    TestPalindromicSubstrings();

    // 91. Decode Ways (Medium)
    // Input: s = "226"
    // Output: 3
    // Explanation : "226" could be decoded as "BZ" ( 2 26 ), "VF" ( 22 6 ), or "BBF" ( 2 2 6 ).
    // Input: 227(2) 2839(1) 1201234(3)
    cout << "\n91. Decode Ways: " << numDecodings("227") << endl;

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

    // 140. Word Break II (Hard)
    // Input: s = "catsanddog", wordDict = ["cat","cats","and","sand","dog"]
    // Output: ["cats and dog", "cat sand dog"]
    // Input: s = "pineapplepenapple", wordDict = ["apple","pen","applepen","pine","pineapple"]
    // Output: ["pine apple pen apple","pineapple pen apple","pine applepen apple"]
    inputStr = "pineapplepenapple";
    inputVS = { "apple","pen","applepen","pine","pineapple" };
    Solution140 sol140;
    auto resultVS = sol140.wordBreak(inputStr, inputVS);
    cout << "\n140. Word Break II (Hard) " << "for " << inputStr << " :" << endl;
    LeetCodeUtil::PrintVector(resultVS);

}
