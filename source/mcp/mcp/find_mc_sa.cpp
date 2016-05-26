#include "mcp.h"
#include <time.h>

int c(bool s[], int n) {
	int num = 0;
	for (int i = 0;i < n;i++) {
		if (s[i]) num++;
	}
	return n - num;
}

void compare(bool s[],int n) {
	int num = 0;
	for (int i = 0;i < n;i++) {
		if (s[i]) {
			num++;
		}
	}
	if (num > mc_count) {
		cout << num << endl;
		for (int i = 0;i < n;i++) {
			mc[i] = s[i];
		}
		mc_count = num;
	}
}

int latest_add;
int latest_del;

void find_mc_sa(int n) {
	int t = n;//gap between initial value of t and range of c(S) effects efficiency a lot
	bool state[N_v];
	srand(time(0));
	for (int i = 0;i < n;i++) {
		state[i] = false;
	}
	state[0] = true;
	t = c(state, n);
	int dt;
	while (true) {
		int flag = 0;
		int i;
		int add[N_v];
		int add_num = 0;
		for (i = 0;i < n;i++) {
			if (state[i]) break;
		}
		for (int j = 0;j < n;j++) {
			if (adj_matrix[i][j] && !state[j]) {
				int can_be_added = 1;
				for (int k = i + 1;k < n;k++) {
					if (state[k] && !adj_matrix[k][j]) {
						can_be_added = 0;
						break;
					}
				}
				if (can_be_added&&j!=latest_del) {
					state[j] = true;
					dt = c(state, n) - t;
					float temp = (float)rand() / RAND_MAX;
					float temp2 = exp(-(float)dt / t);//should exp(-MAX/t) becomes 0?
					if (dt <= 0 || temp < temp2) {
						t = dt + t;
						flag = 1;
						latest_add = j;
						break;
					}
					state[j] = false;
				}
			}
		}

		//when those calculated, change to max-dt state or first proper state
		//if c(S) = v-num in clique and add_num>0 of course choose first

		if (!flag) {
			//remove
			compare(state,n);
			for (int i = 0;i < n;i++) {
				if (state[i]&&i!=latest_add) {
					state[i] = false;
					dt = c(state, n) - t;
					float temp = (float)rand() / RAND_MAX;
					float temp2 = exp(-(float)dt / t)/2;//should exp(-MAX/t) becomes 0?
					if (dt <= 0 || temp < temp2) {
						latest_del = i;
						t = dt + t;
						flag = 1;
						break;
					}
					state[i] = true;
				}
			}
		}
		if (!flag) {
			//compare and add to mc
			compare(state, n);
			return;
		}
	}
}