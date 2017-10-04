// cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(15);
	cout << "Hello" << endl;
	for (size_t i = 0; i < vec.size(); i++)
	{
		cout << "vec[" << i << "]=" << vec.at(i) << endl;
	}
	return 0;
}

