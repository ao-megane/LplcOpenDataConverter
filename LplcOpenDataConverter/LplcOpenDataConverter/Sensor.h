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
	//int Set(int x, int y);
	int Initialize();
	int nAdd(int in, int out);
	int pAdd(int in, int out);
	int GetID();
	int GetnIn();
	int GetnOut();
	int GetSumnIn();
	int GetSumnOut();
	double GetRatio();
	~Sensor();

private:
	int ID;	//�z��Ő錾���Ă��̔ԍ���ID�Ƃ��Ă��悢
	//int x;	//�}�b�v�̍����牽�Ԗڂ̃}�X���Ď����Ă邩
	//int y;	//�}�b�v�̏ォ�牽�Ԗڂ̃}�X���Ď����Ă邩
	int nin;	//�u�ԓI��in��
	int nout;	//�u�ԓI��out��
	int pin;	//�Ώێ҂̏u�ԓI��in��
	int pout;	//�Ώێ҂̏u�ԓI��out��
	int sumnIn;		//����Ȃ�����
	int sumnOut;	//����Ȃ�����
	int sumpIn;		//����Ȃ�����
	int sumpOut;	//����Ȃ�����

};



#endif // !SENSOR_H
