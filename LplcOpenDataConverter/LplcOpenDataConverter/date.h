#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <string>

using namespace std;

//http://zhi-ma.hatenablog.com/entry/2013/10/11/204331

int isleap(int year);//yearが閏年であれば1をそうでなければ0を返す

void tomorrow(int *y, int *m, int *d);//受け取った日付の次の日を求める

string ttos(int y, int m, int d);//timeを受け取り，オープンデータに合わせてstringを返す

#endif // !DATE_H




