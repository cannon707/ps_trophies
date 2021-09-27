#pragma once
#include <string>
#include <time.h>

/* Holds data of playstation trophy */
class Trophy {
	public:
		std::string game;       //holds title of game 
		std::string title;      //holds title of trophy 
		struct tm time_earned;  //holds time of trophy gained
		int grade;              //0 = bronze, 1=silver, 2=gold, 3=platinum

		//trophy constructor
		Trophy(std::string &date, const std::string &game, const std::string &title, const int &grade);

		//used in testing
		//prints out trophy data
		void Print();
};
