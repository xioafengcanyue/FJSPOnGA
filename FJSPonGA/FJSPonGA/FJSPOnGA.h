#include<string>
#include<vector>
using namespace std;
const int WPNUM = 10;//总工作个数
const int JUDGENUM = 6;//优化函数数目
const int MACHINENUM = 8;//机器个数
const int MAX = 32767;
const int POPULATION = 100;//种群数目
const double CROSSP = 0.9;//交叉率
const double VARIATION = 0.02;//变异率
const int MAXGEN = 80;//最大世代
class Indiv 
{
public:
	Indiv();
	Indiv(vector<int> v1, vector<int> v2);
	Indiv(const Indiv &indiv);
	Indiv &operator=(const Indiv &indiv);
	bool &operator==(const Indiv &indiv);
	bool &operator!=(const Indiv &indiv);
	void Decode();//解码操作，计算目标函数值
	vector<int> processCode;//操作工序
	vector<int> machineCode;//机器工序
	vector<int> fintess;//目标函数值
	int rank;//非支配等级
	int crowd;//拥挤度
};
class FJSPOnGA
{
public:
	FJSPOnGA();
	void FastSort();
	void NSGAFastSort();
	void Select();
	void IPOXCross();
	void MPXCross();
	void PrcVariation();
	void MacVariation();
	void ShowFather();
	void ShowSon();
	void AHPDecide();
	bool Isdominate(Indiv s,Indiv t);//s是否支配t
	vector<Indiv> population;//父代种群
	vector<Indiv> offspring;//子代种群
};


