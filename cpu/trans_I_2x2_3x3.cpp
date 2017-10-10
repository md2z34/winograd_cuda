#include "stdafx.h"
#include "trans_I_2x2_3x3.h"

void trans_I_2x2_3x3(float Iw[4][4], float I[4][4])
{
	// Minimal version only
	float T0[4][4];
	for (int ii = 0; ii < 4; ++ii) {
		T0[0][ii] = I[0][ii] - I[2][ii];
		T0[1][ii] = I[1][ii] + I[2][ii];
		T0[2][ii] = I[2][ii] - I[1][ii];
		T0[3][ii] = I[1][ii] - I[3][ii];
	}

	for (int ii = 0; ii < 4; ++ii) {
		Iw[ii][0] = T0[ii][0] - T0[ii][2];
		Iw[ii][1] = T0[ii][1] + T0[ii][2];
		Iw[ii][2] = T0[ii][2] - T0[ii][1];
		Iw[ii][3] = T0[ii][1] - T0[ii][3];
	}
}
