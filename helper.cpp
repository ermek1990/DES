//
//  helper.cpp
//  HW5
//
//  Created by Ermek Zhanabayev on 2/27/16.
//  Copyright © 2016 h.App.y. All rights reserved.
//

#include <iostream>
#include <string>
#include "helper.hpp"

void showHelpInfo(){
	std::cerr << "USAGE:          " << "./hw5" << " operation_name [options]" << std::endl;
	std::cerr << "operation_name: " << "tablecheck" << std::endl;
	std::cerr << "                " << "modprod" << std::endl;
	std::cerr << "                " << "keyexpand" << std::endl;
	std::cerr << "                " << "encrypt" << std::endl;
	std::cerr << "                " << "decrypt" << std::endl;
	std::cerr << "                " << "inverse" << std::endl;
	std::cerr << "[options]" << std::endl;
	std::cerr << "hw5 tablecheck -t=tablefile" << std::endl;
	std::cerr << "hw5 modprod -p1=poly1 -p2=poly2" << std::endl;
	std::cerr << "hw5 keyexpand -k=key -t=tablefile" << std::endl;
	std::cerr << "hw5 encrypt -k=key -t=tablefile [file]" << std::endl;
	std::cerr << "hw5 decrypt -k=key -t=tablefile [file]" << std::endl;
	std::cerr << "hw5 inverse -p=poly" << std::endl;
}
