#pragma once
#include "winograd_debug.h"

void xprop_winograd(FLOAT *I, int lenI, FLOAT *F, int lenF, FLOAT *O, int lenO, int padding[2]);