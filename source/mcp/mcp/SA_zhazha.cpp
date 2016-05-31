#include "mcp.h"
#include <windows.h>
#include <list>

static double T;					//�¶�
static list <int> in;				//�������е�ı��
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

	//��ʼ��&��ʼ�¶�
	mc_count = 1;
	mc_count_temp = 1;
	//��ʼ��Ϊ{0}
	mc_temp[0] = 1;
	in.push_back(0);
	//��ʼ���п������ŵĵ�Ϊ��0�����ĵ�
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
				// 		//��������ֱ�Ӱ��ķ�֮һ�ĵ��ӵ�
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
				// 				//����out
				// 				for (int i = 0; i < n; i++)
				// 				{
				// 					list <int>::iterator it = find(out.begin(), out.end(), i);
				// 					if (it == out.end() && !mc_temp[i])	//out�в�����i,in�в�����i
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
		//���Ӻ�ɾ���ĸ���
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

		if (rand() % 100 < seed)	//����һ����
		{

			//����in��out
			list<int> out_backup;
			for (list<int>::iterator i = out.begin(); i != out.end(); i++)
				out_backup.push_back(*i);

			//���һ����
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

			//��out����in
			in.push_back(*iter);
			mc_temp[*iter] = 1;
			out.erase(iter);

			//����out
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
			if (score_temp > score)		//�¶����ߣ����ʽ��ܽ�
			{
				//cout<<in.size()<<" " << score_temp << " " << score << endl;
				int seed = pow(2.718281828459, (score - score_temp) / T) * 1000;	//0-1000
				if (rand() % 1000 > seed)	//�����ܽ�
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
		else	//ɾ��һ����
		{
			int flag = (out.size() < 5);

			mc_count_temp--;
			//����in��out
			list<int> out_backup;
			int in_backup;
			for (list<int>::iterator i = out.begin(); i != out.end(); i++)
				out_backup.push_back(*i);

			//���һ����
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

			//����out
			for (int i = 0; i < n; i++)
			{
				list <int>::iterator it = find(out.begin(), out.end(), i);
				if (it == out.end() && !mc_temp[i])	//out�в�����i,in�в�����i
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
			if (score_temp > score&&!flag)		//�¶����ߣ����ʽ��ܽ�
			{
				int seed = pow(2.718281828459, (score - score_temp) / (double)T) * 1000;	//0-1000
				//cout << T << " " << seed << endl;
				if (rand() % 1000 > seed)	//�����ܽ�
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