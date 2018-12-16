#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Sensor
{
public:
	Sensor();
	//int Set(int x, int y);
	int Initialize();
	int pInitialize();
	int nAdd(int in, int out);
	int pAdd(int in, int out);
	int pDivision(double a);
	int GetID();
	int GetnIn();
	int GetnOut();
	int GetSumnIn();
	int GetSumnOut();
	int GetSumpIn();
	int GetSumpOut();
	double GetRatio();
	~Sensor();

private:
	int ID;	//配列で宣言してその番号をIDとしてもよい
	//int x;	//マップの左から何番目のマスを監視してるか
	//int y;	//マップの上から何番目のマスを監視してるか
	int nin;	//瞬間的なin数
	int nout;	//瞬間的なout数
	int pin;	//対象者の瞬間的なin数
	int pout;	//対象者の瞬間的なout数
	int sumnIn;		//いらないかも
	int sumnOut;	//いらないかも
	int sumpIn;		//いらないかも
	int sumpOut;	//いらないかも

};



#endif // !SENSOR_H
