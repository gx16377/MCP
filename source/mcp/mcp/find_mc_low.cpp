//渣渣法

#include "mcp.h"

bool mc_temp[N_v];				//本次搜索的最大团
int temp_mc_count = 0;			//本次搜索的最大团点数

int find_mc_low_dfs(int l, int r)
{
	///出口
	if (l == r)
	{
		///多于当前的则更新
		if (temp_mc_count > mc_count)
		{
			mc_count = temp_mc_count;
			memcpy(mc, mc_temp, N_v*sizeof(bool));
		}
		return 1;
	}

	///渣渣剪枝
	if (temp_mc_count + r - l <= mc_count)
		return 0;

	for (int i = l; i < r; i++)
	{
		int flag = 1;
		///必须和前面所有都连着，否则flag变0
		for (int j = 0; j < i; j++)
		{
			if (mc_temp[j] && !adj_matrix[i][j])
			{
				flag = 0;
				break;
			}
		}

		if (flag == 1)
		{
			mc_temp[i] = 1;
			temp_mc_count++;
			find_mc_low_dfs(i + 1, r);
			mc_temp[i] = 0;
			temp_mc_count--;
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
		///包含第i个点但是不包含0~i-1这几个点
		memset(mc_temp, 0, N_v*sizeof(char));
		mc_temp[i] = 1;
		temp_mc_count = 1;
		find_mc_low_dfs(i + 1, n);
		//cout << i + 1 << " " << n << endl;
	}
	return 1;
}