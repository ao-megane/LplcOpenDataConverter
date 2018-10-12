#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Sensor
{
public:
	Sensor();
	int Set(int x, int y);
	int Update(int in, int out);
	int GetIn();
	int GetOut();
	~Sensor();

private:
	//int ID;	//配列で宣言してその番号をIDとしてもよい
	int x;	//マップの左から何番目のマスを監視してるか
	int y;	//マップの上から何番目のマスを監視してるか
	int in;	//瞬間的なin数
	int out;//瞬間的なout数
	//int sumIn;	//いらないかも
	//int sumOut;	//いらないかも

};

static int id = 0;
Sensor::Sensor(){
	//ID = id++;
	x = 0;
	y = 0;
	in = 0;
	out = 0;
	//sumIn = 0;
	//sumOut = 0;
}

Sensor::~Sensor(){
}

#endif // !SENSOR_H
