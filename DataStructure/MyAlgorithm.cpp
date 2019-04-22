#include<iostream>
#include<list>
#include<queue>
#include<functional>
#include<tuple>
using namespace std;
typedef tuple<int, int, int> tupleInt3;
typedef priority_queue<tupleInt3, vector<tupleInt3>, greater<tupleInt3>> pqTupleInt3;

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

#pragma region
void Kruskal()
{
	int node, p1 = 0, p2, dist, cursor, lineCount = 0;
	cout << "node의 갯수는? ";
	cin >> node;
	int *root = new int[node];

	for (int i = 0; i < node; ++i)
		root[i] = -1;

	pqTupleInt3 pq;
	tupleInt3 t;
	queue<tupleInt3> q;

	while (1)
	{
		cout << "간선 입력(점1 점2 거리 / 종료는 -1) : ";
		cin >> p1;

		if (p1 == -1)
			break;

		cin >> p2 >> dist;

		if (p1 >= node || p2 >= node)
		{
			cout << "잘못된 입력입니다." << endl;
			continue;
		}

		t = tupleInt3(dist, p1, p2);
		pq.push(t);
	}

	while (lineCount < node - 1)
	{
		t = pq.top();
		dist = get<0>(t);
		p1 = get<1>(t);
		p2 = get<2>(t);
		pq.pop();

		if (root[p1] == root[p2])
		{
			if (root[p1] == -1)
			{
				root[p1] = p1;
				root[p2] = p1;
			}
			else
				continue;
		}
		else if (root[p1] == -1)
			root[p1] = root[p2];
		else if (root[p2] == -1)
			root[p2] = root[p1];
		else
			for (int i = 0; i < node; ++i)
				if (root[p2] == root[i])
					root[i] = root[p1];

		q.push(t);
		++lineCount;
	}

	while (!q.empty())
	{
		t = q.front();
		q.pop();
		cout << get<1>(t) << "/" << get<2>(t) << "/" << get<0>(t) << endl;
	}
}
#pragma endregion Kruskal

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);

	Kruskal();
}