#include"Sensor.h"

static int id = 0;
Sensor::Sensor() {
	ID = id++;
	//x = 0;
	//y = 0;
	in = 0;
	out = 0;
	sumIn = 0;
	sumOut = 0;
}

Sensor::~Sensor() {
}

//int Sensor::Set(int a, int b) {
//	x = a;
//	y = b;
//	return 0;
//}
int Sensor::Add(int indata, int outdata) {
	in = indata;
	out = outdata;
	sumIn += in;
	sumOut += out;
	return 0;
}
int Sensor::GetID() {
	return ID;
}
int Sensor::GetIn() {
	return in;
}
int Sensor::GetOut() {
	return out;
}
int Sensor::GetSumIn() {
	return sumIn;
}
int Sensor::GetSumOut() {
	return sumOut;
}