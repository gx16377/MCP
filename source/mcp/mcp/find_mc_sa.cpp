#include "mcp.h"

int c(bool s[], int n) {
	//now a c(S) to select all vertices into S
	int num = 0;
	for (int i = 0;i < n;i++) {
		if (s[i]) num++;
	}
	return n - num;
}

void find_mc_sa(int n) {
	int t = n;//gap between initial value of t and range of c(S) effects efficiency a lot
	bool s[N_v];
	for (int i = 0;i < n;i++) {
		s[i] = false;
	}
	int dt;
	while (true) {
		int flag = 0;
		for (int i = 0;i < n;i++) {
			s[i] = !s[i];
			dt = c(s, n) - t;
			float temp = (float)rand() / RAND_MAX;
			float temp2 = exp(-(float)dt / t) / 2;// divide by 2 to reduce false rate temporarily
			if (dt <= 0 || temp < temp2) {
				t = dt + t;
				flag = 1;
				break;
			}
			s[i] = !s[i];
		}
		if (!flag) {
			for (int i = 0;i < n;i++) {
				mc[i] = s[i];
				mc_count++;
			}
			return;
		}
	}
}