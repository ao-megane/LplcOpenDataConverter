#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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
	string line;
	while (getline(ifs, line)) {

		vector<string> strvec = split(line, ',');

		for (int i = 0; i<strvec.size(); i++) {
			printf("%d\n", stoi(strvec.at(i)));
		}

	}

	return 0;
}