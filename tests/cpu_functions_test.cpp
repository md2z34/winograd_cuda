#include "stdafx.h"
#include "CppUTest/TestHarness.h"
#include "../cpu/trans_F_2x2_3x3.h"
#include "../cpu/trans_I_2x2_3x3.h"
#include "../cpu/trans_O_2x2_3x3.h"
#include "../cpu/image_slice.h"
#include "../cpu/ceil_div.h"
#include <stdio.h>

TEST_GROUP(CPU_code)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(CPU_code, trans_F_2x2_3x3_test)
{
	printf("trans_F_2x2_3x3_test...");
	// Outputs for reference
	float out_012[4][4];
	out_012[0][0] = 0; out_012[0][1] = 1.5; out_012[0][2] = 0.5; out_012[0][3] = 2;
	out_012[1][0] = 4.5; out_012[1][1] = 9; out_012[1][2] = 3; out_012[1][3] = 7.5;
	out_012[2][0] = 1.5; out_012[2][1] = 3; out_012[2][2] = 1; out_012[2][3] = 2.5;
	out_012[3][0] = 6; out_012[3][1] = 10.5; out_012[3][2] = 3.5; out_012[3][3] = 8;
	float out_111[4][4];
	out_111[0][0] = 1; out_111[0][1] = 1.5; out_111[0][2] = 0.5; out_111[0][3] = 1;
	out_111[1][0] = 1.5; out_111[1][1] = 2.25; out_111[1][2] = 0.75; out_111[1][3] = 1.5;
	out_111[2][0] = 0.5; out_111[2][1] = 0.75; out_111[2][2] = 0.25; out_111[2][3] = 0.5;
	out_111[3][0] = 1; out_111[3][1] = 1.5; out_111[3][2] = 0.5; out_111[3][3] = 1;

	float in1[3][3]; int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; ++j) {
			in1[i][j] = (float)(k++);
		}
	}
	float in2[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; ++j) {
			in2[i][j] = 1.;
		}
	}
	float out1[4][4];
	trans_F_2x2_3x3(out1, in1);
	float out2[4][4];
	trans_F_2x2_3x3(out2, in2);

	float norm1 = 0.;
	float norm2 = 0.;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; ++j) {
			norm1 += (out1[i][j] - out_012[i][j])*(out1[i][j] - out_012[i][j]);
			norm2 += (out2[i][j] - out_111[i][j])*(out2[i][j] - out_111[i][j]);
		}
	}

	CHECK(norm1 < 1e-10);
	CHECK(norm2 < 1e-10);
	printf(" [PASSED]\n");
}

TEST(CPU_code, trans_I_2x2_3x3_test)
{
	printf("trans_I_2x2_3x3_test...");
	// Outputs for reference
	float out_012[4][4];
	out_012[0][0] = 0; out_012[0][1] = -16; out_012[0][2] = 0; out_012[0][3] = 0;
	out_012[1][0] = -4; out_012[1][1] = 30; out_012[1][2] = 2; out_012[1][3] = -4;
	out_012[2][0] = 0; out_012[2][1] = 8; out_012[2][2] = 0; out_012[2][3] = 0;
	out_012[3][0] = 0; out_012[3][1] = -16; out_012[3][2] = 0; out_012[3][3] = 0;
	float out_111[4][4];
	out_111[0][0] = 0; out_111[0][1] = 0; out_111[0][2] = 0; out_111[0][3] = 0;
	out_111[1][0] = 0; out_111[1][1] = 4; out_111[1][2] = 0; out_111[1][3] = 0;
	out_111[2][0] = 0; out_111[2][1] = 0; out_111[2][2] = 0; out_111[2][3] = 0;
	out_111[3][0] = 0; out_111[3][1] = 0; out_111[3][2] = 0; out_111[3][3] = 0;

	float in1[4][4]; int k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in1[i][j] = (float)(k++);
		}
	}
	float in2[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in2[i][j] = 1.;
		}
	}
	float out1[4][4];
	trans_I_2x2_3x3(out1, in1);
	float out2[4][4];
	trans_I_2x2_3x3(out2, in2);

	float norm1 = 0.;
	float norm2 = 0.;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; ++j) {
			norm1 += (out1[i][j] - out_012[i][j])*(out1[i][j] - out_012[i][j]);
			norm2 += (out2[i][j] - out_111[i][j])*(out2[i][j] - out_111[i][j]);
		}
	}

	CHECK(norm1 < 1e-10);
	CHECK(norm2 < 1e-10);
	printf(" [PASSED]\n");
}

TEST(CPU_code, trans_O_2x2_3x3_test)
{
	printf("trans_O_2x2_3x3_test...");
	// Outputs for reference
	float out_012[2][2];
	out_012[0][0] = 45; out_012[0][1] = -24; 
	out_012[1][0] = -51; out_012[1][1] = 20; 
	float out_111[2][2];
	out_111[0][0] = 9; out_111[0][1] = -3;
	out_111[1][0] = -3; out_111[1][1] = 1;

	float in1[4][4]; int k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in1[i][j] = (float)(k++);
		}
	}
	float in2[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; ++j) {
			in2[i][j] = 1.;
		}
	}
	float out1[2][2];
	trans_O_2x2_3x3(out1,in1);
	float out2[2][2];
	trans_O_2x2_3x3(out2,in2);

	float norm1 = 0.;
	float norm2 = 0.;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; ++j) {
			norm1 += (out1[i][j] - out_012[i][j])*(out1[i][j] - out_012[i][j]);
			norm2 += (out2[i][j] - out_111[i][j])*(out2[i][j] - out_111[i][j]);
		}
	}

	CHECK(norm1 < 1e-10);
	CHECK(norm2 < 1e-10);
	printf(" [PASSED]\n");
}

TEST(CPU_code, image_slice_test)
{
	printf("image_slice_test...");
	int start, stop;
	int pad[2];

	image_slice(0, 4, 2, 4, 1, &start, &stop, pad);

	LONGS_EQUAL(start, 0);
	LONGS_EQUAL(stop,3);
	LONGS_EQUAL(pad[0], 1);
	LONGS_EQUAL(pad[1], 0);
	printf(" [PASSED]\n");
}

TEST(CPU_code, ceil_div)
{
	printf("ceil_div...");
	LONGS_EQUAL(1,ceil_div(4, 4));
	LONGS_EQUAL(2,ceil_div(4, 3));
	LONGS_EQUAL(2,ceil_div(4, 2));
	LONGS_EQUAL(1,ceil_div(4, 5));
	LONGS_EQUAL(1,ceil_div(4, 16));
	//LONGS_EQUAL(-4,ceil_div(-4, 1));
	//LONGS_EQUAL(-2,ceil_div(-4, 2));
	//LONGS_EQUAL(-1,ceil_div(-4, 3));
	//LONGS_EQUAL(-1,ceil_div(-4, 4));
	//LONGS_EQUAL(0,ceil_div(-4, 5));
	printf(" [PASSED]\n");
}