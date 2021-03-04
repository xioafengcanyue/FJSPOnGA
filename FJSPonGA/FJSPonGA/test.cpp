#include"FJSPOnGA.h"
#include<ctime>
#include<algorithm>
#include <iostream>
using namespace std;

int main()
{
	//Indiv t;
	//int num = 0;
	//t.processCode = {7,5,8,1,6,3,0,9,1,8,2,4,0,0,9,4,6,5,1,7,2,3,1,5,7,8,2,6,8,8,1,2,3,1,6};
	//t.machineCode = { 0,1,2,3,4,5,6,7,0,2,1,3,5,6,7,7,0,2,3,1,5,3,6,4,7,1,3,5,1,3,4,0,1,3,5 };
	//t.Decode();
	//for (int i = 0; i < t.fintess.size(); i++)
	//	cout << t.fintess[i] << ends;//较优的一组解测试
	srand(time(0));
	FJSPOnGA test_1;
	test_1.FastSort();
	test_1.ShowFather();
	test_1.Select();
	test_1.IPOXCross();
	test_1.MPXCross();
	test_1.PrcVariation();
	test_1.MacVariation();
	for (int gen = 0; gen < MAXGEN; gen++)
	{
		cout << gen << endl;
		test_1.NSGAFastSort();
		test_1.ShowFather();
		test_1.Select();
		test_1.IPOXCross();
		test_1.MPXCross();
		test_1.PrcVariation();
		test_1.MacVariation();
	}
	test_1.ShowSon();
}