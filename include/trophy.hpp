#pragma once
#include <string>
#include <map>
#include <time.h>

class Trophy {
	public:
		string game;
		string title;
		struct tm* time;
		int grade; //0 = bronze, 1=silver, 2=gold, 3=platinum

		Trophy();
		~Trophy();
};

map <string, Trophy *> Tophies;


