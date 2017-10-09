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

void trans_F_2x2_3x3(float **Fw, // Output 4x4
					 float **F) // Input 3x3
{
	// Minimal version only
	float T0[4][3];
	float T1[4][4];


}

int main()
{
	int r = 3;
	int c = 4;
	vector<int> vec;
	vec.clear();
	vec.reserve(r*c);
	int k = 0;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; ++j) {
			vec.push_back(k++);
		}
	}

	cout << "Hello" << endl;
	int rk = 2;
	int ck = 3;
	int i = mat2vec_idx(rk,ck,c);
	i = 4;
	vec2mat_idx(i, c, &rk, &ck);
	cout << "vec[" << rk << "," << ck << "]=" << vec.at(i) << endl;
	return 0;
}

