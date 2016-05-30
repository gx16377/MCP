#include "mcp.h"
#include <windows.h>
#include <list>


static list <int> in;				//团里所有点的编号
static list <int> out;
static int mc_count_temp;
static bool mc_temp[N_v];
static bool taboo_bo[N_v];
static list<int> taboo_list;
static int len;

int judge(int x)
{
	int temp = 0;
	for (list<int>::iterator i = out.begin(); i != out.end(); i++)
		if (adj_matrix[x][*i])
			temp++;
	if (in.size()<90)
	{
		if (GetTickCount() % 1000 < 300)
			return rand();
		else
			return temp;
	}
	else
	{
		if (GetTickCount() % 1000 < 700)
			return rand();
		else
			return temp;
	}

// 	if (in.size() < 90)
// 		return temp;
// 	else
// 		return rand();
}


int find_mc_TS_zz(int n)
{
	srand(GetTickCount());
	//初始化
	mc_count = 1;
	mc_count_temp = 1;
	//初始团为{0}
	mc_temp[0] = 1;
	in.push_back(0);
	//初始所有可能入团的点为和0相连的点
	for (int i = 0; i < n; i++)
	{
		if (adj_matrix[0][i])
			out.push_back(i);
	}
	len = out.size() - 1;

	int round = 0;
	int round_100000 = 0;
	int last_count = mc_count;

	//迭代
	while (1)
	{
		//找能加入的
		int out_size = 0;
		int _max = -1;
		list<int>::iterator pick;
		int _pick;
		for (list<int>::iterator i = out.begin(); i != out.end(); i++)
		{
			if (!taboo_bo[*i])
			{
				out_size++;
				int temp = judge(*i);
				if (temp > _max)
				{
					_max = temp;
					memcpy(&pick, &i, sizeof(list<int>::iterator));
				}
				else if (temp == _max)
				{
					if (rand() % 100 < 30)
						memcpy(&pick, &i, sizeof(list<int>::iterator));
				}
			}
		}
		round++;
		round_100000++;
		if (round > 500)
		{
			cout << in.size() << " " << out.size() << " " << taboo_list.size() << endl;
			round = 0;
		}

		//增加
		if (out_size > 0)
		{
			//out移入in
			_pick = *pick;
			in.push_back(*pick);
			mc_temp[*pick] = 1;
			out.erase(pick);
			mc_count_temp++;

			if (mc_count_temp > mc_count)
			{
				memcpy(mc, mc_temp, sizeof(mc));
				mc_count = mc_count_temp;
				if (mc_count > 87)
				{
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
			}

			//更新out
			for (list<int>::iterator i = out.begin(); i != out.end();)
			{
				for (list<int>::iterator j = in.begin(); j != in.end(); j++)
				{
					if (!adj_matrix[*i][*j])
					{
						i = out.erase(i);
						goto _break;
					}
				}
				i++;
			_break:;
			}

			//更新taboo
			//cout << *pick;
// 			taboo_bo[_pick] = 1;
// 			taboo_list.push_front(_pick);
			len = out.size() - 1;
			//if (len < 1)len = 1;
			// 			cout << taboo_list.size() << endl;
			// 			cout << len << endl;
			// 			cout << (int)taboo_list.size() - len << endl;
			for (int i = 0; i < (int)taboo_list.size() - len; i++)
			{
				if (taboo_list.size() > len)
				{
					taboo_bo[taboo_list.back()] = 0;
					taboo_list.pop_back();
				}
			}
		}

		//删除
		else
		{
			int del_flag = 1;
			if (round_100000 > 50000)
			{
				if (last_count < mc_count)
				{
					last_count = mc_count;
				}
				else
				{
					del_flag = in.size() / 4;
					cout << "------------------------del-----------------------";
				}
				round_100000 = 0;
			}
		del:;
			del_flag--;

			// 			list <int>::iterator iter;
			// 			int _min = -2147000000;
			// 			for (list<int>::iterator i = in.begin(); i != in.end(); i++)
			// 			{
			// 				int temp = judge(*i);
			// 				if (temp > _min)
			// 				{
			// 					_min = temp;
			// 					memcpy(&iter, &i, sizeof(list<int>::iterator));
			// 				}
			// 
			// 			}

						//随机一个点
			int x = rand() % in.size();
			list <int>::iterator iter = in.begin();
			for (int i = 0; i < x; i++)
				iter++;

			mc_temp[*iter] = 0;
			int _iter = *iter;
			in.erase(iter);
			mc_count_temp--;

			//更新out
			for (int i = 0; i < n; i++)
			{
				list <int>::iterator it = find(out.begin(), out.end(), i);
				if (it == out.end() && !mc_temp[i])	//out中不包含i,in中不包含i
				{
					for (list <int>::iterator j = in.begin(); j != in.end(); j++)
					{
						if (!adj_matrix[i][*j])
							goto _break2;
					}
					out.push_back(i);
				_break2:;
				}
			}

			//更新taboo
			taboo_bo[_iter] = 1;
			taboo_list.push_front(_iter);
			//len = out.size() - 1;
			if (len < 1)len = 1;
			for (int i = 0; i < (int)taboo_list.size() - len; i++)
			{
				if (taboo_list.size() > len)
				{
					taboo_bo[taboo_list.back()] = 0;
					taboo_list.pop_back();
				}
			}
			if (del_flag > 0)
				goto del;
		}


	}
}