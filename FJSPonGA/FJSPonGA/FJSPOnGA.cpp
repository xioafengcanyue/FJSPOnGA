#include"FJSPOnGA.h"
#include<numeric>
#include<algorithm>
#include<iostream>
using namespace std;
vector<int> processNum = { 3,6,4,3,2,3,4,3,5,2 };
vector<int> relaeseTime = { 5,17,16,4,17,10,12,0,16,2 };
vector<int> unitProCost = { 8,5,10,9,7,6,9,4 };
vector<vector<vector<int>>> processTime = { {{0,0,0,12,0,10,9,0},{17,0,0,0,17,10,0,15,},{0,24,0,11,0,0,10,0}},
											{{0,0,16,10,21,14,0,17},{8,12,0,0,19,11,0,0},{0,0,0,15,0,21,25,0},{0,0,0,0,18,0,9,0},{12,15,0,14,9,0,10,0},{0,9,0,7,10,8,0,0}},
											{{0,14,0,0,0,0,17,0},{0,23,23,17,0,18,0,0},{0,0,20,9,22,0,0,21},{7,0,10,0,8,11,9,0}},
											{{0,18,0,0,0,17,18,0},{0,0,0,10,0,0,12,0},{0,8,11,8,0,0,9,20}},
											{{0,0,24,10,16,0,0,0},{0,0,0,17,0,0,0,8}},
											{{0,20,0,0,0,0,22,0},{0,0,18,0,0,0,19,0},{19,0,17,16,16,0,0,18}},
											{{0,0,16,0,17,16,0,0},{22,0,0,0,0,20,22,0,},{0,0,0,0,0,18,23,0},{0,0,0,0,0,10,0,24}},
											{{11,0,0,0,0,0,21,21},{25,11,0,0,0,0,0,0},{24,18,0,0,0,25,0,20}},
											{{0,12,9,0,0,14,0,0},{24,0,11,21,0,22,25,0},{0,12,0,0,0,16,0,19},{0,6,0,8,7,0,10,0},{9,12,0,6,11,8,10,0}},
											{{0,19,0,0,0,0,21,23},{0,18,0,0,0,24,0,12} } };
vector<vector<vector<int>>> avaiMachine = { {{3,5,6},{0,4,5,7},{1,3,6}},
											{{2,3,4,5,7},{0,1,4,5} ,{3,5,6},{4,6},{0,1,3,4,6},{1,3,4,5}},
											{{1,6},{1,2,3,5},{2,3,4,7},{0,2,4,5,6}},
											{{1,5,6},{3,6},{1,2,3,6,7}},
											{{2,3,4},{3,7}},
											{{1,6},{2,6},{0,2,3,4,7}},
											{{2,4,5},{0,5,6},{5,6},{5,7} },
											{{0,6,7},{0,1},{0,1,5,7} },
											{{1,2,5},{0,2,3,5,6},{1,5,7},{1,3,4,6},{0,1,3,4,5,6} },
											{{1,6,7},{1,5,7}} };
vector<int> deadLine = { 60,0,0,80,60,80,0,80,0,60 };
vector<vector<double>> judgeMatrix = { {1.0,7.0,1.0 / 2,6.0,5.0,3.0}
									,{1.0 / 7,1.0,1.0 / 9,1.0 / 3,1.0 / 4,1.0 / 5}
									,{2.0,9.0,1.0,7.0,5.0,3.0}
									,{1.0 / 6,3.0,1.0 / 7,1.0,1.0 / 2,1.0 / 5}
									,{1.0 / 5,4.0,1.0 / 5,2.0,1.0,1.0 / 3}
									,{1.0 / 3,5.0,1.0 / 3,5.0,3.0,1.0} };


Indiv::Indiv()
{
	rank = crowd = 0;
	vector<int> flag(WPNUM, 0);
	for (int i = 0; i < WPNUM; i++)
		for (int j = 0; j < processNum[i]; j++)
			processCode.push_back(i);
	random_shuffle(processCode.begin(), processCode.end());//随机生成工序次序
	for (unsigned int i = 0; i < processCode.size(); i++)
	{
		vector<int> v = avaiMachine[processCode[i]][flag[processCode[i]]];
		flag[processCode[i]]++;
		random_shuffle(v.begin(), v.end());
		machineCode.push_back(v[0]);
	}//随机生成机器次序
	Decode();//解码
}

Indiv::Indiv(vector<int> v1, vector<int> v2)
{
	processCode = v1;
	machineCode = v2;
}

Indiv::Indiv(const Indiv & indiv)
{
	this->fintess = indiv.fintess;
	this->processCode = indiv.processCode;
	this->machineCode = indiv.machineCode;
	this->rank = indiv.rank;
	this->crowd = indiv.crowd;
}

Indiv & Indiv::operator=(const Indiv & indiv)
{
	this->fintess = indiv.fintess;
	this->processCode = indiv.processCode;
	this->machineCode = indiv.machineCode;
	this->rank = indiv.rank;
	this->crowd = indiv.crowd;
	return *this;
}

bool & Indiv::operator==(const Indiv & indiv)
{
	bool flag = false;
	for (int i = 0; i < this->fintess.size(); i++)
		if (this->fintess[i] != indiv.fintess[i])
			return flag;
	flag = true;
	return flag;

}
bool & Indiv::operator!=(const Indiv & indiv)
{
	bool flag = true;
	for (int i = 0; i < this->fintess.size(); i++)
		if (this->fintess[i] != indiv.fintess[i])
			return flag;
	flag = false;
	return flag;
}

void Indiv::Decode()//根据贪婪算法解码，会根据最优在空闲时间插入，计算各个目标函数值
{
	vector<int>().swap(fintess);
	vector<vector<int> > macOccupy(MACHINENUM, vector<int>());
	vector<int> wpPreTime(WPNUM, 0);//工件上一道工序处理完时的时间
	vector<int> wpOrder(WPNUM, 0);//每个工件进行到的工序数
	vector<int> macLoadTime(MACHINENUM, 0);
	for (unsigned int i = 0; i < processCode.size(); i++)
	{
		int wpNumber = processCode[i];//处理的工件序号
		int macNumber = machineCode[i];//工件应置于机器的序号
		int curProTime = 0;//此次工序需要加工的时间
		macLoadTime[macNumber] += processTime[wpNumber][wpOrder[wpNumber]][macNumber];
		if (wpOrder[wpNumber] == 0)//判断是否是第一道工序
			curProTime = relaeseTime[wpNumber] + processTime[wpNumber][wpOrder[wpNumber]][macNumber];
		else curProTime = processTime[wpNumber][wpOrder[wpNumber]][macNumber];
		int maxTimeF = max(0, wpPreTime[wpNumber]);
		if (macOccupy[macNumber].size() == 0 || macOccupy[macNumber][0] >= (maxTimeF + curProTime))//判断是否在空时插入工序
		{
			macOccupy[macNumber].push_back(maxTimeF);
			macOccupy[macNumber].push_back(maxTimeF + curProTime);
			wpPreTime[wpNumber] = maxTimeF + curProTime;
		}
		else//判断是否在中间添加工序
		{
			for (unsigned int j = 1; j < macOccupy[macNumber].size(); j = j + 2)
			{
				int maxTime = max(macOccupy[macNumber][j], wpPreTime[wpNumber]);
				if (j == macOccupy[macNumber].size() - 1 ||
					macOccupy[macNumber][j + 1] <= (maxTime + curProTime))//判断是否在最后添加工序
				{
					macOccupy[macNumber].push_back(maxTime);
					macOccupy[macNumber].push_back(maxTime + curProTime);
					wpPreTime[wpNumber] = maxTime + curProTime;
					break;
				}
			}
		}
		sort(macOccupy[macNumber].begin(), macOccupy[macNumber].end());
		wpOrder[wpNumber]++;
	}
	fintess.push_back(*max_element(wpPreTime.begin(), wpPreTime.end()));
	fintess.push_back((accumulate(wpPreTime.begin(), wpPreTime.end(), 0)
		- accumulate(relaeseTime.begin(), relaeseTime.end(), 0)) / WPNUM);
	int totalDetime = 0;
	for (int i = 0; i < WPNUM; i++)
	{
		if (wpPreTime[i] - deadLine[i] > 0 && deadLine[i] != 0)
			totalDetime += wpPreTime[i] - deadLine[i];
	}
	fintess.push_back(totalDetime);
	fintess.push_back(accumulate(macLoadTime.begin(), macLoadTime.end(), 0));
	fintess.push_back(*max_element(macLoadTime.begin(), macLoadTime.end()));
	int proCost = 0;
	for (unsigned int i = 0; i < macLoadTime.size(); i++)
		proCost += macLoadTime[i] * unitProCost[i];
	fintess.push_back(proCost);
}

FJSPOnGA::FJSPOnGA()
{
	for (int i = 0; i < POPULATION; i++)
		population.push_back(Indiv());
}


void FJSPOnGA::FastSort()//非支配集排序
{
	vector<Indiv> p(population);
	vector<Indiv> p1(population);
	int n = p1.size();
	int ranknum = 1;
	while (n != 0)
	{
		vector<Indiv> rank;
		while (p.size() != 0)
		{
			vector<Indiv> temp;
			bool flag = true;
			for (int i = 1; i < p.size(); i++)
				if (!Isdominate(p[0], p[i]))
				{
					if (Isdominate(p[i], p[0]))
						flag = false;
					temp.push_back(p[i]);
				}
			if (flag)
				rank.push_back(p[0]);
			p = temp;
		}
		n -= rank.size();
		for (int i = 0; i < population.size(); i++)
			if (count(rank.begin(), rank.end(), population[i]))
				population[i].rank = ranknum;
		ranknum++;
		for (int i = 0; i < p1.size(); i++)
			if (!count(rank.begin(), rank.end(), p1[i]))
				p.push_back(p1[i]);
		p1 = p;
	}
	sort(population.begin(), population.end(), [](Indiv x, Indiv y) {return x.rank < y.rank; });
}


void FJSPOnGA::NSGAFastSort()//非支配集排序并计算拥挤度
{
	vector<Indiv> allIndiv(POPULATION * 2);
	for (int i = 0; i < POPULATION; i++)
	{
		allIndiv[i] = population[i];
		allIndiv[i + POPULATION] = offspring[i];
		allIndiv[i].crowd = allIndiv[i + POPULATION].crowd= 0;
	}	
	vector<Indiv> p(allIndiv);
	vector<Indiv> p1(allIndiv);
	int n = p1.size();
	int ranknum = 1;
	while (n != 0)
	{
		vector<Indiv> rank;
		while (p.size() != 0)
		{
			vector<Indiv> temp;
			bool flag = true;
			for (int i = 1; i < p.size(); i++)
				if (!Isdominate(p[0], p[i]))
				{
					if (Isdominate(p[i], p[0]))
						flag = false;
					temp.push_back(p[i]);
				}
			if (flag)
				rank.push_back(p[0]);
			p = temp;
		}
		n -= rank.size();
		for (int i = 0; i < allIndiv.size(); i++)
			if (count(rank.begin(), rank.end(), allIndiv[i]))
			{
				cout << allIndiv[i].rank << ends;
				allIndiv[i].rank = ranknum;
				cout << allIndiv[i].rank << ends;
			}
		ranknum++;
		for (int i = 0; i < p1.size(); i++)
			if (!count(rank.begin(), rank.end(), p1[i]))
				p.push_back(p1[i]);
		p1 = p;
	}
	sort(allIndiv.begin(),allIndiv.end(), [](Indiv x, Indiv y) {return x.rank < y.rank; });
	vector<Indiv> newOffspring;
	int flag = allIndiv[POPULATION].rank;//种群末尾分界的rank值
	for (unsigned int i = 0; i < allIndiv.size(); i++)//所有等于种群末尾rank值的个体挑出计算拥挤度
		if (allIndiv[i].rank == flag)
			newOffspring.push_back(allIndiv[i]);
	for (int i = 0; i < JUDGENUM; i++)//对于每个目标函数值
	{
		vector<int> number;
		for (unsigned int j = 0; j < newOffspring.size(); j++)
			number.push_back(j);
		sort(number.begin(), number.end(), [=](int x, int y) {
			return newOffspring[x].fintess[i] < newOffspring[y].fintess[i];
		});//对于每个目标函数值进行升序排序，较优在前面
		for (unsigned int j = 1; j < newOffspring.size() - 1; j++)
			if(newOffspring[number[j]].crowd!=MAX)
				newOffspring[number[j]].crowd += newOffspring[number[j + 1]].fintess[i] - newOffspring[number[j - 1]].fintess[i];
		newOffspring[number[0]].crowd = newOffspring[number[newOffspring.size() - 1]].crowd = MAX;
	}
	sort(newOffspring.begin(), newOffspring.end(), [](Indiv x, Indiv y) { return x.crowd > y.crowd; });
	int k = 0;
	for (int i = 0; i < POPULATION; i++)
	{
		if (allIndiv[i].rank != flag)
			population[i] = allIndiv[i];
		else
			population[i] = newOffspring[k++];
	}
}

void FJSPOnGA::Select()//锦标赛选择，依据rank和crowd的值进行选择
{
	vector<Indiv>().swap(offspring);//销毁offspring内容
	for (int i = 0; i < POPULATION; i++)
	{
		vector<int> num;
		vector<int> rankv;
		vector<int> order;
		for (int j = 0; j < POPULATION; j++)
			num.push_back(j);
		random_shuffle(num.begin(), num.end());
		for (int j = 0; j < POPULATION /2; j++)
		{
			//cout << num[j];
			rankv.push_back(population[num[j]].rank);
			order.push_back(num[j]);
		}
		int win=0;
		int minrank = *min_element(rankv.begin(), rankv.end());
		int maxcrowd = -32767;
		for (int j = 0; j < order.size(); j++)
		{
			if (population[order[j]].rank != minrank)
				continue;
			 int count = population[order[j]].crowd;
			if (count > maxcrowd)
			{
				maxcrowd = count;
				win = j;
			}
		}
		//cout << order[win] << endl;
		offspring.push_back(population[order[win]]);
	}
}

void FJSPOnGA::IPOXCross()
{
	int crossNum = static_cast<int>(CROSSP * POPULATION) / 2;//计算交叉对数目
	vector<int> number;//产生所有个体的序号
	for (int i = 0; i < POPULATION; i++)
		number.push_back(i);
	random_shuffle(number.begin(), number.end());//打乱个体序号，取最前面的进行交叉
	for (int i = 0; i < crossNum; i++)
	{
		vector<int> proSonFirst(offspring[number[i]].processCode.size(), -1);//两个子代，-1表示该基因未完成操作
		vector<int> proSonSecond(offspring[number[i]].processCode.size(), -1);
		vector<int> macSonFirst(offspring[number[i]].machineCode.size(), -1);
		vector<int> macSonSecond(offspring[number[i]].machineCode.size(), -1);
		vector<int> wpSet;//所有工件的序号集合
		for (int j = 0; j < WPNUM; j++)
			wpSet.push_back(j);
		random_shuffle(wpSet.begin(), wpSet.end());
		int x = rand() % (WPNUM - 1) + 1;//序号集合随机分成两部分
		vector<int> wpFirst(wpSet.begin(), wpSet.begin() + x);
		vector<int> wpSecond(wpSet.begin() + x, wpSet.end());
		for (unsigned int j = 0; j < offspring[number[i]].processCode.size(); j++)
		{
			if (any_of(wpFirst.begin(), wpFirst.end(), [=](int x) {//染色体1中的基因在集合1时，按位置赋给子代1
				return offspring[number[i]].processCode[j] == x;
			}))
			{
				proSonFirst[j] = offspring[number[i]].processCode[j];
				macSonFirst[j] = offspring[number[i]].machineCode[j];
			}
			if (any_of(wpSecond.begin(), wpSecond.end(), [=](int x) {//染色体2中的基因在集2时，按位置赋给子代2
				return offspring[number[i + crossNum]].processCode[j] == x;
			}))
			{
				proSonSecond[j] = offspring[number[i + crossNum]].processCode[j];
				macSonSecond[j] = offspring[number[i + crossNum]].machineCode[j];
			}
		}
		for (unsigned int j = 0; j < offspring[number[i]].processCode.size(); j++)
		{
			if (any_of(wpFirst.begin(), wpFirst.end(), [=](int x) {//染色体1中的基因在集合1时，按顺序赋给子代2
				return offspring[number[i]].processCode[j] == x;
			}))
			{
				auto it = find(proSonSecond.begin(), proSonSecond.end(), -1);
				int Loction = distance(proSonSecond.begin(), it);
				*it = offspring[number[i]].processCode[j];
				macSonSecond[Loction] = offspring[number[i]].machineCode[j];
			}
			if (any_of(wpSecond.begin(), wpSecond.end(), [=](int x) {//染色体2中的基因在集合2时，按顺序赋给子代1
				return offspring[number[i + crossNum]].processCode[j] == x;
			}))
			{
				auto it = find(proSonFirst.begin(), proSonFirst.end(), -1);
				int Loction = distance(proSonFirst.begin(), it);
				*it = offspring[number[i + crossNum]].processCode[j];
				macSonFirst[Loction] = offspring[number[i + crossNum]].machineCode[j];
			}
		}
		offspring[number[i]].processCode = proSonFirst;
		offspring[number[i]].machineCode = macSonFirst;
		offspring[number[i + crossNum]].processCode = proSonSecond;
		offspring[number[i + crossNum]].machineCode = macSonSecond;
	}


}

void FJSPOnGA::MPXCross()
{
	int crossNum = static_cast<int>(CROSSP * POPULATION) / 2;//计算交叉对数目
	vector<int> number;//产生所有个体的序号
	for (int i = 0; i < POPULATION; i++)
		number.push_back(i);
	random_shuffle(number.begin(), number.end());//打乱个体序号，取最前面的进行交叉
	for (int i = 0; i < crossNum; i++)
	{
		vector<int> setR;
		for (unsigned int j = 0; j < offspring[number[i]].machineCode.size(); j++)
			setR.push_back(rand() % 2);
		for (unsigned int j = 0; j < setR.size(); j++)
			if (setR[j] == 1 && offspring[number[i]].processCode[j] == offspring[number[i + crossNum]].processCode[j])
			{
				swap(offspring[number[i]].machineCode[j], offspring[number[i + crossNum]].machineCode[j]);
				//cout << number[i]<<"   "<<number[i+crossNum] << "  " << j << endl;
			}
	}
}

void FJSPOnGA::PrcVariation()
{
	int varNum = static_cast<int>(POPULATION*VARIATION);//变异数目
	vector<int> number;
	for (int i = 0; i < POPULATION; i++)
		number.push_back(i);
	random_shuffle(number.begin(), number.end());
	for (int i = 0; i < varNum; i++)
	{
		int varPrc = rand() % offspring[number[i]].processCode.size();
		int varLocation = rand() % offspring[number[i]].processCode.size();
		//cout << number[i] << "  " << varPrc << "  " << varLocation << endl;
		int tempPro = offspring[number[i]].processCode[varPrc];
		int tempMac = offspring[number[i]].machineCode[varPrc];
		if (varPrc < varLocation)
		{
			for (int j = varPrc; j < varLocation; j++)
			{
				offspring[number[i]].processCode[j] = offspring[number[i]].processCode[j + 1];
				offspring[number[i]].machineCode[j] = offspring[number[i]].machineCode[j + 1];
			}
		}
		else
		{
			for (int j = varPrc; j > varLocation; j--)
			{
				offspring[number[i]].processCode[j] = offspring[number[i]].processCode[j - 1];
				offspring[number[i]].machineCode[j] = offspring[number[i]].machineCode[j - 1];
			}
		}
		offspring[number[i]].processCode[varLocation] = tempPro;
		offspring[number[i]].machineCode[varLocation] = tempMac;
	}
}

void FJSPOnGA::MacVariation()
{
	int varNum = static_cast<int>(POPULATION*VARIATION);//变异数目
	vector<int> number;
	for (int i = 0; i < POPULATION; i++)
		number.push_back(i);
	random_shuffle(number.begin(), number.end());
	for (int i = 0; i < varNum; i++)
	{
		int firstLoc, secondLoc;
		do
		{
			firstLoc = rand() % offspring[number[i]].machineCode.size();
			secondLoc = rand() % offspring[number[i]].machineCode.size();
		} while (firstLoc == secondLoc);
		//cout << number[i] << "  " << firstLoc << " " << secondLoc << endl;
		int order = 0;
		for (int k = 0; k < firstLoc; k++)
			if (offspring[number[i]].processCode[firstLoc] == offspring[number[i]].processCode[k])
				order++;
		offspring[number[i]].machineCode[firstLoc] =
			avaiMachine[offspring[number[i]].processCode[firstLoc]][order][rand() % avaiMachine[offspring[number[i]].processCode[firstLoc]][order].size()];
		//cout << offspring[number[i]].processCode[firstLoc] << "   " << order << endl;
		order = 0;
		for (int k = 0; k < secondLoc; k++)
			if (offspring[number[i]].processCode[secondLoc] == offspring[number[i]].processCode[k])
				order++;
		offspring[number[i]].machineCode[secondLoc] =
			avaiMachine[offspring[number[i]].processCode[secondLoc]][order][rand() % avaiMachine[offspring[number[i]].processCode[secondLoc]][order].size()];
		//cout << offspring[number[i]].processCode[secondLoc] << "   " << order << endl;
	}
	for (Indiv indiv : offspring)
		indiv.Decode();
}


void FJSPOnGA::ShowFather()//输出每个个体的每个目标函数值，rank和crowd
{
	for (unsigned int i = 0; i < population.size(); i++)
	{
		//for (unsigned int j = 0; j < population[i].processCode.size(); j++)
		//	cout << population[i].processCode[j] << ends;
		//cout << endl;
		//for (unsigned int j = 0; j < population[i].machineCode.size(); j++)
		//	cout << population[i].machineCode[j] << ends;
		//cout << endl;
		for (unsigned int j = 0; j < population[i].fintess.size(); j++)
			cout << population[i].fintess[j] << ',';
		cout << endl;
		cout << population[i].rank <<"    "<<population[i].crowd<< endl;
		cout << endl;
	}
}

void FJSPOnGA::ShowSon()//输出每个个体的每个目标函数值，rank和crowd
{
	for (unsigned int i = 0; i < offspring.size(); i++)
	{
		for (unsigned int j = 0; j < offspring[i].processCode.size(); j++)
			cout << offspring[i].processCode[j] << ends;
		cout << endl;
		for (unsigned int j = 0; j < offspring[i].machineCode.size(); j++)
			cout << offspring[i].machineCode[j] << ends;
		cout << endl;
		for (unsigned int j = 0; j < offspring[i].fintess.size(); j++)
			cout << offspring[i].fintess[j] << ends;
		cout << endl;
		cout << offspring[i].rank <<"    " << population[i].crowd << endl;
		cout << endl;
	}
}

void FJSPOnGA::AHPDecide()
{
	vector<double> judgeVector(judgeMatrix.size(), 0);
	for (unsigned int i = 0; i < judgeMatrix.size(); i++)
		for (unsigned int j = 0; j < judgeMatrix[i].size(); j++)
			judgeVector[i] += judgeMatrix[i][j];
	double judgeSum = accumulate(judgeVector.begin(), judgeVector.end(), 0.0);
	for (unsigned int i = 0; i < judgeVector.size(); i++)
		judgeVector[i] = judgeVector[i] / judgeSum;
}

bool FJSPOnGA::Isdominate(Indiv s, Indiv t)
{
	if (s == t)
		return false;
	vector<int> sub;
	for (int i = 0; i < s.fintess.size(); i++)
		sub.push_back(s.fintess[i] - t.fintess[i]);
	return all_of(sub.begin(), sub.end(), [](int x) {return x <= 0; });
}
