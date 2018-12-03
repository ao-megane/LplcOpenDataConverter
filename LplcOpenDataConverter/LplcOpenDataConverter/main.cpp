#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Sensor.h"

const string START = "17:00";	//計測開始時刻	//hh:mmという形式なので注意
const string END = "18:00";			//計測時間

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
	vector<vector<int>> in[19];
	vector<vector<int>> out[19];

	bool isTracking = false;

	ifstream ifs;
	ifs.open("mapdatas/allmap/map.txt");	//マップの広さとか
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
	for (int i = 0; i < 19; i++) {//大きさ格納
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
			map[j].resize(width);
		}
	}

	for (int i = 0; i < 19; i++) {//各センサ有効範囲格納
		string a = "mapdatas/allmap/12/";
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
				if (strvec.at(k) == "") in[i][j][k] = 0;
				else in[i][j][k] = stoi(strvec.at(k));
			}
			j++;
		}
		ifs.close();

		a = "mapdatas/allmap/12/";
		a += to_string(i + 1);
		a += "out.csv";
		cout << a << endl;
		ifs.open(a.c_str());
		if (!ifs) {
			cout << "ファイルオープンに失敗" << a << endl;
		}
		j = 0; k = 0;
		while (getline(ifs, line)) {
			vector<string> strvec = split(line, ',');
			for (k = 0; k < strvec.size(); k++) {
				if (strvec.at(k) == "") out[i][j][k] = 0;
				else out[i][j][k] = stoi(strvec.at(k));
			}
			j++;
		}
		ifs.close();

	}
	/*for (int i = 0; i < 19; i++) {
		string a = "mapdatas/allmap/12/";
		a += to_string(i + 1);
		a += "out.csv";
		cout << a << endl;
		ifs.open(a.c_str());
		if (!ifs) {
			cout << "ファイルオープンに失敗" << a << endl;
		}

		int j = 0, k = 0;
		while (getline(ifs, line)) {
			vector<string> strvec = split(line, ',');
			for (k = 0; k < strvec.size(); k++) {
				out[i][j][k] = stoi(strvec.at(k));
			}
			j++;
		}
		ifs.close();
	}*/
	for (int i = 0; i < map.size(); ++i) {//念のための初期化
		for (int j = 0; j < map[i].size(); ++j) {
			map[i][j] = 0;
		}
	}

	cout << "----------------------------" << endl;
	/*for (int i = 0; i < in[0].size(); ++i) {
		printf("%3d ", i);
		for (int j = 0; j < in[0][i].size(); ++j) {
			printf("%2d",in[0][i][j]);
		}
		printf("\n");
	}*/
	

	i = 0;
	//センサID，yyyy/m/d, h:mm, in, out, in累計, out累計\n
	ifs.open("opendatas/20180501.csv");
	Sensor sensor[19];//格納データの内容を扱うクラス
	while (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');//データ一行ゲット
		if (strvec.at(2) == START) {//データの時間評価
			isTracking = true;
		}
		if (isTracking) {
			if (strvec.at(2) == END) {
				isTracking = false;
			}
		}
		if (isTracking) {
			cout << "計測中:";
			cout << strvec.at(2) << endl;
			sensor[i].Add(stoi(strvec.at(3)), stoi(strvec.at(4)));

			/*for (int i = 0; i<strvec.size(); i++) {
			printf("%d\n", stoi(strvec.at(i)));
			}*/

			//std::cout << i+1 << sensor[i].GetIn() << sensor[i].GetOut() << std::endl;

			//sensorinsum[i] += sensor[i].GetIn();

			if (i < 18) {
				i++;
			}
			else {//１周期終了，マップ更新
				cout << "動作中！！" << endl;
				for (int id = 0; id < 19; id++) {
					for (int h = 0; h < height; h++) {
						for (int w = 0; w < width; w++) {
							map[h][w] += in[id][h][w] * sensor[id].GetIn();
							//cout << in[id][h][w] << endl;//ここがおかしい
							map[h][w] += out[id][h][w] * sensor[id].GetOut();
						}
					}
				}

				i = 0;
			}
		}
		else {
			cout << strvec.at(2) << endl;
		}
	}
	
	ofstream ofs;
	ofs.open("results/output.csv", ios::trunc);
	//for (int i = 0; i < 19; i++) {
	//	//ofs << sensorinsum[i] << "," << i + 1 << endl;
	//	ofs << sensor[i].GetID() << "," << sensor[i].GetSumIn() << endl;
	//}

	cout << "----------------------------" << endl;
	for (int i = 0; i < map.size(); ++i) {
		//printf("%3d ", i);
		for (int j = 0; j < map[i].size(); ++j) {
			//printf("%5d", map[i][j]);
			ofs << map[i][j] << ",";
		}
		//printf("\n");
		ofs << endl;
	}

	return 0;
}