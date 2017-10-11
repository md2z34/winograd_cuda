// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "trans_F_2x2_3x3.h"
#include "trans_I_2x2_3x3.h"
#include "trans_O_2x2_3x3.h"
#include "ceil_div.h"

using namespace std;

inline int mat2vec_idx(int row, int col, int row_n)
{
	return (row*row_n + col);
}

inline void vec2mat_idx(int N, int row_n, int *row, int *col)
{
	*row = N / row_n;
	*col = N % row_n;
}

int main()
{
	int a;
	a = -4 / 3;
	a = (-4)%3;
	a = 4 % (-3);
	a = (-4) % (-3);
	int b = ceil_div(-4, 3);
	return 0;
}

