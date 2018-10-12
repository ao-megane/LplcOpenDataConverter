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
	//int ID;	//�z��Ő錾���Ă��̔ԍ���ID�Ƃ��Ă��悢
	int x;	//�}�b�v�̍����牽�Ԗڂ̃}�X���Ď����Ă邩
	int y;	//�}�b�v�̏ォ�牽�Ԗڂ̃}�X���Ď����Ă邩
	int in;	//�u�ԓI��in��
	int out;//�u�ԓI��out��
	//int sumIn;	//����Ȃ�����
	//int sumOut;	//����Ȃ�����

};



#endif // !SENSOR_H
