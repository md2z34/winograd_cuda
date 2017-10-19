#include "stdafx.h"
#include "winograd_debug.h"
#include "xprop_winograd.h"
#include "ceil_div.h"
#include "trans_F_2x2_3x3.h"
#include "trans_I_2x2_3x3.h"
#include "trans_O_2x2_3x3.h"
#include "image_slice.h"
#ifdef DEBUG
#include <fstream>
#include <iostream>
#endif // DEBUG

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
	float tmp2x2[2][2];

	float sliceI[C][Y][X][N];
	memset(O, 0, sizeof(float) * 32 * 4 * 4 * 32);
	// Transform Filters
#ifdef DEBUG
	std::ofstream Fw_file;
	std::ofstream Iw_file;
	std::ofstream Mw_file;
	std::ofstream slice_file;
#endif // DEBUG
	for (int c = 0; c < C; ++c) {
		for (int k = 0; k < K; ++k) {
			// F[c, :, : , k] -> tmp3x3
			for (int ii = 0; ii < 3; ++ii) {
				for (int jj = 0; jj < 3; ++jj) {
					tmp3x3[ii][jj] = F[c][ii][jj][k];
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
#ifdef DEBUG
	Fw_file.open("Fw_cpu.txt");
	for (int a = 0; a < 4; ++a) {
		for (int b = 0; b < 4; ++b) {
			for (int c = 0; c < 32; ++c) {
				for (int d = 0; d < 32; ++d) {
					Fw_file << Fw[a][b][c][d] << ";";
				}
			}
		}
	}
	Fw_file.close();
#endif // DEBUG
	// Iterate over image transform dimensions and slice out tiles of the image
	float in[4][4];
	float out[4][4];
	for (int y = 0; y < Yw; ++y) {
		int start_y, stop_y, pad_y[2];
		image_slice(y, Y, B, D, padding[0], &start_y, &stop_y, pad_y);
		for (int x = 0; x < Xw; ++x) {
			int start_x, stop_x, pad_x[2];
			image_slice(x, X, B, D, padding[1], &start_x, &stop_x, pad_x);
			// sliceI = I[:, start_y:stop_y, start_x:stop_x, :]
			memset(sliceI, 0, sizeof(float)*C*Y*X*N);
			for (int c = 0; c < C; ++c) {
				for (int n = 0; n < N; ++n) {
					for (int yy = start_y; yy < MIN(stop_y, Y); ++yy) {
						for (int xx = start_x; xx < MIN(stop_x, X); ++xx) {
							if ((pad_x[0] > 0) && (pad_y[0] > 0)) {
								sliceI[c][yy + pad_y[0]][xx + pad_x[0]][n] = I[c][yy][xx][n];
							}
							else if ((pad_x[1] > 0) && (pad_y[0] > 0)) {
								sliceI[c][yy + pad_y[0]][xx - pad_x[1]][n] = I[c][yy][xx][n];
							}
							else if ((pad_x[0] > 0) && (pad_y[1] > 0)) {
								sliceI[c][yy - pad_y[1]][xx + pad_x[0]][n] = I[c][yy][xx][n];
							}
							else if ((pad_x[1] > 0) && (pad_y[1] > 0)) {
								sliceI[c][yy - pad_y[1]][xx - pad_x[1]][n] = I[c][yy][xx][n];
							}
							else {
								sliceI[c][yy][xx][n] = I[c][yy][xx][n];
							}
						}
					}
				}
			}
#ifdef DEBUG
			slice_file.open("sliceI_cpu.txt");
			for (int a = 0; a < C; ++a) {
				for (int b = 0; b < Y; ++b) {
					for (int c = 0; c < X; ++c) {
						for (int d = 0; d < N; ++d) {
							slice_file << sliceI[a][b][c][d] << ";";
						}
					}
				}
			}
			slice_file.close();

			slice_file.open("sliceI_mat.txt");
#endif // DEBUG
			// Apply the Image transform
			for (int c = 0; c < C; ++c) {
				for (int n = 0; n < N; ++n) {
					// in = sliceI[c,:,:,n]
#ifdef DEBUG
					slice_file << "sliceI (c="<<c<<";n="<<n<<"):"<<std::endl;
#endif // DEBUG
					for (int yy = 0; yy < Y; ++yy) {
						for (int xx = 0; xx < X; ++xx) {
							in[yy][xx] = sliceI[c][yy][xx][n];
#ifdef DEBUG
							slice_file << in[yy][xx] << "; ";
#endif // DEBUG
						}
#ifdef DEBUG
						slice_file << std::endl;
#endif // DEBUG
					}
#ifdef DEBUG
					slice_file << std::endl;
#endif // DEBUG
					trans_I_2x2_3x3(out, in);
					// Iw[:,:,c,y,x,n] = out
					// std::cout << "Iw:" << std::endl;
					for (int dx = 0; dx < D; ++dx) {
						for (int dy = 0; dy < D; ++dy) {
							Iw[dx][dy][c][y][x][n] = out[dx][dy];
#ifdef DEBUG
							//	std::cout << out[dx][dy] << ";";
#endif // DEBUG
						}
#ifdef DEBUG
						//	std::cout << std::endl;
#endif // DEBUG
					}
#ifdef DEBUG
					//	std::cout << std::endl;
#endif // DEBUG
				} // for n
			} // for c
#ifdef DEBUG
			slice_file.close();
#endif // DEBUG
		} // for x
	} // for y
#ifdef DEBUG
	Iw_file.open("Iw_cpu.txt");
	for (int a = 0; a < D; ++a) {
		for (int b = 0; b < D; ++b) {
			for (int c = 0; c < C; ++c) {
				for (int d = 0; d < Yw; ++d) {
					for (int e = 0; e < Xw; ++e) {
						for (int f = 0; f < N; ++f) {
							Iw_file << Iw[a][b][c][d][e][f] << ";";
						}
					}
				}
			}
		}
	}
	Iw_file.close();
#endif // DEBUG

	// Batched gemm for the pointwise multiplication step
	
	float mat1T[K][C];
	float mat2[C][Yw*Xw*N];
	float matout[C][Yw*Xw*N];
	for (int s = 0; s < D; ++s) {
		for (int t = 0; t < D; ++t) {
			// Fill in mat1T = Fw[D][D][C][K]
			for (int c = 0; c < C; ++c) {
				for (int k = 0; k < C; ++k) {
					mat1T[k][c] = Fw[s][t][c][k];
				}
			}
			// Fill in mat2 = Iw[D][D][C][Yw][Xw][N]
			for (int c = 0; c < C; ++c) {
				for (int yw = 0; yw < Yw; ++yw) {
					for (int xw = 0; xw < Xw; ++xw) {
						for (int n = 0; n < N; ++n) {
							mat2[c][yw*Xw*N + xw*N + n] = Iw[s][t][c][yw][xw][n];
						}
					}
				}
			}
			// Matrix multiplication
			memset(matout, 0, sizeof(float)*C*Yw*Xw*N);
			for (int k = 0; k < K; ++k) {
				for (int yxn = 0; yxn < Yw*Xw*N; ++yxn) {
					for (int c = 0; c < C; ++c) {
						matout[k][yxn] += mat1T[k][c] * mat2[c][yxn];
					}
				}
			}
			// Wrinting back te result to Mw[D][D][K][Yw][Xw][N]
			for (int k = 0; k < K; ++k) {
				for (int yw = 0; yw < Yw; ++yw) {
					for (int xw = 0; xw < Xw; ++xw) {
						for (int n = 0; n < N; ++n) {
							Mw[s][t][k][yw][xw][n] = matout[k][yw*Xw*N + xw*N + n];
						}
					}
				}
			}

		} // for t
	} //for s
#ifdef DEBUG
	//Mw[D][D][K][Yw][Xw][N];
	Mw_file.open("Mw_cpu.txt");
	for (int a = 0; a < D; ++a) {
		for (int b = 0; b < D; ++b) {
			for (int c = 0; c < K; ++c) {
				for (int d = 0; d < Yw; ++d) {
					for (int e = 0; e < Xw; ++e) {
						for (int f = 0; f < N; ++f) {
							Mw_file << Mw[a][b][c][d][e][f] << ";";
						}
					}
				}
			}
		}
	}
	Mw_file.close();
#endif // DEBUG

	// Iterate over the convovled result in the pointwise space and apply inverse transform
	for (int y = 0; y < Yw; ++y) {
		int p = y*B;
		int plen = ((p + 1) < P) ? 2 : 1;
		for (int x = 0; x < Xw; ++x) {
			int q = x*B;
			int qlen = ((q + 1) < Q) ? 2 : 1;
			for (int k = 0; k < K; ++k) {
				for (int n = 0; n < N; ++n) {
					// Mw[:, : , k, y, x, n] -> tmp4x4
					for (int ii = 0; ii < D; ++ii) {
						for (int jj = 0; jj < D; ++jj) {
							tmp4x4[ii][jj] = Mw[ii][jj][k][y][x][n];
						}
					}
					trans_O_2x2_3x3(tmp2x2,tmp4x4);
					// tmp2x2 -> O[k,p:p + plen,q:q + qlen,n]
					for (int ii = 0; ii < plen; ++ii) {
						for (int jj = 0; jj < qlen; ++jj) {
							if (((p + ii) >= 0) && ((p + ii) < 4) && ((q + jj) >= 0) && ((q + jj) < 4)) {
								O[k][p + ii][q + jj][n] = tmp2x2[ii][jj];
							}
						}
					}
				}
			}
		}
	}
}