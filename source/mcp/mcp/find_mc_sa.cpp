#include "mcp.h"

int c(vertice *s, int n) {
	int num = 0;
	for (vertice *v = s;v != nullptr;v = v->next) {
		num++;
	}
	return n - num;
}

void compare(vertice *s, int n) {
	int num = 0;
	for (vertice *v = s;v != nullptr;v = v->next) {
		num++;
	}
	if (num > mc_count) {
		cout << num << endl;
		memset(mc, 0, n*sizeof(bool));
		for (vertice *v = s;v != nullptr;v = v->next) {
			mc[v->i] = true;
		}
		mc_count = num;
	}
}

int latest_add;
int latest_del;

int add_num[N_v];
int del_num[N_v];
long long int add_sum;
long long int del_sum;

void find_mc_sa(int n) {
	//int t = n;//gap between initial value of t and range of c(S) effects efficiency a lot
	vertice *state,*free;
	vertice temp_v;

	vertice statelist[MAX_MC];
	free = &statelist[1];
	for (int i = 1;i < MAX_MC;i++) {
		statelist[i].next = i == MAX_MC - 1 ? nullptr : &statelist[i + 1];
	}
	
	state = &statelist[0];
	state->i = 0;
	state->next = nullptr;
	srand(time(0));
	int t = c(state, n);
	int dt;
	while (true) {
		int flag = 0;
		compare(state, n);
		for (int j = 0;j < n;j++) {
			int can_be_added = 1;
			for (vertice *v = state;v != nullptr;v = v->next) {
				if (!adj_matrix[v->i][j]) {
					can_be_added = 0;
					break;
				}
			}
			if (can_be_added&&j != latest_del&&add_num[j] <add_sum*2/n+100) {
				temp_v.i = j;
				temp_v.next = state;
				dt = c(&temp_v, n) - t;
				float temp = (float)rand() / RAND_MAX;
				float temp2 = exp(-(float)dt / t);//should exp(-MAX/t) becomes 0?
				if (dt <= 0 || temp < temp2) {
					t = dt + t;
					flag = 1;
					latest_add = j;
					add_num[j]++;
					add_sum++;
					vertice *v = free;
					free = free->next;
					v->i = j;
					v->next = state;
					state = v;
					break;
				}
			}
		}

		//when those calculated, change to max-dt state or first proper state
		//if c(S) = v-num in clique and add_num>0 of course choose first

		if (!flag) {
			//remove
			vertice *prev = nullptr;
			for (vertice *v = state;v != nullptr;v = v->next) {
				if (v->i == latest_add|| del_num[v->i] >del_sum*2/n + 100) {
					prev = v;
					continue;
				}
				vertice *del = v;
				if (prev == nullptr) {
					dt = c(state->next, n) - t;
				} else {
					prev->next = v->next;
					dt = c(state, n) - t;
				}
				float temp = (float)rand() / RAND_MAX;
				float temp2 = exp(-(float)dt / t);//should exp(-MAX/t) becomes 0?
				if (dt <= 0 || temp < temp2) {
					latest_del = v->i;
					del_num[v->i]++;
					del_sum++;
					t = dt + t;
					flag = 1;
					if (prev == nullptr)state=state->next;
					v->next = free;
					free = v;
					break;
				}
				if(prev!=nullptr)prev->next = v;
				prev = v;
			}
		}
		if (!flag) {
			compare(state, n);
			return;
		}
	}
}