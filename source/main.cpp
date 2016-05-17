#define N_v 1000

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

bool adj_matrix[N_v][N_v];
bool mc[N_v];

int find_mc_low_dfs(int l, int r)
{
	if (l == r)
	{
		for (int i = 0; i < r; i++)
		{
			printf("%d ", mc[i]);
		}
		printf("\n");
		return 1;
	}
	for (int i = l; i < r; i++)
	{
		int flag = 1;
		for (int j = 0; j < i; j++)
		{
			if (mc[j] && !adj_matrix[i][j])
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
		{
			mc[i] = 1;
			find_mc_low_dfs(i + 1, r);
			mc[i] = 0;
		}
		else
		{
			find_mc_low_dfs(i + 1, r);
		}
	}
}

int find_mc_low(int n)
{
	for (int i = 0; i < n; i++)
	{
		memset(mc, 0, sizeof(mc));
		mc[i] = 1;
		find_mc_low_dfs(i + 1, n);
		cout << i + 1 << " " << n << endl;
	}
	return 1;
}

int main()
{
	ifstream fin("in.txt");

	int n, m;
	cin >> n;
	cin >> m;

	char trash;
	int x, y;
	for (int i = 0; i < m; i++)
	{
		//cin >> trash;
		cin >> x >> y;
		adj_matrix[x][y] = 1;
		adj_matrix[y][x] = 1;
	}

	find_mc_low(n);
	system("pause");




}