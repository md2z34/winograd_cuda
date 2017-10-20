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

void xprop_winograd(FLOAT *I, int lenI, FLOAT *F, int lenF, FLOAT *O, int lenO, int padding[2]) {
	// I shape
	int C=32, Y=4, X=4, N=32;
	// O shape
	int K=32, P=4, Q=4;

	int B = 2;
	int D = B + 2;

	int Yw = ceil_div(P, B);
	int Xw = ceil_div(Q, B);

	FLOAT *Fw; // [D][D][C][K];
	int lenFw = D*D*C*K;
	FLOAT *Iw; // [D][D][C][Yw][Xw][N];
	int lenIw = D*D*C*Yw*Xw*N;
	FLOAT *Mw; // [D][D][K][Yw][Xw][N];
	int lenMw = D*D*K*Yw*Xw*N;

	FLOAT tmp4x4[4][4];
	FLOAT tmp3x3[3][3];
	FLOAT tmp2x2[2][2];

	FLOAT *sliceI; //[C][Y][X][N];
	int lenSliceI = C*Y*X*N;
	memset(O, 0, sizeof(FLOAT) * lenO);
	// FLOAT I[32][4][4][32]; 
	// FLOAT F[32][3][3][32];
	// Transform Filters
#ifdef DEBUG
	std::ofstream Fw_file;
	std::ofstream Iw_file;
	std::ofstream Mw_file;
	std::ofstream slice_file;
#endif // DEBUG
	Fw = (FLOAT *)malloc(sizeof(FLOAT)*lenFw);
	if (NULL == Fw) {
		exit(-1);
	}
	Iw = (FLOAT *)malloc(sizeof(FLOAT)*lenIw);
	if (NULL == Iw) {
		exit(-1);
	}
	Mw = (FLOAT *)malloc(sizeof(FLOAT)*lenMw);
	if (NULL == Mw) {
		exit(-1);
	}
	sliceI = (FLOAT *)malloc(sizeof(FLOAT)*lenSliceI);
	if (NULL == sliceI) {
		exit(-1);
	}

	for (int c = 0; c < C; ++c) {
		for (int k = 0; k < K; ++k) {
			// F[c, :, : , k] -> tmp3x3
			for (int ii = 0; ii < 3; ++ii) {
				for (int jj = 0; jj < 3; ++jj) {
					tmp3x3[ii][jj] = F[c*3*3*32 + ii*3*32 + jj*32 + k];
				}
			}
			trans_F_2x2_3x3(tmp4x4, tmp3x3);
			// tmp4x4 -> Fw[:,:,c,k]
			for (int ii = 0; ii < 4; ++ii) {
				for (int jj = 0; jj < 4; ++jj) {
					Fw[ii*D*C*K + jj*C*K + c*K + k] = tmp4x4[ii][jj];
				}
			}
		}
	}
#ifdef DEBUG
	Fw_file.open("Fw_cpu.txt");
	for (int a = 0; a < lenFw; ++a) {
		Fw_file << Fw[a] << ";";
	}
	Fw_file.close();
#endif // DEBUG
	// Iterate over image transform dimensions and slice out tiles of the image
	FLOAT in[4][4];
	FLOAT out[4][4];
	for (int y = 0; y < Yw; ++y) {
		int start_y, stop_y, pad_y[2];
		image_slice(y, Y, B, D, padding[0], &start_y, &stop_y, pad_y);
		for (int x = 0; x < Xw; ++x) {
			int start_x, stop_x, pad_x[2];
			image_slice(x, X, B, D, padding[1], &start_x, &stop_x, pad_x);
			// sliceI = I[:, start_y:stop_y, start_x:stop_x, :]
			memset(sliceI, 0, sizeof(FLOAT)*C*Y*X*N);
			for (int c = 0; c < C; ++c) {
				for (int n = 0; n < N; ++n) {
					for (int yy = start_y; yy < MIN(stop_y, Y); ++yy) {
						for (int xx = start_x; xx < MIN(stop_x, X); ++xx) {
							if ((pad_x[0] > 0) && (pad_y[0] > 0)) {
								sliceI[c*Y*X*N + (yy + pad_y[0])*X*N + (xx + pad_x[0])*N + n] = I[c * 4 * 4 * 32 + yy * 4 * 32 + xx * 32 + n];
							}
							else if ((pad_x[1] > 0) && (pad_y[0] > 0)) {
								sliceI[c*Y*X*N + (yy + pad_y[0])*X*N + (xx - pad_x[1])*N + n] = I[c * 4 * 4 * 32 + yy * 4 * 32 + xx * 32 + n];
							}
							else if ((pad_x[0] > 0) && (pad_y[1] > 0)) {
								sliceI[c*Y*X*N + (yy - pad_y[1])*X*N + (xx + pad_x[0])*N + n] = I[c * 4 * 4 * 32 + yy * 4 * 32 + xx * 32 + n];
							}
							else if ((pad_x[1] > 0) && (pad_y[1] > 0)) {
								sliceI[c*Y*X*N + (yy - pad_y[1])*X*N + (xx - pad_x[1])*N + n] = I[c * 4 * 4 * 32 + yy * 4 * 32 + xx * 32 + n];
							}
							else {
								sliceI[c*Y*X*N + (yy)*X*N + (xx)*N + n] = I[c * 4 * 4 * 32 + yy * 4 * 32 + xx * 32 + n];
							}
						}
					}
				}
			}
#ifdef DEBUG
			slice_file.open("sliceI_cpu.txt");
			for (int a = 0; a < lenSliceI; ++a) {
				slice_file << sliceI[a] << ";";
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
							in[yy][xx] = sliceI[c*Y*X*N + (yy)*X*N + (xx)*N + n];
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
							Iw[dx*D*C*Yw*Xw*N + dy*C*Yw*Xw*N + c*Yw*Xw*N + y*Xw*N + x*N + n] = out[dx][dy];
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
	for (int a = 0; a < lenIw; ++a) {
		Iw_file << Iw[a] << ";";
	}
	Iw_file.close();
#endif // DEBUG

	// Batched gemm for the pointwise multiplication step
	
	FLOAT *mat1T; // [K][C];
	int lenMat1T = K*C;
	FLOAT *mat2; //[C][Yw*Xw*N];
	int lenMat2 = C*Yw*Xw*N;
	FLOAT *matout; //[C][Yw*Xw*N];
	int lenMatout = C*Yw*Xw*N;

	mat1T = (FLOAT *)malloc(sizeof(FLOAT)*lenMat1T);
	if (NULL == mat1T) {
		exit(-1);
	}
	mat2 = (FLOAT *)malloc(sizeof(FLOAT)*lenMat2);
	if (NULL == mat2) {
		exit(-1);
	}
	matout = (FLOAT *)malloc(sizeof(FLOAT)*lenMatout);
	if (NULL == matout) {
		exit(-1);
	}

	for (int s = 0; s < D; ++s) {
		for (int t = 0; t < D; ++t) {
			// Fill in mat1T = Fw[D][D][C][K]
			for (int c = 0; c < C; ++c) {
				for (int k = 0; k < C; ++k) {
					mat1T[k*C + c] = Fw[s*D*C*K + t*C*K + c*K + k];
				}
			}
			// Fill in mat2 = Iw[D][D][C][Yw][Xw][N]
			for (int c = 0; c < C; ++c) {
				for (int yw = 0; yw < Yw; ++yw) {
					for (int xw = 0; xw < Xw; ++xw) {
						for (int n = 0; n < N; ++n) {
							mat2[c*Yw*Xw*N + yw*Xw*N + xw*N + n] = Iw[s*D*C*Yw*Xw*N + t*C*Yw*Xw*N + c*Yw*Xw*N + yw*Xw*N + xw*N + n];
						}
					}
				}
			}
			// Matrix multiplication
			memset(matout, 0, sizeof(FLOAT)*C*Yw*Xw*N);
			for (int k = 0; k < K; ++k) {
				for (int yxn = 0; yxn < Yw*Xw*N; ++yxn) {
					for (int c = 0; c < C; ++c) {
						matout[k*Yw*Xw*N + yxn] += mat1T[k*C + c] * mat2[c*Yw*Xw*N + yxn];
					}
				}
			}
			// Wrinting back te result to Mw[D][D][K][Yw][Xw][N]
			for (int k = 0; k < K; ++k) {
				for (int yw = 0; yw < Yw; ++yw) {
					for (int xw = 0; xw < Xw; ++xw) {
						for (int n = 0; n < N; ++n) {
							Mw[s*D*K*Yw*Xw*N + t*K*Yw*Xw*N + k*Yw*Xw*N + yw*Xw*N + xw*N + n] = matout[k*Yw*Xw*N + yw*Xw*N + xw*N + n];
						}
					}
				}
			}

		} // for t
	} //for s
#ifdef DEBUG
	//Mw[D][D][K][Yw][Xw][N];
	Mw_file.open("Mw_cpu.txt");
	for (int a = 0; a < lenMw; ++a) {
		Mw_file << Mw[a] << ";";
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
							tmp4x4[ii][jj] = Mw[ii*D*K*Yw*Xw*N + jj*K*Yw*Xw*N + k*Yw*Xw*N + y*Xw*N + x*N + n];
						}
					}
					trans_O_2x2_3x3(tmp2x2,tmp4x4);
					// tmp2x2 -> O[k,p:p + plen,q:q + qlen,n]
					for (int ii = 0; ii < plen; ++ii) {
						for (int jj = 0; jj < qlen; ++jj) {
							if (((p + ii) >= 0) && ((p + ii) < 4) && ((q + jj) >= 0) && ((q + jj) < 4)) {
								O[k*4*4*32 + (p + ii)*4*32 + (q + jj)*32 + n] = tmp2x2[ii][jj];
							}
						}
					}
				}
			}
		}
	}
	free(mat1T);
	free(mat2);
	free(matout);
	free(Fw);
	free(Iw);
	free(Mw);
	free(sliceI);
}