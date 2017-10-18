// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "trans_F_2x2_3x3.h"
#include "trans_I_2x2_3x3.h"
#include "trans_O_2x2_3x3.h"
#include "ceil_div.h"
#include "xprop_winograd.h"

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
	float I[32][4][4][32]; 
	float F[32][3][3][32];
	float Ow[32][4][4][32];
	int padding[2];
	ofstream Ow_file;
	ofstream F_file;
	// Init input values
	padding[0] = 1; padding[1] = 1;
	for (int a = 0; a < 32; ++a) {
		for (int b = 0; b < 4; ++b) {
			for (int c = 0; c < 4; ++c) {
				for (int d = 0; d < 32; ++d) {
					I[a][b][c][d] = 1.0f;
				}
			}
		}
	}
	for (int a = 0; a < 32; ++a) {
		for (int b = 0; b < 3; ++b) {
			for (int c = 0; c < 3; ++c) {
				for (int d = 0; d < 32; ++d) {
					F[a][b][c][d] = 1.0f;
				}
			}
		}
	}
	F_file.open("F_cpu.txt");
	for (int a = 0; a < 32; ++a) {
		for (int b = 0; b < 3; ++b) {
			for (int c = 0; c < 3; ++c) {
				for (int d = 0; d < 32; ++d) {
					F_file << F[a][b][c][d] << ";";
				}
			}
		}
	}
	F_file.close();
	xprop_winograd(I, F, Ow, padding);

	Ow_file.open("Ow_cpu.txt");
	for (int a = 0; a < 32; ++a) {
		for (int b = 0; b < 4; ++b) {
			for (int c = 0; c < 4; ++c) {
				for (int d = 0; d < 32; ++d) {
					Ow_file <<Ow[a][b][c][d]<<";";
				}
			}
		}
	}
	Ow_file.close();
	return 0;
}

