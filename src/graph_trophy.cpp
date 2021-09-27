/* Hunter Kitts
 * hkitts2 cs494 jgraph
 * Takes in trophy data in csv form and prints out pdf of graph
 */

#include "trophy.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

/* Outputs stacked bar graph of trophies earned at each hour
 * bars stacked by grade
 */
void TimeByGrade(vector<Trophy *> &t, ofstream &ofs){

	vector< vector<int> > bars(24, vector<int> (4, 0)); //holds vector of trophy grades at each hour
	vector<int> total(24, 0);                           //total amount of trophies at each hour
	size_t i;					
	int max = 0;		//max size of y-axis
	int hash;		//hash size of y-axis

	//fill bars, total, and max with trophy data
	for(i = 0; i < t.size(); i++){
		bars[t[i]->time_earned.tm_hour][t[i]->grade] += 1;
		
		total[t[i]->time_earned.tm_hour] += 1;
		if(total[t[i]->time_earned.tm_hour] >= max){
			max = total[t[i]->time_earned.tm_hour];
		}
	}

	//calculate max 
	max = ((max + 10 - 1)/10)*10;
	hash = 100;

	//start output of jgraph
	ofs << "newgraph\n\n\n";
	ofs << "xaxis size 5\n";
	ofs << "  min 0.1 max 24.9 hash 1 mhash 0 shash 0\n";
	ofs << "  no_auto_hash_labels\n";

	//set 12 hour labels 
	for(i = 0; i < 12; i++){
		ofs << "  hash_label at " << i+1 << " : ";
		if(i == 0){
			ofs << "12 AM\n";
		} else{
			ofs << i;
			if(i < 10) ofs << "   AM\n";
			else ofs << " AM\n";
		}
	}
	for(i = 0; i < 12; i++){
		ofs << "  hash_label at " << i+13 << " : ";
		if(i == 0){
			ofs << "12  PM\n";
		} else{
			ofs << i;
			if(i < 10) ofs << "    PM\n";
			else ofs << "  PM\n";
		}

	}
	ofs << "  hash_labels fontsize 8 font Times hjl vjc rotate -60\n";
	ofs << "  label : Time of Day\n";
	ofs << "yaxis min 0 max " << max << " hash " << hash << " mhash 1"<<" size 1.5\n";
	ofs << "  label : Trophies Earned\n";
	ofs << "  grid_lines grid_gray .7\n\n\n";
	ofs << "newline pts 0.1 0 24.9 0\n";

	//each grade has a different color and loop
	//bars[i][0] is bronze
	//to make it a stacked bar graph the highest bar will print first
	//its size is all the trophies added together
	//then that grade at that hour is removed from the total
	//so the next grade can print
	ofs << "newcurve marktype xbar cfill 0.745 0.415 0.227\n";
	ofs << "  marksize .8 25\n";
	ofs << "  label : Bronze\n";
	ofs << "  pts\n";
	for(i = 0; i < bars.size(); i++){
		ofs << "   " << i+1 << " " << total[i] << "\n";
		total[i] -= bars[i][0];
	}
	//bars[i][1] is silver
	ofs << "newcurve marktype xbar cfill 0.839 0.839 0.839\n";
	ofs << "  marksize .8 25\n";
	ofs << "  label : Silver\n";
	ofs << "  pts\n";
	for(i = 0; i < bars.size(); i++){
		ofs << "   " << i+1 << " " << total[i] << "\n";
		total[i] -= bars[i][1];
	}
	//bars[i][2] is gold
	ofs << "newcurve marktype xbar cfill 0.803 0.603 0.274\n";
	ofs << "  marksize .8 25\n";
	ofs << "  label : Gold\n";
	ofs << "  pts\n";
	for(i = 0; i < bars.size(); i++){
		ofs << "   " << i+1 << " " << total[i] << "\n";
		total[i] -= bars[i][2];
	}
	//bars[i][3] is platinum
	ofs << "newcurve marktype xbar cfill 0.478 0.588 0.819\n";
	ofs << "  marksize .8 25\n";
	ofs << "  label : Platinum\n";
	ofs << "  pts\n";
	for(i = 0; i < bars.size(); i++){
		ofs << "   " << i+1 << " " << total[i] << "\n";
		total[i] -= bars[i][3];
	}
	return;

}

/* Line graph of all trophies gained by month */
void LineMonthsAll(vector<Trophy *> &t, ofstream &ofs){

	//used to print out months on the x-axis
	string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
	
	//holds when first and last trophies where gained and holds that to calculate total years and months
	int total_years, total_months;
	int first_year, last_year;
	int first_month, last_month;

	//used to calculate max y
	int max = 0;
	int hash;
	size_t i, j, k;

	//calculate first and last of years to calculate total
	first_year = t[0]->time_earned.tm_year;
	last_year = t.back()->time_earned.tm_year;
	total_years = last_year - first_year;

	//calculate first and last month
	first_month = t[0]->time_earned.tm_mon;
	last_month = t.back()->time_earned.tm_mon;

	//to calculate the total months
	//it may be possible to have trophies gained that does not
	//cover the entire year
	//to catch this we calcualte depeneding on the year difference
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

	//declared here so the total_years can be used
	//so it can be the set size we need
	//holds vectors of every year with each month having a total trophies gained
	vector< vector<int> > points(total_years+1, vector<int> (12, 0));

	//put the trophy data into the points 2D vector and calculate max 
	for(i = 0; i < t.size(); i++){
		points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] += 1;
		if(points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] >= max){
			max = points[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1];
		}
	}
	max = ((max + 10 - 1)/10)*10;
	hash = 20;

	//start of jgraph output
	ofs << "newgraph\n\n\n";
	ofs << "xaxis size 5\n";
	ofs << "  min 0.2 max " << total_months + 0.2 << " hash 6 mhash 5 shash 0\n";
	ofs << "  no_auto_hash_labels\n";

	//set the month and year of each the y-axis
	//split by every six months
	//miner hashes on every month
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

	//this is too combat the first and last months of trophies gained
	//it is possible the first trophy was gained in the middle of the year
	//so for the first year the for loop must be calcualted by with respect
	//to the first month
	//This is the same for the last year and last month
	k = 0;
	for(i = 0; i < points.size(); i++){
		ofs << "   pts ";
		//first year
		if(i == 0){
			for(j = first_month - 1; j < points[i].size(); j++){
				ofs << k << " " << points[i][j] << "  ";
				k++;
			}
		
		}
		//last year
		else if(i == points.size() -1){
			for(j = 0; j < ((size_t)last_month -1); j++){
				ofs << k << " " << points[i][j] << "  ";
				k++;
			}
		//every other year
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

/* prints out jgraph 
 * stacked bar graph
 * trophy data of all months
 * each color represents the year trophy was gained
 */
void BarMonthsAll(vector<Trophy *> &t, ofstream &ofs){
	
	//used to print out months on x-axis
	string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
	vector<int> total(12, 0);                    //holds total trohies for each month accross all years
	int first_year, last_year, total_years;	     //used to loop through trophies t
	int max = 0;
	int hash;
	size_t i, j;
	int k;

	//playstation has not had trophies for 30 years so right now this is safe
	//Each color represents a year
	//The stacked bar graph will use the same index to colors as the year
	//So if there are 8 years of trophies the first color will be at index colors[7]
	float colors[30][3] = 
	{		
		{0.733, 0.262, 0.949}, {0.196, 0.121, 0.498}, {0.254, 0.474, 0.709}, {1.0, 0.843, 0.156},
		{0.349, 0.670, 0.858}, {0.886, 0.301, 0.733}, {0.819, 0.819, 0.031}, {0.988, 0.403, 0.678},
		{0.768, 0.168, 0.196}, {0.019, 0.658, 0.133}, {0.149, 0.788, 0.576}, {0.164, 0.654, 0.917},
		{0.749, 0.537, 0.149}, {0.776, 0.294, 0.439}, {0.392, 0.898, 0.278}, {0.239, 0.780, 0.827},
		{0.886, 0.231, 0.713}, {0.396, 0.949, 0.701}, {0.937, 0.043, 0.788}, {0.352, 0.988, 0.658},
		{0.219, 0.819, 0.258}, {0.388, 0.192, 0.698}, {0.737, 0.094, 0.266}, {0.231, 0.886, 0.690},
		{0.858, 0.639, 0.160}, {0.086, 0.368, 0.886}, {0.639, 0.788, 0.290}, {0.603, 0.423, 0.988},
		{0.858, 0.376, 0.615}, {0.137, 0.549, 0.0}
	};

	//calculate first, last, and total years
	first_year = t[0]->time_earned.tm_year;
	last_year = t.back()->time_earned.tm_year;
	total_years = last_year - first_year;
	if(total_years > 30){
		cout << "That's too many years right now. I don't have enough colors\n";
		return;
	}
	
	//holds a vector for every year of trophy data
	//the inner vector holds the trophies gained on that month for that year
	vector< vector<int> > bars(total_years+1, vector<int> (12, 0));

	//fill the bars 2D vector and the total amount of trophies for each month
	for(i = 0; i < t.size(); i++){
		bars[t[i]->time_earned.tm_year - first_year][t[i]->time_earned.tm_mon - 1] += 1;
		total[t[i]->time_earned.tm_mon - 1] += 1;
	}

	//calculate the max y value
	for(i= 0; i < total.size(); i++){
		if(total[i] >= max) max = total[i];
	}

	max = ((max + 100 - 1)/100)*100;
	hash = max / 20;

	//start plotting jgraph 
	ofs << "newgraph\n\n\n";
	ofs << "xaxis size 5\n";
	ofs << "  min 0.1 max 12.9 hash 1 mhash 0 shash 0\n";
	ofs << "  no_auto_hash_labels\n";
	
	//set hash labels to be each month
	for(i=0; i < 12; i++){
		ofs << "  hash_label at " << i+1 << " : " << months[i] << "\n";
	}
	ofs << "  label : Months\n\n\n";
	ofs << "yaxis min 0 max " << max << " hash " << hash << " mhash 3"<<" size 5\n";
	ofs << "  label : Trophies Earned\n";
	ofs << "  grid_lines grid_gray .7\n\n\n";
	ofs << "newline pts 0.1 0 12.9 0\n\n\n";

	//print out points and colors 
	//it prints out total trophies earned and subtracts from the total
	//at each month and year
	//the top color is the most recent year and the bottom is the first year
	for(k = bars.size() -1; k >= 0; k--){
		ofs << "newcurve marktype xbar cfill ";
		ofs << colors[k][0] << " " << colors[k][1] << " " << colors[k][2] << "\n\n";
		ofs << "  marksize .8 8\n\n";
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
	string file, line, ofile, fileJPG;
	string jgraph_call;                                   //makes system call to jgraph
	string date, game, title, rest_of_title, gradeString; //input from csv file
	char comma;					      //used to get rid of comma
	size_t pos;                                           //used to remove .jgr from argument line
	int grade;                                            //grade of trophy
	int option;					      //command line option
	size_t i;
	istringstream ss;
	vector<Trophy *> trophies;			      //holds all trophy data

	if(argc != 4){
		cerr << "Not enough arguments\n";
		cerr << "usage: graph_trophies [input.csv] [output.jgr] [graph choice]\n";
		cerr << "       graph choices: bar_graph_months\n";
		cerr << "                      line_graph_months\n";
		cerr << "                      bar_graph_time\n";
		return -1;
	}

	//get graph choice
	if(string(argv[3]) == "bar_graph_months") option = 0;
	else if(string(argv[3]) == "line_graph_months") option = 1;
	else if(string(argv[3]) == "bar_graph_time") option = 2;
	else{
		cerr << "Incorrect graph choice\n";
		cerr << "usage: graph_trophies [input.csv] [output.jgr] [graph choice]\n";
		cerr << "       graph choices: bar_graph_months\n";
		cerr << "                      line_graph_months\n";
		cerr << "                      bar_graph_time\n";
		return -1;
	}
	

	//try to open file csv file given
	file = argv[1];
	fin.open(file.c_str());
	if (fin.fail()){
		cerr << "file could not be opened\n";
		return -1;
	}

	//loop through the csv file
	while(getline(fin, line)){

		ss.str(line);
		getline(ss, date, ',');		//get the date trophy was earned
		getline(ss, game, ',');         //get the title of game

		//The title might have commas in it
		//if it has double quotes read to the next double quotes
		getline(ss, title, ',');
		if(title[0] == '"'){
			getline(ss, rest_of_title, '"');
			ss.get(comma);
		}

		//get the grade of the trophy and store it in an integer for quick comparison
		ss >> gradeString;
		if(     gradeString.compare("bronze") == 0   || gradeString.compare("Bronze") == 0)   grade = 0;
		else if(gradeString.compare("silver") == 0   || gradeString.compare("Silver") == 0)   grade = 1;	
		else if(gradeString.compare("gold") == 0     || gradeString.compare("Gold") == 0)     grade = 2;	
		else if(gradeString.compare("platinum") == 0 || gradeString.compare("Platinum") == 0) grade = 3;
		else{
			cerr << "incorrect grade for trophy: " << title << endl;
			return -1;
		}	

		//push back Trophy objects into trophies to store all the data
		trophies.push_back(new Trophy(date, game, title, grade));
	}

	//check if output file can be opened
	ofs.open(argv[2]);
	if(ofs.is_open()){
		//run the graph option
		switch(option){
			case 0:
				BarMonthsAll(trophies, ofs);
				break;
			case 1:
				LineMonthsAll(trophies, ofs);
				break;
			case 2:
				TimeByGrade(trophies, ofs);
				break;
		}
		ofs.close();

	} else{
		cerr << "Can't open output file\n";
		return -1;
	}

	//free up data from trophy objects	
	for(i = 0; i < trophies.size(); i++){
		Trophy *tmp;
		tmp = trophies[i];
		delete tmp;
	}

	//call jgraph
	ofile = string(argv[2]);
	pos = ofile.find(".jgr");
	fileJPG = ofile.substr(0,pos);
	fileJPG += ".jpg";
	
	jgraph_call = "./jgraph/jgraph -P " + ofile + " | ps2pdf - | convert -density 300 - -quality 100 " + fileJPG;
	cout <<jgraph_call << endl;
	const char * cstring = jgraph_call.c_str();
	system(cstring);

	return 0;
}
