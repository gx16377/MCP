#ifndef Mo_Bai_God_of_Guo
#define Mo_Bai_God_of_Guo

#define N_v 4001

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

extern bool adj_matrix[N_v][N_v];
extern bool mc[N_v];
extern int mc_count;

int find_mc_low(int);
void find_mc_sa(int);

#endif