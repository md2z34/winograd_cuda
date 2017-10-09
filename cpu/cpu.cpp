// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
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

void trans_F_2x2_3x3(float Fw[4][4], float F[3][3])
{
	// Minimal version only
	float T0[4][3];
	for (int ii = 0; ii < 3; ++ii) {
		T0[0][ii] = F[0][ii];
		T0[1][ii] = (F[0][ii] + F[2][ii] + F[1][ii]) * 0.5f;
		T0[2][ii] = (F[0][ii] + F[2][ii] - F[1][ii]) * 0.5f;
		T0[3][ii] = F[2][ii];
	}

	for (int ii = 0; ii < 4; ++ii) {
		Fw[ii][0] = T0[ii][0];
		Fw[ii][1] = (T0[ii][0] + T0[ii][2] + T0[ii][1]) * 0.5f;
		Fw[ii][2] = (T0[ii][0] + T0[ii][2] - T0[ii][1]) * 0.5f;
		Fw[ii][3] = T0[ii][2];
	}
}

int main()
{
	int r = 3;
	int c = 3;
	vector<float> vec;
	vec.clear();
	vec.reserve(r*c);
	int k = 0;
	float in[3][3];
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; ++j) {
			in[i][j] = (k++);
			//out =
			//	0 1.5 0.5 2
			//	4.5 9 3 7.5
			//	1.5 3 1 2.5
			//	6 10.5 3.5 8

			//in[i][j] = 1.;
			//out =
			//	1 1.5 0.5 1
			//	1.5 2.25 0.75 1.5
			//	0.5 0.75 0.25 0.5
			//	1 1.5 0.5 1
		}
	}
	float out[4][4];
	trans_F_2x2_3x3(out, in);
	cout << "out=" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			cout  << out[i][j] <<" ";
		}
		cout << endl;
	}
	return 0;
}

