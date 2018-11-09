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
	string line;
	int i = 0;
	int width = 0;
	int height = 0;
	vector<vector<int>> map;
	vector<vector<int>> in[8];
	vector<vector<int>> out[8];


	ifstream ifs;
	ifs.open("mapdatas/map.txt");	//マップの広さとか
	if (!ifs) {
		cout << "ファイルオープンに失敗" << "mapdatas/map.txt" << endl;
	}
	int mapdata[2] = { 0 };
	while (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		mapdata[i] = stoi(strvec.at(0));
		i++;
	}
	ifs.close();
	width = mapdata[0];
	height = mapdata[1];//マップの広さを格納

	map.resize(height);
	for (int i = 0; i < 8; i++) {
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
			map[j].resize(width);
		}
	}

	for (int i = 0; i < 8; i++) {
		string a = "mapdatas/";
		a += to_string(i + 1);
		a += "in.csv";
		cout << a << endl;
		ifs.open(a.c_str());
		if (!ifs) {
			cout << "ファイルオープンに失敗" << a << endl;
		}

		int j = 0, k = 0;
		while (getline(ifs, line)) {
			vector<string> strvec = split(line, ',');

			for (k = 0; k < strvec.size(); k++) {
				in[i][j][k] = stoi(strvec.at(k));
			}

			j++;
			cout << endl;
		}
		ifs.close();
	}

	cout << "----------------------------" << endl;
	for (int i = 0; i < in[0].size(); ++i) {
		printf("%3d ", i);
		for (int j = 0; j < in[0][i].size(); ++j) {
			printf("%2d",in[0][i][j]);
		}
		printf("\n");
	}
	

	i = 0;
	//センサID，yyyy/m/d,h:mm,in,out,in累計,out累計\n
	ifs.open("opendatas/20180501.csv");
	Sensor sensor[19];
	int sensorinsum[19] = { 0 };
	while (getline(ifs, line)) {
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
	
	ofstream ofs;
	ofs.open("opendatas/output.csv", ios::trunc);
	for (int i = 0; i < 19; i++) {
		ofs << sensorinsum[i] << "," << i + 1 << endl;
	}

	return 0;
}