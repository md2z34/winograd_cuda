#pragma once
#include "winograd_debug.h"

void xprop_winograd(FLOAT I[32][4][4][32], FLOAT F[32][3][3][32], FLOAT O[32][4][4][32], int padding[2]);