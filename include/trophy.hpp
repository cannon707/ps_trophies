#pragma once
#include <string>
#include <time.h>

class Trophy {
	public:
		std::string game;
		std::string title;
		struct tm time_earned;
		//int day, month, year, hour, min;
		int grade; //0 = bronze, 1=silver, 2=gold, 3=platinum

		Trophy(std::string &date, const std::string &game, const std::string &title, const int &grade);
		~Trophy();
		void Print();
};
