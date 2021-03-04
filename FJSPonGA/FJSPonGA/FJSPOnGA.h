#include<string>
#include<vector>
using namespace std;
const int WPNUM = 10;//�ܹ�������
const int JUDGENUM = 6;//�Ż�������Ŀ
const int MACHINENUM = 8;//��������
const int MAX = 32767;
const int POPULATION = 100;//��Ⱥ��Ŀ
const double CROSSP = 0.9;//������
const double VARIATION = 0.02;//������
const int MAXGEN = 80;//�������
class Indiv 
{
public:
	Indiv();
	Indiv(vector<int> v1, vector<int> v2);
	Indiv(const Indiv &indiv);
	Indiv &operator=(const Indiv &indiv);
	bool &operator==(const Indiv &indiv);
	bool &operator!=(const Indiv &indiv);
	void Decode();//�������������Ŀ�꺯��ֵ
	vector<int> processCode;//��������
	vector<int> machineCode;//��������
	vector<int> fintess;//Ŀ�꺯��ֵ
	int rank;//��֧��ȼ�
	int crowd;//ӵ����
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
	bool Isdominate(Indiv s,Indiv t);//s�Ƿ�֧��t
	vector<Indiv> population;//������Ⱥ
	vector<Indiv> offspring;//�Ӵ���Ⱥ
};


