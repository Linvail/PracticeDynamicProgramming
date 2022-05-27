#include <iostream>
#include <vector>

using namespace std;

//! This returns the max length of all LCS.
int longestCommonSubsequence(string text1, string text2)
{
	const size_t lenText1 = text1.length();
	const size_t lenText2 = text2.length();

	const string& longerText = lenText1 > lenText2 ? text1 : text2;
	const string& shorterText = lenText1 <= lenText2 ? text1 : text2;
	const size_t& longerLen = lenText1 > lenText2 ? lenText1 : lenText2;
	const size_t& shorterLen = lenText1 <= lenText2 ? lenText1 : lenText2;

	if (0 == lenText1 || 0 == lenText2)
	{
		return 0;
	}

	const size_t rowLen = shorterLen;
	vector<int> prevRow(rowLen, 0);

	int maxLen = 0;
	int upperLeft = 0;

	// Put shorter inside, so we need smaller prevRow.
	for (size_t i = 0; i < longerLen; ++i)
	{
		for (size_t j = 0; j < shorterLen; ++j)
		{
			if (j == 0 || i == 0)
			{
				upperLeft = 0;
			}

			if (longerText[i] == shorterText[j])
			{


				int tempUpperLeft = prevRow[j];
				prevRow[j] = upperLeft + 1;
				upperLeft = tempUpperLeft;
			}
			else
			{
				upperLeft = prevRow[j];
				if (j != 0)
				{
					prevRow[j] = max(prevRow[j], prevRow[j - 1]);
				}
			}
		}
	}

	return prevRow.back();
}

int main()
{
	string text1("AGCAT");
	string text2("GAC");

	cout << "Longest Common Subsequence: " << longestCommonSubsequence(text1, text2) << "\n\n";

	text1 = "bsbininm";
	text2 = "jmjkbkjkv";

	cout << "Longest Common Subsequence: " << longestCommonSubsequence(text1, text2) << "\n\n";

	text1 = "abcba";
	text2 = "abcbcba";

	cout << "Longest Common Subsequence: " << longestCommonSubsequence(text1, text2) << "\n\n";
}