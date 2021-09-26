#include "trophy.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


int main(int argc, char **argv)
{

	ifstream fin;
	string file, line;
	string date, game, title, rest_of_title, gradeString;
	char comma;
	int grade;
	istringstream ss;
	vector<Trophy *> trophies;

	file = argv[1];

	fin.open(file.c_str());
	if (fin.fail()){
		cerr << "file could not be opened\n";
		return -1;
	}
	
	//int i = 1; 

	while(getline(fin, line)){

		ss.str(line);
		getline(ss, date, ',');
		getline(ss, game, ',');

		//The title might have commas in it
		//if it has double quotes read to the next double quotes
		getline(ss, title, ',');
		if(title[0] == '"'){
			getline(ss, rest_of_title, '"');
			ss.get(comma);
		}
		ss >> gradeString;
		if(     gradeString.compare("bronze") == 0   || gradeString.compare("Bronze") == 0)   grade = 0;
	       	else if(gradeString.compare("silver") == 0   || gradeString.compare("Silver") == 0)   grade = 1;	
	       	else if(gradeString.compare("gold") == 0     || gradeString.compare("Gold") == 0)     grade = 2;	
	       	else if(gradeString.compare("platinum") == 0 || gradeString.compare("Platinum") == 0) grade = 3;
		else{
			cerr << "incorrect grade for trophy: " << title << endl;
			return -1;
		}	

		trophies.push_back(new Trophy(date, game, title, grade));
		
		//return 0;
		//cout << i << ": "<< line << endl;
		//i++;
	}

	for(size_t i = 0; i < trophies.size(); i++){
		trophies[i]->Print();
		cout << endl;
	}
	
	cout << "thats the end\n";

	//Trophy t();

	cout << "it worked\n";

	return 0;
}
