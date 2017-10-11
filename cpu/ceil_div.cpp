#include "stdafx.h"
#include "ceil_div.h"

int ceil_div(int x, int y) {
	// For positive numbers only
	return x / y + (x%y != 0);
}