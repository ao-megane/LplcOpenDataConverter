#include"date.h"

int day[2][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
};

/*--- yearが閏年であれば1をそうでなければ0を返す ---*/
int isleap(int year){
	return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}

/*--- 受け取った日付の次の日を求める ---*/
void tomorrow(int *y, int *m, int *d){
	if (*d != day[isleap(*y)][*m]) // 最終日でなければ*dをインクリメント
		(*d)++;
	else {
		if (*m != 12)   // 12月以外の最終日
			(*m)++;
		else {      // 12月31日
			(*y)++;
			*m = 1;
		}
		*d = 1;
	}
}

string ttos(int y, int m, int d) {//timeを受け取り，オープンデータに合わせてstringを返す
	string result = "";
	result += to_string(y);
	if (m < 10) {//一桁なら
		result += "0";
		result += to_string(m);
	}
	else {
		result += to_string(m);
	}
	if (d < 10) {//一桁なら
		result += "0";
		result += to_string(d);
	}
	else {
		result += to_string(d);
	}

	return result;
}

//曜日を求める(ツェラーの公式)(http://www.aoharu-b.com/cgi/sk/2008/03/zellerc.html)
