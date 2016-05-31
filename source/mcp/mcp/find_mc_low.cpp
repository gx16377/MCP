//������

#include "mcp.h"

static bool mc_temp[N_v];				//���������������
static int temp_mc_count = 0;			//��������������ŵ���

int find_mc_low_dfs(int l, int r)
{
	///����
	if (l == r)
	{
		///���ڵ�ǰ�������
		if (temp_mc_count > mc_count)
		{
			mc_count = temp_mc_count;
			memcpy(mc, mc_temp, N_v*sizeof(bool));
		}
		return 1;
	}

	///������֦
	if (temp_mc_count + r - l <= mc_count)
		return 0;

	for (int i = l; i < r; i++)
	{
		int flag = 1;
		///�����ǰ�����ж����ţ�����flag��0
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
		find_mc_low_dfs(i + 1, r);
	}
}

int find_mc_low(int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << i << endl;
		///������i���㵫�ǲ�����0~i-1�⼸����
		memset(mc_temp, 0, N_v*sizeof(char));
		mc_temp[i] = 1;
		temp_mc_count = 1;
		find_mc_low_dfs(i + 1, n);
		//cout << i + 1 << " " << n << endl;
	}
	return 1;
}