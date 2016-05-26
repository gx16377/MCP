#include "mcp.h"
#include <time.h>

typedef struct Vertice {
	int i;
	struct Vertice* next;
} vertice;

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



void find_mc_sa(int n) {
	//int t = n;//gap between initial value of t and range of c(S) effects efficiency a lot
	vertice *state;
	vertice head,temp_v;
	head.i = 0;
	head.next = nullptr;
	state = &head;

	srand(time(0));
	int t = c(state, n);
	int dt;
	while (true) {
		int flag = 0;
		for (int j = 0;j < n;j++) {
			int can_be_added = 1;
			for (vertice *v = state;v != nullptr;v = v->next) {
				if (!adj_matrix[v->i][j]) {
					can_be_added = 0;
					break;
				}
			}
			if (can_be_added&&j != latest_del) {
				temp_v.i = j;
				temp_v.next = state;
				dt = c(&temp_v, n) - t;
				float temp = (float)rand() / RAND_MAX;
				float temp2 = exp(-(float)dt / t);//should exp(-MAX/t) becomes 0?
				if (dt <= 0 || temp < temp2) {
					t = dt + t;
					flag = 1;
					latest_add = j;
					vertice *v = (vertice*)malloc(sizeof(vertice));
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
			compare(state, n);
			vertice *prev = nullptr;
			for (vertice *v = state;v != nullptr;v = v->next) {
				if (v->i == latest_add) {
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
				float temp2 = exp(-(float)dt / t) / 2;//should exp(-MAX/t) becomes 0?
				if (dt <= 0 || temp < temp2) {
					latest_del = v->i;
					t = dt + t;
					flag = 1;
					if (prev == nullptr)state=state->next;
					free(v);
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