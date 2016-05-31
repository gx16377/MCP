#include "mcp.h"
#include <windows.h>
#include <list>

static double T;					//温度
static list <int> in;				//团里所有点的编号
static list <int> out;
static int mc_count_temp;
static bool mc_temp[N_v];
static short taboo[N_v];

double judge()
{
	double temp = -(double)in.size() * 3000 - (double)out.size();
	if ((double)out.size() < 5)
		temp -= (double)out.size() * 1000;
	else
		temp -= 5000;
	return temp;

}

int find_mc_SA_zz(int n)
{
	srand(GetTickCount());

	//初始解&初始温度
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
	T = 10000;
	double score = judge();

	int round = 0;
	int mc_count_last = 1;
	while (1)
	{
		for (int i = 0; i < n; i++)
		{
			if (taboo[i] > 0)
				taboo[i]--;
		}

		/*		round++;*/
				// 		//看不给力直接把四分之一的点扔掉
				// 		if (round > 100)
				// 		{
				// 			if (mc_count_temp <= mc_count_last)
				// 			{
				// 				cout << mc_count << " " << mc_count_last << endl;
				// 				round = 0;
				// 
				// 				int k = in.size() / 4;
				// 				mc_count_temp -= k;
				// 
				// 				for (int i = 0; i < k; i++)
				// 				{
				// 					int x = rand() % in.size();
				// 					list <int>::iterator iter = in.begin();
				// 					for (int i = 0; i < x; i++)
				// 						iter++;
				// 					mc_temp[*iter] = 0;
				// 					in.erase(iter);
				// 				}
				// 				//更新out
				// 				for (int i = 0; i < n; i++)
				// 				{
				// 					list <int>::iterator it = find(out.begin(), out.end(), i);
				// 					if (it == out.end() && !mc_temp[i])	//out中不包含i,in中不包含i
				// 					{
				// 						for (list <int>::iterator j = in.begin(); j != in.end(); j++)
				// 						{
				// 							if (!adj_matrix[i][*j])
				// 								goto _break3;
				// 						}
				// 						out.push_back(i);
				// 					_break3:;
				// 					}
				// 				}
				// 				score = judge();
				// 
				// 				mc_count_last = in.size();
				// 			}
				// 			else
				// 			{
				// 				mc_count_last = in.size();
				// 				round = 0;
				// 			}
				// 		}

		if (round > 200)
		{
			cout << in.size() << " " << out.size() << " " << T << endl;
			round = 0;
		}
		round++;

		int out_count = 0;
		for (list<int>::iterator i = out.begin(); i != out.end(); i++)
		{
			if (taboo[*i] == 0)
				out_count++;
		}
		//增加和删除的概率
		int seed = (double)out.size() * 100 / (double)(in.size() + out.size());
		if (seed < 70)
			seed = 70;
		if (in.size() == 1)
			seed = 100;

		else if (out_count == 0)
		{
			if (T < 1600)
				break;
			else
				seed = 0;
		}

		else if (out_count == 1)
			seed = 100;

		// 		if (out.size() == 1 && flag == 2)
		// 			seed = 0;

		if (rand() % 100 < seed)	//增加一个点
		{

			//备份in和out
			list<int> out_backup;
			for (list<int>::iterator i = out.begin(); i != out.end(); i++)
				out_backup.push_back(*i);

			//随机一个点
			int x = rand() % out_count;
			list <int>::iterator iter = out.begin();
			for (int i = 0; i < x; i++)
			{
				while (taboo[*iter > 0])
					iter++;
				iter++;
			}

			mc_count_temp++;
			if (mc_count_temp > mc_count)
			{
				memcpy(mc, mc_temp, sizeof(mc));
				mc_count = mc_count_temp;
				mc[*iter] = 1;
			}

			int iter_backup = *iter;
			if (taboo[iter_backup] > 0)
			{
				mc_count_temp--;
				continue;
			}

			//从out移入in
			in.push_back(*iter);
			mc_temp[*iter] = 1;
			out.erase(iter);

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

			int score_temp = judge();
			if (score_temp > score)		//温度升高，概率接受解
			{
				//cout<<in.size()<<" " << score_temp << " " << score << endl;
				int seed = pow(2.718281828459, (score - score_temp) / T) * 1000;	//0-1000
				if (rand() % 1000 > seed)	//不接受解
				{
					mc_temp[in.back()] = 0;
					in.pop_back();
					out.clear();
					for (list<int>::iterator i = out_backup.begin(); i != out_backup.end(); i++)
						out.push_back(*i);
					score_temp = score;
					mc_count_temp--;
				}
				else
					taboo[iter_backup] = in.size() / 2;

			}
			else
				taboo[iter_backup] = in.size() / 2;
			score = score_temp;

		}
		else	//删除一个点
		{
			int flag = (out.size() < 5);

			mc_count_temp--;
			//备份in和out
			list<int> out_backup;
			int in_backup;
			for (list<int>::iterator i = out.begin(); i != out.end(); i++)
				out_backup.push_back(*i);

			//随机一个点
			int x = rand() % in.size();
			list <int>::iterator iter = in.begin();
			for (int i = 0; i < x; i++)
				iter++;
			in_backup = *iter;

// 			if (taboo[in_backup] > 0)
// 			{
// 				mc_count_temp++;
// 				continue;
// 			}
			in.erase(iter);
			mc_temp[in_backup] = 0;

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

			int score_temp = judge();
			if (score_temp > score&&!flag)		//温度升高，概率接受解
			{
				int seed = pow(2.718281828459, (score - score_temp) / (double)T) * 1000;	//0-1000
				//cout << T << " " << seed << endl;
				if (rand() % 1000 > seed)	//不接受解
				{
					mc_temp[in_backup] = 1;
					in.push_back(in_backup);
					out.clear();
					for (list<int>::iterator i = out_backup.begin(); i != out_backup.end(); i++)
						out.push_back(*i);
					score_temp = score;
					mc_count_temp++;
				}
				else
					taboo[in_backup] = out.size();

			}
			else
				taboo[in_backup] = out.size();
			score = score_temp;
		}

		if (T < 3000)
			T *= 0.99999;
		else
			T *= 0.9999;
	}
}