#include "PracticeDynamicProgramming.h"

#include <iostream>
#include <string>
#include <vector>

namespace DynamicProgramming
{
    using namespace std;

    // 647. Palindromic Substrings (Medium)
    // Given string s = ['i'"text"'j'>, s would be palindromic if
    // s[i] == s[j] and:
    // a. "text" is palindromic.
    // b. "text" is empty or just has one char.
    int countSubstrings(string s)
    {
        const size_t len = s.size();
        int result = 0;
        // Define isPalindromic[i][j] as whether s[i~j] substring is isPalindromic.
        // State transition equation:
        // isPalindromic[i][j] = s[i] == s[j] && ( j - i <= 2 || isPalindromic[i + 1][j - 1] )
        vector<vector<bool>> isPalindromic(len, vector<bool>(len, false));

        // Now we need to figure out how to build the table.
        // We need "i + 1" for "i", so we must iterate i from right to left.
        for (int i = len - 1; i >= 0; --i)
        {
            for (int j = i; j <= len - 1; ++j)
            {
                isPalindromic[i][j] = s[i] == s[j] && ( j - i <= 2 || isPalindromic[i + 1][j - 1] );
                if (isPalindromic[i][j])
                {
                    result++;
                }
            }
        }

        return result;
    }

    // 647. Palindromic Substrings (Medium)
    void TestPalindromicSubstrings()
    {
        string inputString("aaa");
        cout << "\n647. Palindromic Substrings (Medium) for " << inputString << " : ";
        cout << countSubstrings(inputString) << ". Expect: 6." << endl;
    }
}