#include<iostream>
using namespace std;

#pragma region
int Strcmp(string s1, string s2)
{
	int len1 = s1.length(), len2 = s2.length(), len = (len1 < len2) ? len1 : len2, ret = 0;

	for (int i = 0; i < len; ++i)
	{
		if (s1[i] < s2[i])
		{
			ret = -1;
			break;
		}
		else if (s1[i] > s2[i])
		{
			ret = 1;
			break;
		}
	}

	if (ret != 0)
		return ret;

	if (len1 > len2)
		return 1;
	else if (len1 < len2)
		return -1;
	else
		return 0;
}
#pragma endregion strcmp

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);
	string a = "bbbb", b = "bbbb";
	cout << Strcmp(a, b);
}