// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "trans_F_2x2_3x3.h"
#include "trans_I_2x2_3x3.h"
#include "trans_O_2x2_3x3.h"

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
	float out_012[2][2];
	out_012[0][0] = 45; out_012[0][1] = -24;
	out_012[1][0] = -51; out_012[1][1] = 20;
	float out_111[2][2];
	out_111[0][0] = 9; out_111[0][1] = -3;
	out_111[1][0] = -3; out_111[1][1] = 1;

	float in1[4][4]; int k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in1[i][j] = (float)(k++);
		}
	}
	float in2[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in2[i][j] = 1.;
		}
	}
	float out1[2][2];
	trans_O_2x2_3x3(out1, in1);
	float out2[2][2];
	trans_O_2x2_3x3(out2, in2);

	float norm1 = 0.;
	float norm2 = 0.;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; ++j) {
			norm1 += (out1[i][j] - out_012[i][j])*(out1[i][j] - out_012[i][j]);
			norm2 += (out2[i][j] - out_111[i][j])*(out2[i][j] - out_111[i][j]);
		}
	}
	return 0;
}

