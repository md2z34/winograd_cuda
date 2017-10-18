#include "stdafx.h"
#include "image_slice.h"

void image_slice(int x, int X, int B, int D, int pad, int *start, int *stop, int pad_out[2]) {
	(*start) = x * B - pad;
	(*stop) = (*start) + D;
	pad_out[0] = 0; pad_out[1] = 0;
	if ((*start) < 0) {
		pad_out[0] = -(*start);
		(*start) = 0;
	}
	if ((*stop) - 1 >= X) {
		pad_out[1] = (*stop) - X;
	}
}