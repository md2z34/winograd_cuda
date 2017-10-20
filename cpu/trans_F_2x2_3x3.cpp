#include "stdafx.h"
#include "trans_F_2x2_3x3.h"
#include "winograd_debug.h"

void trans_F_2x2_3x3(FLOAT Fw[4][4], FLOAT F[3][3])
{
	// Minimal version only
	FLOAT T0[4][3];
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
