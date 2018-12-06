#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include "Sensor.h"
#include "date.h"

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
	vector<vector<int>> map;
	vector<vector<int>> in[19];
	vector<vector<int>> out[19];

	bool isTracking = false;

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
	ifs.close();
	width = mapdata[0];
	height = mapdata[1];//�}�b�v�̍L�����i�[

	map.resize(height);
	for (int i = 0; i < 19; i++) {//�傫���i�[
		in[i].resize(height);
		out[i].resize(height);
		for (int j = 0; j < height; j++) {
			in[i][j].resize(width);
			out[i][j].resize(width);
			map[j].resize(width);
		}
	}

	for (int range = 0; range < 12; range++) {//�Z���T�L���͈͂ɂ��Ă�for���[�v
		for (int i = 0; i < 19; i++) {//�e�Z���T�ɗL���͈͂��i�[(�Z���T�ɂ��Ă�for���[�v)
			filename = "mapdatas/allmap/";
			filename += to_string(range + 1);
			filename += "/";
			filename += to_string(i + 1);
			filename += "in.csv";

			cout << filename << endl;
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
			cout << filename << endl;
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

		for (int i = 0; i < map.size(); ++i) {//�O�̂��߂̏�����
			for (int j = 0; j < map[i].size(); ++j) {
				map[i][j] = 0;
			}
		}
		cout << "----------------------------" << endl;

		i = 0;
		//���t������
		year = 2016;
		month = 1;
		date = 1;

		while(year < 2017) {//���t(�I�[�v���f�[�^)�ɂ��Ẵ��[�v
		//for(int count=0;count<24;){
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
				cout << "�t�@�C���I�[�v���ɐ���" << filename << endl;
				//count++;
			}
			Sensor sensor[19];//�i�[�f�[�^�̓��e�������N���X
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
					//cout << "�v����:";
					//cout << strvec.at(2) << endl;
					sensor[i].Add(stoi(strvec.at(3)), stoi(strvec.at(4)));

					if (i < 18) {
						i++;
					}
					else {//�P�����I���C�}�b�v�X�V
						//cout << "���쒆�I�I" << endl;
						for (int id = 0; id < 19; id++) {
							for (int h = 0; h < height; h++) {
								for (int w = 0; w < width; w++) {
									map[h][w] += in[id][h][w] * sensor[id].GetIn();
									map[h][w] += out[id][h][w] * sensor[id].GetOut();
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

			
			filename = "results/";
			filename += to_string(range + 1);
			filename += "/";
			filename += ttos(year, month, date);
			filename += ".csv";
			ofs.open(filename, ios::trunc);

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
			ofs.close();

			//map������
			for (int i = 0; i < map.size(); ++i) {
				for (int j = 0; j < map[i].size(); ++j) {
					map[i][j] = 0;
				}
			}
			tomorrow(&year, &month, &date);
		}
	}

	return 0;
}