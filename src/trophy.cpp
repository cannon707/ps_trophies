#include "trophy.hpp"
#include <string>
#include <iostream>
#include <cstdio>
#include <sstream>
using namespace std;

//constructor for trophy data
Trophy::Trophy(string &date, const string &game, const string &title, const int &grade)
{
	istringstream ss;	
	char junk;
	int day, month, year, hour, min;

	//convert string of data into struct tm object
	ss.str(date);
	ss >> month >> junk >> day >> junk >> year >> hour >> junk >> min;

	this->time_earned.tm_mon = month;
	this->time_earned.tm_mday = day;	
	this->time_earned.tm_year = year - 1900;
	this->time_earned.tm_hour = hour;
	this->time_earned.tm_min = min;

	//store title of game, trophy, and grade of trophy
	this->game = game;
	this->title = title;
	this->grade = grade;
}

/* used in testing prints out info on trophy */
void Trophy::Print()
{
	printf("Trophy Title: %s\nGame Title: %s\n", this->title.c_str(), this->game.c_str());
	switch(this->grade){
		case 0:
			printf("Grade: Bronze\n");
			break;
		case 1:
			printf("Grade: Silver\n");
			break;
		case 2:
			printf("Grade: Gold\n");
			break;
		case 3:
			printf("Grade: Platinum\n");
			break;
		default:
			printf("SOMETHINGS WRONG\n");
			break;
	}
	printf("Date: %d/%d/%d\n", this->time_earned.tm_mon, this->time_earned.tm_mday, this->time_earned.tm_year);
	printf("Time: %d:%d\n", this->time_earned.tm_hour, this->time_earned.tm_min);
}
