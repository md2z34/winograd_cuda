#include "stdafx.h"
#include "xprop_winograd.h"
#include "ceil_div.h"
#include "trans_F_2x2_3x3.h"
#include "image_slice.h"

void xprop_winograd(float I[32][4][4][32], float F[32][3][3][32], float O[32][4][4][32], int padding[2]) {
	// I shape
	const int C=32, Y=4, X=4, N=32;
	// O shape
	const int K=32, P=4, Q=4;

	const int B = 2;
	const int D = B + 2;

	const int Yw = 2; // ceil_div(P, B);
	const int Xw = 2; // ceil_div(Q, B);

	float Fw[D][D][C][K];
	float Iw[D][D][C][Yw][Xw][N];
	float Mw[D][D][K][Yw][Xw][N];

	float tmp4x4[4][4];
	float tmp3x3[3][3];
	// Transform Filters
	for (int c = 0; c < C; ++c) {
		for (int k = 0; k < K; ++k) {
			// F[c, :, : , k] -> tmp3x3
			for (int ii = 0; ii < 3; ++ii) {
				for (int jj = 0; jj < 3; ++jj) {
					tmp3x3[ii][jj] = F[32][ii][jj][32];
				}
			}
			trans_F_2x2_3x3(tmp4x4, tmp3x3);
			// tmp4x4 -> Fw[:,:,c,k]
			for (int ii = 0; ii < 4; ++ii) {
				for (int jj = 0; jj < 4; ++jj) {
					Fw[ii][jj][c][k] = tmp4x4[ii][jj];
				}
			}
		}
	}
	// Iterate over image transform dimensions and slice out tiles of the image
	for (int y = 0; y < Yw; ++y) {
		int start_y, stop_y, pad_y[2];
		image_slice(y, Y, B, D, padding[0], &start_y, &stop_y, pad_y);
		for (int x = 0; x < Xw; ++x) {
			int start_x, stop_x, pad_x[2];
			image_slice(x, X, B, D, padding[1], &start_y, &stop_y, pad_x);

			

		}

}