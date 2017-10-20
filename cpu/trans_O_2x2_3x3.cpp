#include "stdafx.h"
#include "trans_O_2x2_3x3.h"
#include "winograd_debug.h"

void trans_O_2x2_3x3(FLOAT Mw[2][2], FLOAT M[4][4])
{
	// Minimal version only
	FLOAT T0[2][4];
	for (int ii = 0; ii < 4; ++ii) {
		T0[0][ii] = M[0][ii] + M[1][ii] + M[2][ii];
		T0[1][ii] = M[1][ii] - M[2][ii] - M[3][ii];
	}

	for (int ii = 0; ii < 2; ++ii) {
		Mw[ii][0] = T0[ii][0] + T0[ii][1] + T0[ii][2];
		Mw[ii][1] = T0[ii][1] - T0[ii][2] - T0[ii][3];
	}
}
