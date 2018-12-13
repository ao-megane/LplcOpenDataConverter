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

vector<string> split(string& input, char delimiter){//delimiter��input�𕪊����Cvector<string>�ɂ܂Ƃ߂ĕԂ�
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
	string start;	//�v���J�n����	//hh:mm�Ƃ����`���Ȃ̂Œ���
	string end;		//�v���I������
	string filename;	//���т��ѕς��t�@�C���l�[���C�g���܂킵�܂���̂ŏ�������Y��Ȃ��悤��
	int year;
	int month;
	int date;
	int i = 0;
	int width = 0;
	int height = 0;
	int posnum = 0;//���ۂ�posnum*3�l�������Ă�
	vector<vector<int>> map;
	vector<vector<int>> positive[15];
	vector<vector<int>> in[19];
	vector<vector<int>> out[19];

	Sensor sensor[19];//�i�[�f�[�^�̓��e�������N���X

	bool isTracking = false;

	SetRand();

	ifstream ifs;
	ofstream ofs;
	ifs.open("mapdatas/allmap/map.txt");	//�}�b�v�̍L���Ƃ�
	if (!ifs) {
		cout << "�t�@�C���I�[�v���Ɏ��s" << "mapdatas/map.txt" << endl;
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
	height = mapdata[1];//�}�b�v�̍L�����i�[

	map.resize(height);
	for (int i = 0; i < 15; i++) {
		positive[i].resize(height);
	}
	for (int i = 0; i < 19; i++) {//�傫���i�[
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
		}
	}
	for (int i = 0; i < height; i++) {
		map[i].resize(width);
		for (int num = 0; num < 15; num++) {
			positive[num][i].resize(width);
		}
	}

	//pos�t�@�C���ǂݍ���&�i�[
	for (int i = 0; i < 15; i++) {
		int num = 0;
		switch (i)
		{
		case 0:
			num = 1;
			break;
		case 1:
			num = 2;
			break;
		case 2:
			num = 3;
			break;
		case 3:
			num = 4;
			break;
		case 4:
			num = 5;
			break;
		case 5:
			num = 6;
			break;
		case 6:
			num = 9;
			break;
		case 7:
			num = 10;
			break;
		case 8:
			num = 12;
			break;
		case 9:
			num = 13;
			break;
		case 10:
			num = 15;
			break;
		case 11:
			num = 16;
			break;
		case 12:
			num = 17;
			break;
		case 13:
			num = 18;
			break;
		case 14:
			num = 19;
			break;
		default:
			break;
		}
		filename = "mapdatas/allmap/pos/";
		filename += to_string(num);
		filename += ".csv";
		//cout << filename << endl;
		ifs.open(filename.c_str());
		if (!ifs) {
			cout << "�t�@�C���I�[�v���Ɏ��s" << filename << endl;
		}
		int j = 0, k = 0;
		while (getline(ifs, line)) {
			vector<string> strvec = split(line, ',');
			for (k = 0; k < strvec.size(); k++) {
				if (strvec.at(k) == "") positive[i][j][k] = 0;
				else positive[i][j][k] = stoi(strvec.at(k));
			}
			j++;
		}
		ifs.close();
	}

	//�Z���T�L���͈͂ɂ��Ă�for���[�v
	for (int range = 0; range < 12; range++) {
		//�e�Z���T�ɗL���͈͂��i�[(�Z���T�ɂ��Ă�for���[�v)
		for (int i = 0; i < 19; i++) {
			filename = "mapdatas/allmap/";
			filename += to_string(range + 1);
			filename += "/";
			filename += to_string(i + 1);
			filename += "in.csv";

			//cout << filename << endl;
			ifs.open(filename.c_str());
			if (!ifs) {
				cout << "�t�@�C���I�[�v���Ɏ��s" << filename << endl;
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
				cout << "�t�@�C���I�[�v���Ɏ��s" << filename << endl;
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

		//�Z���T������
		for (int i = 0; i < 19; i++) {
			sensor[i].Initialize();
		}

		//�}�b�v������
		for (int i = 0; i < map.size(); ++i) {
			for (int j = 0; j < map[i].size(); ++j) {
				map[i][j] = 0;
			}
		}
		cout << "----------------------------" << endl;

		//���t������
		year = 2018;
		month = 6;
		date = 1;
		int data[19 * 2] = { 0 };
		//30������time�����܂Ƃ߂ď���
		for (int count = 0; count < 30; count++) {
			if (aaa(year, month, date) == 0 || aaa(year, month, date) == 6) {
				//cout << "�x��" << ttos(year, month, date) << endl;
				tomorrow(&year, &month, &date);
				continue;
			}
			//cout << range << ":";
			//cout << date << ":";
			//cout << count << ":" << endl;
			//�}�b�v������
			for (int i = 0; i < map.size(); ++i) {
				for (int j = 0; j < map[i].size(); ++j) {
					map[i][j] = 0;
				}
			}

			filename = "opendatas/";
			filename += ttos(year, month, date);
			filename += ".csv";
			ifs.open(filename);	//�Z���TID�Cyyyy/m/d, h:mm, in, out, in�݌v, out�݌v\n
			if (!ifs) {
				cout << "�t�@�C���I�[�v���Ɏ��s" << filename << endl;
				tomorrow(&year, &month, &date);
				continue;
			}
			else {
				//cout << "�t�@�C���I�[�v���ɐ���" << filename << endl;
				//count++;
			}

			while (getline(ifs, line)) {//�I�[�v���f�[�^���̃��[�v
				vector<string> strvec = split(line, ',');//�f�[�^��s�Q�b�g
				if (strvec.at(2) == start) {//�f�[�^�̎��ԕ]��
					isTracking = true;
				}
				if (isTracking) {
					if (strvec.at(2) == end) {
						isTracking = false;
					}
				}
				if (isTracking) {
					//cout << "�v����" << endl;
						
					//cout << strvec.at(2) << endl;
					sensor[i].nAdd(stoi(strvec.at(3)), stoi(strvec.at(4)));

					if (i < 18) {
						i++;
					}
					else {//�P�����I���C�}�b�v�X�V
						//cout << "���쒆�I�I" << endl;
						for (int id = 0; id < 19; id++) {
							for (int h = 0; h < height; h++) {
								for (int w = 0; w < width; w++) {//map�ɏ�������
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
		}//30���I���

		//�Ώێ҂ɂ���
		int num;
		for (int t = 0; t < posnum; t++) {//�l��
			num = GetPosNum();
			for (int id = 0; id < 19; id++) {//�Z���T
				for (int h = 0; h < height; h++) {
					for (int w = 0; w < width; w++) {
						sensor[id].pAdd(positive[num][h][w] * in[id][h][w] * posnum, positive[num][h][w] * out[id][h][w] * posnum);
					}
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

		cout << range+1 << ":�Ώێҏo�͏I���" << endl;

		/*filename = "results/test.csv";
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
		ofs.close();*/

	}//range�I���

	int decoi;
	scanf_s("%d", &decoi);

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
//for (int i = 0; i < 19; i++) {//�Z���T�̃��[�v
//	//ofs << sensor[i].GetID() + 1 << "," << sensor[i].GetSumnIn() << "," << sensor[i].GetSumnOut();
//	//ofs << endl;
//}
//ofs.close();