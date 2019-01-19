//============================================================================
// Name        : AdvancedProgramming.cpp
// Author      : Guru Gulundu
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>

const int MAX_CHAR = 256;

using namespace std;

bool uniqueSubstring (string str);
int longestUniqueString (string str);

int main(int argc, char **argv) {

	if (argc <= 1) {
		cout << "Usage: \nchal01 <string>" << endl;
		return 0;
	} else {
		string str = argv[1];

		cout << longestUniqueString(str) << endl;
	}


	return 0;
}

bool uniqueSubstring (string str){
	if (str.length() > MAX_CHAR){
		return false;
	}
	bool characters[MAX_CHAR] = {0};
	for (int i = 0; i < str.length(); ++i){
		if (characters[(int)(str[i])] == true){
			return false;
		}
		characters[int(str[i])] = true;
	}
	return true;
}

int longestUniqueString (string str){
	int overall_max = 0;
	int current_max = 0;
	for(int i = 0; i < str.length()-1; ++i){
		for(int j = 1; j < str.length()-i; ++j){
			if (uniqueSubstring(str.substr(i,j))){
				++current_max;
			}
		}
		if(current_max > overall_max){
			overall_max = current_max;
		}
		current_max = 0;
	}
	return overall_max;
}

