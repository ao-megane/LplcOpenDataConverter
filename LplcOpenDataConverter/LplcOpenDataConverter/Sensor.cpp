#include"Sensor.h"

int Sensor::Set(int a, int b) {
	x = a;
	y = b;
	return 0;
}
int Sensor::Update(int a, int b) {
	in = a;
	out = b;
	//sumIn += in;
	//sumOut += out;
	return 0;
}
int Sensor::GetIn() {
	return in;
}
int Sensor::GetOut() {
	return out;
}