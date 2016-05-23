#include "mcp.h"

bool adj_matrix[N_v][N_v];		//邻接矩阵
bool mc[N_v];					//最大团
int mc_count = 0;				//最大团计数

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

	cout << "最大团包含点数：\n" << mc_count << endl;
	cout << "最大团包含点：\n";
	for (int i = 0; i < n; i++)
	{
		if (mc[i])
			cout << i + 1 << " ";
	}
	cout << endl;

	system("pause");




}