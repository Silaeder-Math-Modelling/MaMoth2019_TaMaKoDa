#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

unsigned long long int get_n_of_lines(const string &file_name)
{
	ifstream f(file_name);
	string s;
	unsigned long long int ans = 0;
	while(getline(f, s))
		++ans;
	return ans;
}

void skip_to_semicolon(istringstream &ss)
{
	char trash = '\0';
    while(trash != ';')
	{
		ss >> trash;
	}
}

vector<pair<double, double>> get_credit_deposit_rate(const string &table_name) {
	/*Returns a pair of the credit/deposit value and it's rate
	 * Attention: credit value will be positive, deposit value - negative
	 Parameters:
	 	table_name - The csv file name to read data from. Attention:
			credit value must be first column, deposit value must be
			second column, rate must be fifth column, first line must
			contain columns' headers
	*/
	ifstream f(table_name);
	string tmp;
	getline(f, tmp);

	vector<pair<double, double>> ans;
	ans.reserve(get_n_of_lines(table_name));
	while(getline(f, tmp))
	{
		istringstream ss(tmp);
		pair<double, double> value;
		ss >> value.first;
		skip_to_semicolon(ss);
		ss >> value.second;
		if(value.first == 0)
		    ans.push_back({-value.second, 0});
		else
		    ans.push_back({value.first, 0});
		for(int i = 0; i < 3; ++i)
			skip_to_semicolon(ss);
		ss >> ans.back().second;
	}
	return ans;
}
