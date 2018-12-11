#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <iomanip>
#include "Sensor.h"
#include "date.h"

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
	string start;	//計測開始時刻	//hh:mmという形式なので注意
	string end;		//計測終了時刻
	string filename;	//たびたび変わるファイルネーム，使いまわしまくるので初期化を忘れないように
	int year;
	int month;
	int date;
	int i = 0;
	int width = 0;
	int height = 0;
	int posnum = 0;//実際はposnum*3人ずつ増えてる
	vector<vector<int>> map;
	vector<vector<int>> positive;
	vector<vector<int>> in[19];
	vector<vector<int>> out[19];

	Sensor sensor[19];//格納データの内容を扱うクラス

	bool isTracking = false;

	ifstream ifs;
	ofstream ofs;
	ifs.open("mapdatas/allmap/map.txt");	//マップの広さとか
	if (!ifs) {
		cout << "ファイルオープンに失敗" << "mapdatas/map.txt" << endl;
	}
	int mapdata[2] = { 0 };
	if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		mapdata[0] = stoi(strvec.at(0));
	}
	if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		mapdata[1] = stoi(strvec.at(0));
	}
	if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		start = strvec.at(0);
	}
	if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		end = strvec.at(0);
	}
	if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		posnum = stoi(strvec.at(0));
	}
	ifs.close();
	width = mapdata[0];
	height = mapdata[1];//マップの広さを格納

	map.resize(height);
	positive.resize(height);
	for (int i = 0; i < 19; i++) {//大きさ格納
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
			map[j].resize(width);
			positive[j].resize(width);
		}
	}

	//posファイル読み込み&格納
	filename = "mapdatas/allmap/pos/pos.csv";
	cout << filename << endl;
	ifs.open(filename.c_str());
	if (!ifs) {
		cout << "ファイルオープンに失敗" << filename << endl;
	}
	int j = 0, k = 0;
	while (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		for (k = 0; k < strvec.size(); k++) {
			if (strvec.at(k) == "") positive[j][k] = 0;
			else positive[j][k] = stoi(strvec.at(k));
		}
		j++;
	}
	ifs.close();

	//センサ有効範囲についてのforループ
	for (int range = 0; range < 12; range++) {
		//各センサに有効範囲を格納(センサについてのforループ)
		for (int i = 0; i < 19; i++) {
			filename = "mapdatas/allmap/";
			filename += to_string(range + 1);
			filename += "/";
			filename += to_string(i + 1);
			filename += "in.csv";

			cout << filename << endl;
			ifs.open(filename.c_str());
			if (!ifs) {
				cout << "ファイルオープンに失敗" << filename << endl;
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

			filename = "mapdatas/allmap/";
			filename += to_string(range + 1);
			filename += "/";
			filename += to_string(i + 1);
			filename += "out.csv";
			cout << filename << endl;
			ifs.open(filename.c_str());
			if (!ifs) {
				cout << "ファイルオープンに失敗" << filename << endl;
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

		//センサ初期化
		for (int i = 0; i < 19; i++) {
			sensor[i].Initialize();
		}

		//マップ初期化
		for (int i = 0; i < map.size(); ++i) {
			for (int j = 0; j < map[i].size(); ++j) {
				map[i][j] = 0;
			}
		}
		cout << "----------------------------" << endl;

		//日付初期化
		year = 2018;
		month = 6;
		date = 1;
		int data[19 * 2] = { 0 };
		//30日分のtime時をまとめて処理
		for (int count = 0; count < 30; count++) {
			if (aaa(year, month, date) == 0 || aaa(year, month, date) == 6) {
				//cout << "休日" << ttos(year, month, date) << endl;
				tomorrow(&year, &month, &date);
				continue;
			}
			//cout << range << ":";
			//cout << date << ":";
			//cout << count << ":" << endl;
			//マップ初期化
			for (int i = 0; i < map.size(); ++i) {
				for (int j = 0; j < map[i].size(); ++j) {
					map[i][j] = 0;
				}
			}

			filename = "opendatas/";
			filename += ttos(year, month, date);
			filename += ".csv";
			ifs.open(filename);	//センサID，yyyy/m/d, h:mm, in, out, in累計, out累計\n
			if (!ifs) {
				cout << "ファイルオープンに失敗" << filename << endl;
				tomorrow(&year, &month, &date);
				continue;
			}
			else {
				//cout << "ファイルオープンに成功" << filename << endl;
				//count++;
			}

			while (getline(ifs, line)) {//オープンデータ内のループ
				vector<string> strvec = split(line, ',');//データ一行ゲット
				if (strvec.at(2) == start) {//データの時間評価
					isTracking = true;
				}
				if (isTracking) {
					if (strvec.at(2) == end) {
						isTracking = false;
					}
				}
				if (isTracking) {
					//cout << "計測中" << endl;
						
					//cout << strvec.at(2) << endl;
					sensor[i].nAdd(stoi(strvec.at(3)), stoi(strvec.at(4)));

					if (i < 18) {
						i++;
					}
					else {//１周期終了，マップ更新
						//cout << "動作中！！" << endl;
						for (int id = 0; id < 19; id++) {
							for (int h = 0; h < height; h++) {
								for (int w = 0; w < width; w++) {//mapに書き込む
									map[h][w] += in[id][h][w] * sensor[id].GetnIn();
									map[h][w] += out[id][h][w] * sensor[id].GetnOut();
									data[id * 2 + 0] += in[id][h][w] * sensor[id].GetnIn();
									data[id * 2 + 1] += out[id][h][w] * sensor[id].GetnOut();
								}
							}
						}
						i = 0;
					}
				}
				else {
					//cout << strvec.at(2) << endl;
				}
			}
			ifs.close();

			tomorrow(&year, &month, &date);
		}//30日終わり

		//対象者について
		for (int i = 0; i < 19; i++) {
			for (int h = 0; h < height; h++) {
				for (int w = 0; w < width; w++) {
					sensor[i].pAdd(positive[h][w] * in[i][h][w] * posnum, positive[h][w] * out[i][h][w] * posnum);
				}
			}
		}

		filename = "results/add_positive/";
		filename += to_string(range + 1);
		filename += ".csv";
		ofs.open(filename, ios::trunc);
		//ofs.setf(ios_base::fixed, ios_base::floatfield);
		for (int i = 0; i < 19; i++) {
			//ofs << i + 1 << "," << sensor[i].GetSumnIn()/30.0 << "," << sensor[i].GetSumnOut()/30.0;
			ofs << i + 1 << ",";
			ofs << fixed << setprecision(5) << (double)sensor[i].GetRatio() << endl;
		}
		ofs.close();

		cout << "対象者出力終わり" << endl;

		filename = "results/test.csv";
		ofs.open(filename, ios::trunc);		
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				int a = 0;
				for (int id = 0; id < 19; id++) {
					a += positive[h][w] * in[id][h][w] * posnum + positive[h][w] * out[id][h][w] * posnum;
				}
				ofs << a << ",";
			}
			ofs << endl;
		}
		ofs.close();

	}//range終わり

	//int decoi;
	//scanf_s("%d", &decoi);

	return 0;
}



//filename = "results/";
//filename += to_string(range + 1);
//filename += "/";
//filename += ttos(year, month, date);
//filename += ".csv";
//ofs.open(filename, ios::trunc);

//cout << "----------------------------" << endl;
//for (int i = 0; i < map.size(); ++i) {
//	//printf("%3d ", i);
//	for (int j = 0; j < map[i].size(); ++j) {
//		//printf("%5d", map[i][j]);
//		ofs << map[i][j] << ",";
//	}
//	//printf("\n");
//	ofs << endl;
//}
//ofs.close();

//filename = "results/ratio/";
////filename += to_string(range + 1);
////filename += "/";
//filename += ttos(year, month, date);
//filename += ".csv";
//ofs.open(filename, ios::trunc);
//for (int i = 0; i < 19; i++) {//センサのループ
//	//ofs << sensor[i].GetID() + 1 << "," << sensor[i].GetSumnIn() << "," << sensor[i].GetSumnOut();
//	//ofs << endl;
//}
//ofs.close();