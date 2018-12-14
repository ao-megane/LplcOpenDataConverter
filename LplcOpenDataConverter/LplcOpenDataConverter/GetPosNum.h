#ifndef GETPOSNUM_H
#define GETPOSNUM_H

#include <iostream>
#include <time.h>


int SetRand() {
	srand((unsigned int)time(NULL));
	return 0;
}
//0<= GetRand() < 100
int GetRand() {
	return rand() % 100;
}

int GetPosNum() {//1ƒx[ƒX‚ð•Ô‚·
	int a = GetRand();
	if (a < 5) {
		return 6;
	}
	else if (a < 5 + 21) {
		return 9;
	}
	else if (a < 5 + 21 + 24) {
		return 10;
	}
	else if (a < 5 + 21 + 24 + 13) {
		return 12;
	}
	else if (a < 5 + 21 + 24 + 13 + 37) {
		return 13;
	}
	else {
		cout << "RAND ERROR!" << endl;
	}
	return -1;
}

#endif // !GETPOSNUM_H
