#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Sensor.h"

using namespace std;

vector<string> split(string& input, char delimiter){//delimiterでinputを分割し，vector<string>にまとめて返す
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

int main(){
	//std::cout << "Hello World\n" << std::endl;
	ifstream ifs("opendatas/20180501.csv");
	//センサID，yyyy/m/d,h:mm,in,out,in累計,out累計\n
	ofstream ofs;
	ofs.open("opendatas/output.csv", ios::trunc);

	Sensor sensor[19];
	string line;
	int sensorinsum[19] = { 0 };
	int i = 0;
	while(getline(ifs, line)) {
		vector<string> strvec = split(line, ',');

		sensor[i].Update(stoi(strvec.at(3)), stoi(strvec.at(4)));

		/*for (int i = 0; i<strvec.size(); i++) {
			printf("%d\n", stoi(strvec.at(i)));
		}*/

		//std::cout << i+1 << sensor[i].GetIn() << sensor[i].GetOut() << std::endl;

		sensorinsum[i] += sensor[i].GetIn();

		if (i < 18) i++;
		else i = 0;
	}

	for (int i = 0; i < 19; i++) {
		ofs << sensorinsum[i] << "," << i + 1 << endl;
		//ofs << "aaaaaaaaa" << endl;
	}

	return 0;
}