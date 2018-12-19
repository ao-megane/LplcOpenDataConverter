#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <iomanip>
#include "Sensor.h"
#include "date.h"
#include "GetPosNum.h"

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
	//int posnum = 0;//対象者人数
	vector<vector<int>> map;
	vector<vector<int>> decoi;
	vector<vector<int>> positive[19];
	vector<vector<int>> in[19];
	vector<vector<int>> out[19];

	Sensor sensor[19];//格納データの内容を扱うクラス
	double ratio[11];

	int workdaynum = 21;

	bool isTracking = false;

	SetRand();

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
	/*if (getline(ifs, line)) {
		vector<string> strvec = split(line, ',');
		posnum = stoi(strvec.at(0));
	}
	*/
	ifs.close();
	width = mapdata[0];
	height = mapdata[1];//マップの広さを格納

	map.resize(height);
	decoi.resize(height);
	for (int i = 0; i < 19; i++) {
		positive[i].resize(height);
	}
	for (int i = 0; i < 19; i++) {//大きさ格納
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
		}
	}
	for (int i = 0; i < height; i++) {
		map[i].resize(width);
		decoi[i].resize(width);
		for (int num = 0; num < 19; num++) {
			positive[num][i].resize(width);
		}
	}

	//posファイル読み込み&格納
	for (int i = 0; i < 19; i++) {
		filename = "mapdatas/allmap/pos/";
		filename += to_string(i+1);
		filename += ".csv";
		cout << filename << endl;
		ifs.open(filename.c_str());
		if (!ifs) {
			cout << "ファイルオープンに失敗" << filename << endl;
			for (int h = 0; h < height; h++) {
				for (int w = 0; w < width; w++) {
					positive[i][h][w] = 0;
				}
			}
			continue;
		}
		else {
			int j = 0, k = 0;
			while (getline(ifs, line)) {
				vector<string> strvec = split(line, ',');
				for (k = 0; k < strvec.size(); k++) {
					if (strvec.at(k) == "") positive[i][j][k] = 0;
					else positive[i][j][k] = stoi(strvec.at(k));
				}
				j++;
			}
		}
		ifs.close();
		//cout << i << endl;
	}
	

	//センサ有効範囲についてのforループ
	int range = 0;
	for (int t = 0; t < 1; t++) {
		/*switch (t)
		{
		case 0:
			range = 0;
			break;
		case 1:
			range = 5;
			break;
		case 2:
			range = 11;
			break;
		default:
			break;
		}*/
		//各センサに有効範囲を格納(センサについてのforループ)
		for (int i = 0; i < 19; i++) {
			filename = "mapdatas/allmap/";
			filename += to_string(range + 1);
			filename += "/";
			filename += to_string(i + 1);
			filename += "in.csv";

			//cout << filename << endl;
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
			//cout << filename << endl;
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
				cout << "休日(カウントしない)" << ttos(year, month, date) << endl;
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
			i = 0;
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
					//センサ更新
					for (int h = 0; h < height; h++) {
						for (int w = 0; w < width; w++) {
							sensor[i].nAdd(stoi(strvec.at(3)) * in[i][h][w], stoi(strvec.at(4))*out[i][h][w]);
						}
					}

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
		//for (int posnum = 0; posnum <= 100; posnum += 10) {
		//	int num;
		//	for (int i = 0; i < 19; i++) {
		//		sensor[i].pInitialize();
		//	}
		//	for (int i = 0; i < 30; i++) {//30倍回して
		//		for (int t = 0; t < posnum*workdaynum; t++) {//人数
		//			num = GetPosNum();
		//			for (int id = 0; id < 19; id++) {//センサ
		//				for (int h = 0; h < height; h++) {
		//					for (int w = 0; w < width; w++) {
		//						sensor[id].pAdd(positive[num - 1][h][w] * in[id][h][w], positive[num - 1][h][w] * out[id][h][w]);
		//					}
		//				}
		//			}
		//		}
		//	}
		//	for (int id = 0; id < 19; id++) {//平均にする
		//		sensor[id].pDivision(30.0);
		//	}
		//	int posdata = 0;
		//	int negdata = 0;
		//	for (int id = 0; id < 19; id++) {
		//		negdata += sensor[id].GetSumnIn() + sensor[id].GetSumnOut();
		//		posdata += sensor[id].GetSumpIn() + sensor[id].GetSumpOut();
		//	}
		//	ratio[posnum / 10] = (double)(negdata) / (negdata + posdata);

		//	cout << posnum<< ":対象者出力終わり" << endl;
		//}

		//デバッグ
		//int posnum = 200;
		//int num;
		//for (int i = 0; i < 19; i++) {
		//	sensor[i].pInitialize();
		//}
		//for (int i = 0; i < 30; i++) {//30倍回して
		//	for (int t = 0; t < posnum; t++) {//人数
		//		num = GetPosNum();
		//		for (int id = 0; id < 19; id++) {
		//			for (int h = 0; h < height; h++) {
		//				for (int w = 0; w < width; w++) {
		//					if (id != 9 && id != 11) {
		//						map[h][w] += positive[num - 1][h][w] * in[id][h][w];
		//					}
		//					if (id != 7 && id != 13) {
		//						map[h][w] += positive[num - 1][h][w] * out[id][h][w];
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
		//for (int h = 0; h < height; h++) {
		//	for (int w = 0; w < width; w++) {
		//		map[h][w] /= 30.0;
		//	}
		//}
	}//range終わり

	filename = "results/add_positive/result";
	//filename += to_string(result);
	filename += ".csv";
	ofs.open(filename, ios::trunc);
	//ofs.setf(ios_base::fixed, ios_base::floatfield);
	/*for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			ofs << map[h][w] << ",";
		}
		ofs << endl;
	}*/
	ofs << "2018/6平日7～8時の合計" << endl;
	for (int i = 0; i < 19; i++) {
		ofs << i+1 << "," << sensor[i].GetnOut() + sensor[i].GetnIn() << endl;
	}
	ofs.close();

	//デバッグ用
	//int num;
	//for (int i = 0; i < posnum; i++) {
	//	num = GetPosNum();
	//	//テスト用
	//	for (int h = 0; h < height; h++) {
	//		for (int w = 0; w < width; w++) {
	//			decoi[h][w] += positive[num-1][h][w];
	//		}
	//	}
	//	cout << num << endl;
	//}

	//filename = "results/test.csv";
	//ofs.open(filename, ios::trunc);
	//for (int h = 0; h < height; h++) {
	//	for (int w = 0; w < width; w++) {
	//		ofs << decoi[h][w] << ",";
	//	}
	//	ofs << endl;
	//}
	//ofs.close();

	int a;
	cout << "終わった";
	scanf_s("%d", &a);

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