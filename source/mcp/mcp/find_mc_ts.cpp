#include "mcp.h"
#define TABU_LEN 100000

float c_ts(vertice *s, int n) {
	int num = 0;
	for (vertice *v = s;v != nullptr;v = v->next) {
		num++;
	}
	return (float)rand() / RAND_MAX + n - num;
}

int compare(vertice *s, int n) {
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
	return num;
}

void find_mc_ts(int n) {
	vertice *state, *free;
	vertice temp_v;

	vertice statelist[MAX_MC];

	int tabulist[TABU_LEN] = { 0 };
	int tabu_i = 0;
	free = &statelist[1];
	for (int i = 1;i < MAX_MC;i++) {
		statelist[i].next = i == MAX_MC - 1 ? nullptr : &statelist[i + 1];
	}

	state = &statelist[0];
	state->i = 0;
	state->next = nullptr;
	srand(time(0));

	while (true) {
		compare(state, n);

		float best = 0;
		bool best_add;
		int best_id;

		int sum = 0;
		for (vertice *t = state;t != nullptr;t = t->next) {
			sum += t->i;
		}
		tabulist[tabu_i%TABU_LEN] = sum;
		tabu_i++;

		for (int j = 0;j < n;j++) {
			int can_be_added = 1;
			for (vertice *v = state;v != nullptr;v = v->next) {
				if (!adj_matrix[v->i][j]) {
					can_be_added = 0;
					break;
				}
			}
			if (can_be_added) {
				temp_v.i = j;
				temp_v.next = state;
				bool add = false;
				int num = 0;
				for (vertice *v = &temp_v;v != nullptr;v = v->next) {
					num++;
				}
				if (num > mc_count) {
					add = true;
				}
				if (!add) {
					add = true;
					for (int i = 0;i < TABU_LEN;i++) {
						if (sum + j == tabulist[i]) {
							add = false;
						}
					}
				}
				if (add) {
					float tc = c_ts(&temp_v, n);
					if (tc > best) {
						best = tc;
						best_add = true;
						best_id = j;
					}
				}
			}
		}

		if (best == 0) {
			//remove
			vertice *prev = nullptr;
			for (vertice *v = state;v != nullptr;v = v->next) {
				bool del = true;
				for (int i = 0;i < TABU_LEN;i++) {
					if (sum - v->i == tabulist[i]) {
						del = false;
					}
				}
				if (!del) {
					prev = v;
					continue;
				}

				float tc = 0;
				if (prev == nullptr) {
					tc = c_ts(state->next, n);
				} else {
					prev->next = v->next;
					tc = c_ts(state, n);
				}
				if (tc > best) {
					best = tc;
					best_add = false;
					best_id = v->i;
				}

				if (prev != nullptr)prev->next = v;
				prev = v;
			}
		}

		if (best > 0) {
			if (best_add) {
				vertice *v = free;
				free = free->next;
				v->i = best_id;
				v->next = state;
				state = v;
			} else {
				vertice *prev = nullptr;
				for (vertice *v = state;v != nullptr;v = v->next) {
					if (v->i == best_id) {
						if (prev == nullptr) {
							state = state->next;

						} else {
							prev->next = v->next;
						}
						v->next = free;
						free = v;
						break;
					}
					prev = v;
				}
			}
		} else {
			compare(state, n);
			//return;
		}
	}
}