#include "trophy.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void LineMonthsAll(vector<Trophy *> &t, ofstream &ofs){

	string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
	int total_years, total_months;
	int first_year, last_year;
	int first_month, last_month;
	int max = 0;
	int hash;
	size_t i, j, k;

	first_year = t[0]->time_earned.tm_year;
	last_year = t.back()->time_earned.tm_year;
	total_years = last_year - first_year;
	first_month = t[0]->time_earned.tm_mon;
	last_month = t.back()->time_earned.tm_mon;
	if(total_years >= 2){
		total_months = (total_years - 1) * 12;
		total_months += 13 - t[0]->time_earned.tm_mon;
		total_months += t.back()->time_earned.tm_mon;
	}
	else if(total_years == 1){
		total_months = 0; 
		total_months += 13 - t[0]->time_earned.tm_mon;
		total_months += t.back()->time_earned.tm_mon;
	}
	else if(total_years == 0){
		total_months = 0;
		total_months = t.back()->time_earned.tm_mon - t[0]->time_earned.tm_mon + 1;
	}
	if(total_months < 1) return;

	vector< vector<int> > points(total_years+1, vector<int> (12, 0));

	for(i = 0; i < t.size(); i++){
		points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] += 1;
		if(points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] >= max){
			max = points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1];
		}
	}
	max = ((max + 10 - 1)/10)*10;
	cout << "max: " << max << endl;
	hash = 20;

	ofs << "newgraph\n\n\n";
	ofs << "xaxis size 5\n";
	ofs << "  min 0.2 max " << total_months + 0.2 << " hash 6 mhash 5 shash 0\n";
	ofs << "  no_auto_hash_labels\n";

	//first_month =  t[0]->time_earned.tm_mon;
	k = first_year - 100;
	for(i =0; i < (size_t)total_months; i += 6){
		j = (first_month - 1 + i)%12;
		ofs << "  hash_label at " << i + 1 << " : " << months[j] << " " << k << "\n";
		if(j > 5) k++;
	}
	ofs << "  hash_labels fontsize 8 font Times-Italic hjl vjc rotate -60\n";
	ofs << "  label : Month and Year\n\n\n";
	ofs << "yaxis min 0 max " << max << " hash " << hash << " mhash 3"<<" size 1.5\n";
	ofs << "  label : Trophies Earned\n";
	ofs << "  grid_lines grid_gray .7\n\n\n";
	ofs << "newline pts 0.8 0 " << total_months + 0.2 << " 0\n\n\n";
	ofs << "newcurve marktype none linetype solid label : Trophies per month\n";

	k = 0;
	for(i = 0; i < points.size(); i++){
		ofs << "   pts ";
		if(i == 0){
			for(j = first_month - 1; j < points[i].size(); j++){
				ofs << k << " " << points[i][j] << "  ";
				k++;
			}
		
		}
		else if(i == points.size() -1){
			for(j = 0; j < ((size_t)last_month -1); j++){
				ofs << k << " " << points[i][j] << "  ";
				k++;
			}
			
		} else { 
			for(j = 0; j < points[i].size(); j++){
				ofs << k << " " << points[i][j] << "  ";
				k++;
			}
		}
		ofs << "\n";
	}		
	return;


}

void BarMonthsAll(vector<Trophy *> &t, ofstream &ofs){

	string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
	vector<int> total(12, 0);
	int total_years, total_months;
	int first_year, last_year;
	int max = 0;
	int hash;
	size_t i, j;
	int k;

	float colors[30][3] = 
	{		
		{0.733, 0.262, 0.949},
		{0.196, 0.121, 0.498},
		{0.254, 0.474, 0.709},
		{1.0, 0.843, 0.156},
		{0.349, 0.670, 0.858},
		{0.886, 0.301, 0.733},
		{0.819, 0.819, 0.031},
		{0.988, 0.403, 0.678},
		{0.768, 0.168, 0.196},
		{0.019, 0.658, 0.133},
		{0.149, 0.788, 0.576},
		{0.164, 0.654, 0.917},
		{0.749, 0.537, 0.149},
		{0.776, 0.294, 0.439},
		{0.392, 0.898, 0.278},
		{0.239, 0.780, 0.827},
		{0.886, 0.231, 0.713},
		{0.396, 0.949, 0.701},
		{0.937, 0.043, 0.788},
		{0.352, 0.988, 0.658},
		{0.219, 0.819, 0.258},
		{0.388, 0.192, 0.698},
		{0.737, 0.094, 0.266},
		{0.231, 0.886, 0.690},
		{0.858, 0.639, 0.160},
		{0.086, 0.368, 0.886},
		{0.639, 0.788, 0.290},
		{0.603, 0.423, 0.988},
		{0.858, 0.376, 0.615},
		{0.137, 0.549, 0.0}
	};

	first_year = t[0]->time_earned.tm_year;
	last_year = t.back()->time_earned.tm_year;
	total_years = last_year - first_year;
	if(total_years >= 2){
		total_months = (total_years - 2) * 12;
		total_months += 13 - t[0]->time_earned.tm_mon;
		total_months += t.back()->time_earned.tm_mon;
	}
	else if(total_years == 1){
		total_months = 0; 
		total_months += 13 - t[0]->time_earned.tm_mon;
		total_months += t.back()->time_earned.tm_mon;
	}
	else if(total_years == 0){
		total_months = 0;
		total_months = t.back()->time_earned.tm_mon - t[0]->time_earned.tm_mon + 1;
	}
	if(total_years > 30){
		cout << "That's too many years right now. I don't have enough colors\n";
		return;
	}

	if(total_months < 1) return;
	vector< vector<int> > bars(total_years+1, vector<int> (12, 0));

	for(i = 0; i < t.size(); i++){
		bars[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] += 1;
		total[t[i]->time_earned.tm_mon - 1] += 1;
	}

	for(i= 0; i < total.size(); i++){
		if(total[i] >= max) max = total[i];
	}

	max = ((max + 100 - 1)/100)*100;
	hash = max / 20;

	ofs << "newgraph\n\n\n";
	ofs << "xaxis size 5\n";
	ofs << "  min 0.1 max 12.9 hash 1 mhash 0 shash 0\n";
	ofs << "  no_auto_hash_labels\n";
	for(i=0; i < 12; i++){
		ofs << "  hash_label at " << i+1 << " : " << months[i] << "\n";
	}
	ofs << "  label : Months\n\n\n";
	ofs << "yaxis min 0 max " << max << " hash " << hash << " mhash 3"<<" size 5\n";
	ofs << "  label : Trophies Earned\n";
	ofs << "  grid_lines grid_gray .7\n\n\n";
	ofs << "newline pts 0.1 0 12.9 0\n\n\n";
	for(k = bars.size() -1; k >= 0; k--){
		ofs << "newcurve marktype xbar cfill ";
		cout << colors[k][0] << " " << colors[k][1] << " " << colors[k][2] << "\n\n";
		ofs << colors[k][0] << " " << colors[k][1] << " " << colors[k][2] << "\n\n";
		ofs << "  marksize .8 5\n\n";
		ofs << "  label : " << 1900 + first_year + k << "\n";
		ofs << "  pts\n";
		for(j = 0; j < bars[k].size(); j++){
			ofs << "   " << j + 1 << " " << total[j] << "\n";
			total[j] -= bars[k][j];
		}
	}
	return;
}

int main(int argc, char **argv)
{

	ifstream fin;
	ofstream ofs;
	string file, line;
	string date, game, title, rest_of_title, gradeString;
	char comma;
	int grade;
	istringstream ss;
	vector<Trophy *> trophies;

	if(argc < 2){
		cerr << "fix this later ARGUMENT ERROR\n";
		return -1;
	}

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

	ofs.open(argv[2]);
	if(ofs.is_open()){
		LineMonthsAll(trophies, ofs); 
	} else{
		cerr << "Can't open output file\n";
		return -1;
	}
	/*
	   for(size_t i = 0; i < trophies.size(); i++){
	   trophies[i]->Print();
	   cout << endl;
	   }
	   */

	return 0;
}
