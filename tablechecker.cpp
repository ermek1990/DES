//
//  tablechecker.cpp
//  HW5
//
//  Created by Ermek Zhanabayev on 3/15/16.
//  Copyright © 2016 h.App.y. All rights reserved.
//

#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "utility.hpp"
#include "tablechecker.hpp"

using namespace std;

bool funcCheckTable(string tablestring){
	stringstream ss(tablestring);
	string line = "", pstr = "", invpstr = "";
	while (getline(ss, line)){
		size_t pos_end = line.find('=', 0);
		if (pos_end != string::npos){
			string param = line.substr(0, pos_end);
			if (param == "P")
				pstr = line.substr(pos_end + 1, line.size() - 1);
			else if (param == "INVP")
				invpstr = line.substr(pos_end + 1, line.size() - 1);
			if (!funcCheckHexTab(line.substr(pos_end + 1, line.size() - 1), param))
				return false;
		}
	}
	if (pstr.size() != 8 || invpstr.size() != 8){
		cerr << "E: Table P or INVP is not of a length 8." << endl;
		return false;
	} else {
		vector<UInt> p = funcHexToVector(pstr), invp = funcHexToVector(invpstr);
		vector<UInt> res = funcCircleX(p, invp);
		if ((res[0] != 0x01) || (res[1] != 0x00) || (res[2] != 0x00) || (res[3] != 0x00)){
			cerr << "Product of polynomials in P and INVP is NOT {00}{00}{00}{01}" << endl;
			return false;
		}
	}
	return true;
}
