/*
 * Utility.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: fantom
 */

#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "utility.hpp"

using namespace std;

typedef unsigned int UInt;
typedef unsigned long ULong;

vector<UInt> funcLongToVector(ULong val)
{
  vector<UInt> vector;
  vector.push_back(val & 0xFF);
  vector.push_back((val & 0xFF00) >> 8);
  vector.push_back((val & 0xFF0000) >> 16);
  vector.push_back((val & 0xFF000000) >> 24);
  return vector;
}

ULong funcVectorToLong(vector<UInt> in_vec){
	ULong in = 0;
	for (size_t i = 0; i < in_vec.size(); i++){
		in = (in << 8) + in_vec[i];
	}
	return in;
}

vector<UInt> funcHexToVector(string hexstring){
	vector<UInt> vector;
	size_t n = hexstring.size() / 2;
	for (size_t i = 0; i < n; i++){
		vector.push_back(funcHexToChar(hexstring.substr(i * 2, 2)));
	}
	return vector;
}

UInt funcHexToChar(string input){
	stringstream ss(input);
	UInt c;
	ss >> hex >> c;
	return c;
}

string funcIntToHex(unsigned int n)
{
	stringstream ss;
	ss << setfill('0') << setw(2) << hex << n;
	return ss.str();
}

string funcPrintV(vector<UInt> in_vec){
	stringstream ss;
	if(in_vec.size() == 5)
		for (int i = 1; i < 5; i++)
			ss << hex << "{" << setfill('0') << setw(2) <<  in_vec[i] << "}";
	else
		for (int i = 0; i < 4; i++)
			ss << hex << "{" << setfill('0') << setw(2) <<  in_vec[i] << "}";
	return ss.str();
}

bool funcCheckHex(string input){
	for (size_t i = 0; i < input.size(); i++){
		if (!isxdigit(input[i])){
			cerr << "E: The input string contains incorrect characters. It should contain a hex string only." << endl;
			return false;
		}
	}
	return true;
}

bool funcCheckHexTab(string input, string param){
	if (!funcCheckHex(input)){
		cerr << "E: The " << "'" << param << "'" << " table contains incorrect characters. The table should contain a hex string only." << endl;
		return false;
	}
	if (param == "S")
		for (unsigned int i = 0; i < 256; i++){
			string hexv = funcIntToHex(i);
			size_t pos = input.find(hexv, 0);
			if (pos == string::npos){
				cerr << "E: The " << "'" << param << "'" << " table does not contain " << "{" << hexv << "}." << endl;
				return false;
			}
		}
	return true;
}

UInt funcBigDot(UInt a, UInt b){
	unsigned char c = 0;
	for (int i = 0; i < 8; i++){
		if (b & 1){
			c ^= a;
		}
		UInt bit = a & 0x80;
		a <<= 1;
		b >>= 1;
		if (bit){
			a ^= 0x1b;
		}
	}
	return c;
}

vector<UInt> funcCircleX(vector<UInt> a, vector<UInt> s){
	vector<UInt> vector;
	vector.push_back(funcBigDot(a[3], s[3]) ^ funcBigDot(a[0], s[2]) ^ funcBigDot(a[1], s[1]) ^ funcBigDot(a[2], s[0]));
	vector.push_back(funcBigDot(a[2], s[3]) ^ funcBigDot(a[3], s[2]) ^ funcBigDot(a[0], s[1]) ^ funcBigDot(a[1], s[0]));
	vector.push_back(funcBigDot(a[1], s[3]) ^ funcBigDot(a[2], s[2]) ^ funcBigDot(a[3], s[1]) ^ funcBigDot(a[0], s[0]));
	vector.push_back(funcBigDot(a[0], s[3]) ^ funcBigDot(a[1], s[2]) ^ funcBigDot(a[2], s[1]) ^ funcBigDot(a[3], s[0]));
	return vector;
}
