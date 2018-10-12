#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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
	//std::cout << "Hello World\n" << std::endl;
	ifstream ifs("opendatas/20180501.csv");
	//�Z���TID�Cyyyy/m/d,h:mm,in,out,in�݌v,out�݌v\n
	string line;
	while (getline(ifs, line)) {

		vector<string> strvec = split(line, ',');

		for (int i = 0; i<strvec.size(); i++) {
			printf("%d\n", stoi(strvec.at(i)));
		}

	}

	return 0;
}