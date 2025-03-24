#include<iostream>
#include<queue>

using namespace std;

int main(void)
{
	queue<int> q;
	int n, k;

	cin >> n >> k;
	for (int i = 1; i < n + 1; i++)
	{
		q.push(i);
	}

	cout << '<';
	while (!q.empty())
	{
		for (int i = 0; i < k - 1; i++)
		{
			q.push(q.front());
			q.pop();
		}
		if (q.size() != 1)
		{
			cout << q.front() << "," << ' ';
			q.pop();
		}
		else 
		{
			cout << q.front();
			q.pop();
		}
		
	}
	cout << '>';
}







