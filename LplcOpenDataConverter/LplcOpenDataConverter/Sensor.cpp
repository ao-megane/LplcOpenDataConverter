#include"Sensor.h"

static int id = 0;
Sensor::Sensor() {
	ID = id++;
	//x = 0;
	//y = 0;
	pin = 0;
	pout = 0;
	sumpIn = 0;
	sumpOut = 0;
	nin = 0;
	nout = 0;
	sumnIn = 0;
	sumnOut = 0;
}

Sensor::~Sensor() {
}

//int Sensor::Set(int a, int b) {
//	x = a;
//	y = b;
//	return 0;
//}
int Sensor::Initialize() {
	pin = 0;
	pout = 0;
	sumpIn = 0;
	sumpOut = 0;
	nin = 0;
	nout = 0;
	sumnIn = 0;
	sumnOut = 0;
	return 0;
}
int Sensor::nAdd(int indata, int outdata) {
	nin = indata;
	nout = outdata;
	sumnIn += nin;
	sumnOut += nout;
	return 0;
}
int Sensor::pAdd(int indata, int outdata) {
	pin = indata;
	pout = outdata;
	sumpIn += pin;
	sumpOut += pout;
	return 0;
}
int Sensor::GetID() {
	return ID;
}
int Sensor::GetnIn() {
	return nin;
}
int Sensor::GetnOut() {
	return nout;
}
int Sensor::GetSumnIn() {
	return sumnIn;
}
int Sensor::GetSumnOut() {
	return sumnOut;
}
double Sensor::GetRatio() {//不要なプライバシデータ流出比率
	return (sumnIn + sumnOut) / (sumnIn + sumnOut + sumpIn + sumpOut);
}
int Sensor::GetAllData() {
	return (sumnIn + sumnOut + sumpIn + sumpOut);
}