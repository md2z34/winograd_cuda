// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winograd_debug.h"
#include <cstdlib>
#include <iostream>
#ifdef DEBUG
#include <fstream>
#endif // DEBUG
#include <vector>
#include "trans_F_2x2_3x3.h"
#include "trans_I_2x2_3x3.h"
#include "trans_O_2x2_3x3.h"
#include "ceil_div.h"
#include "xprop_winograd.h"

using namespace std;

int main()
{
	FLOAT *I; //[32][4][4][32]; 
	int lenI = 32 * 4 * 4 * 32;
	FLOAT *F; //[32][3][3][32];
	int lenF = 32 * 3 * 3 * 32;
	FLOAT *Ow; // [32][4][4][32];
	int lenOw = 32 * 4 * 4 * 32;
	int padding[2];
#ifdef DEBUG
	ofstream Ow_file;
	ofstream F_file;
#endif // DEBUG
	// Init input values
	I = (FLOAT *)malloc(sizeof(FLOAT)*lenI);
	if (NULL == I) {
		return (-1);
	}
	F = (FLOAT *)malloc(sizeof(FLOAT)*lenF);
	if (NULL == F) {
		return (-1);
	}
	Ow = (FLOAT *)malloc(sizeof(FLOAT)*lenOw);
	if (NULL == Ow) {
		return (-1);
	}

	padding[0] = 1; padding[1] = 1;
	int k = 0;
	for (int a = 0; a < lenI; ++a) {
		//I[a] = (FLOAT)k++;
		I[a] = 1.0;
	}
	for (int a = 0; a < lenF; ++a) {
		//F[a] = (FLOAT)k++;
		F[a] = 1.0;
	}
#ifdef DEBUG
	F_file.open("F_cpu.txt");
	for (int a = 0; a < lenF; ++a) {
		F_file << F[a] << ";";
	}
	F_file.close();
#endif // DEBUG
	xprop_winograd(I, lenI, F, lenF, Ow, lenOw, padding);

#ifdef DEBUG
	Ow_file.open("Ow_cpu.txt");
	for (int a = 0; a < lenOw; ++a) {
		Ow_file << std::fixed << Ow[a]<<";";
	}
	Ow_file.close();
#endif // DEBUG
	free(I);
	free(F);
	free(Ow);
	return 0;
}

