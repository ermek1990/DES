//
//  main.cpp
//  HW5
//
//  Created by Ermek Zhanabayev on 3/15/16.
//  Copyright © 2016 h.App.y. All rights reserved.
//

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "helper.hpp"
#include "utility.hpp"
#include "encryptFx.hpp"
#include "tablechecker.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
	if (argc >= 2){
		argc--, argv++;
		int operation = 0;
		if (strcmp(*argv, "tablecheck") == 0) {
			operation = 1;
		} else if (strcmp(*argv, "modprod") == 0) {
			operation = 2;
		} else if (strcmp(*argv, "keyexpand") == 0) {
			operation = 3;
		} else if (strcmp(*argv, "encrypt") == 0) {
			operation = 4;
		} else if (strcmp(*argv, "decrypt") == 0) {
			operation = 5;
		} else if (strcmp(*argv, "inverse") == 0) {
			operation = 6;
		} else {
			showHelpInfo();
		}
		if (operation == 1)
		{
			if (argc == 2){
                argv++, argc--;
                string tablefile = "", tablestring = "";
                int index = (int)(strchr(argv[0], '-') - argv[0]);
                if (index == 0) {
                	if (strstr(argv[0], "-t=") != NULL) {
                		string s(argv[0]);
                		tablefile = s.substr(3, s.length());
                	}
                } else {
                	cerr << "E: Please check the input parameters. You specified them incorrectly. Please try again." << endl;
                	showHelpInfo();
                	return 1;
                }
                if (tablefile.empty()) {
                	cerr << "E: Please check the input parameters. You did not specify the table file. Please try again." << endl;
                	showHelpInfo();
                	return 1;
                } else {
                	ifstream file(tablefile.c_str(), ios::in|ios::binary);
                	if (file.is_open())
                	{
                		file.seekg(0, ios::end);
                		size_t readBufSize = file.tellg();
                		if (readBufSize == 0) {
                			cerr << "E: The table file is empty. Please try another file." << endl;
                			return 1;
                		}
                		file.seekg(0, ios::beg);
                		vector<unsigned char> vector(readBufSize);
                		file.read((char*) &vector[0], readBufSize);
                		file.close();
                		tablestring = string(vector.begin(), vector.end());
                		size_t rows = count(tablestring.begin(), tablestring.end(), '\n');
                		if (rows != 3){
                			cerr << "E: Terminated since the table file is malformed." << endl;
                			return 1;
                		}
                	} else {
                		cerr << "E: Unable to open the file with the tables. The file is not existent or corrupted." << endl;
                		return 1;
                	}
                }
                return funcCheckTable(tablestring);
            } else {
				cerr << "E: Please check the input parameters. You did not specify them, or you specified them incorrectly. Please try again." << endl;
				showHelpInfo();
				return 1;
			}
		} else if (operation == 2){
			if (argc == 3){
				argc--, argv++;
				string p1 = "", p2 = "";
				for (int i = 0; i < argc; i++){
					int index = (int)(strchr(argv[i], '-') - argv[i]);
					if (index == 0) {
						if (strstr(argv[i], "-p1=") != NULL) {
							string s(argv[i]);
							p1 = s.substr(4, strlen(argv[i]));
						}
						if (strstr(argv[i], "-p2=") != NULL) {
							string s(argv[i]);
							p2 = s.substr(4, s.length());
						}
					}
				}
				if (p1.empty() || p2.empty()) {
					cerr << "E: Please check the input parameters. You did not specify parameters." << endl;
					showHelpInfo();
					return 1;
				} else {
					if (p1.size() != 8 || p2.size() != 8){
						cerr << "E: Either P1 or P2 is not of a length 8." << endl;
						return 1;
					} else {
						if (!funcCheckHex(p1)){
							cerr << "E: P1 contains incorrect characters. It should contain a hex string only." << endl;
							return 1;
						}
						if (!funcCheckHex(p2)){
							cerr << "E: P2 contains incorrect characters. It should contain a hex string only." << endl;
							return 1;
						}
					}
				}
				cout << funcModProd(p1, p2);
			} else {
				cerr << "E: Please check the input parameters. You did not specify them, or you specified them incorrectly. Please try again." << endl;
				showHelpInfo();
				return 1;
			}
		} else if (operation == 3){
			if (argc == 3){
				argc--, argv++;
				string keystring = "", tablefile = "", tablestring = "";
				for (int i = 0; i < argc; i++){
					int index = (int)(strchr(argv[i], '-') - argv[i]);
					if (index == 0) {
						if (strstr(argv[i], "-k=") != NULL){
							string s(argv[i]);
							keystring = s.substr(3, strlen(argv[i]));
						}
						if (strstr(argv[i], "-t=") != NULL){
							string s(argv[i]);
							tablefile = s.substr(3, s.length());
						}
					}
				}
				if (keystring.empty()){
					cerr << "E: Please check the input parameters. You did not specify the key parameter. Please try again." << endl;
					showHelpInfo();
					return 1;
				} else {
					if (keystring.size() != 32){
						cerr << "E: Please check the key parameter. It is either too short or too long for this operation." << endl;
						return 1;
					} else {
							if (!funcCheckHex(keystring)){
								cerr << "E: The key contains incorrect characters. The key should be a hex string." << endl;
								return 1;
							}
					}
				}
				if (tablefile.empty()){
					cerr << "E: Please check the input parameters. You did not specify the table file. Please try again." << endl;
					showHelpInfo();
					return 1;
				} else {
					ifstream file(tablefile.c_str(), ios::in|ios::binary);
					if (file.is_open())
					{
						file.seekg(0, ios::end);
						size_t readBufSize = file.tellg();
						if (readBufSize == 0){
							cerr << "E: The table file is empty. Please try another file." << endl;
							return 1;
						}
						file.seekg(0, ios::beg);
						vector<unsigned char> vector(readBufSize);
						file.read((char*) &vector[0], readBufSize);
						file.close();
						string str(vector.begin(), vector.end());
						tablestring = str;
						size_t rows = count(tablestring.begin(), tablestring.end(), '\n');
						if (rows != 3){
							cerr << "E: Terminated since the table file is malformed." << endl;
							return 1;
						}
						if (!funcCheckTable(tablestring))
							return 1;
					} else {
						cerr << "E: Unable to open the file with the tables. The file is not existent or corrupted." << endl;
						return 1;
					}
				}
				cout << funcKeyExpandPrint(keystring, tablestring);
			} else {
				cerr << "E: Please check the input parameters. You did not specify them, or you specified them incorrectly. Please try again." << endl;
				showHelpInfo();
				return 1;
			}
		} else if (operation == 4 || operation == 5){
			if (argc >= 3){
				argc--, argv++;
				string keystring = "", tablefile = "", tablestring = "", filename = "", input = "";
				for (int i = 0; i < argc; i++){
					int index = (int)(strchr(argv[i], '-') - argv[i]);
					if (index == 0) {
						if (strstr(argv[i], "-k=") != NULL) {
							string s(argv[i]);
							keystring = s.substr(3, strlen(argv[i]));
						}
						if (strstr(argv[i], "-t=") != NULL) {
							string s(argv[i]);
							tablefile = s.substr(3, s.length());
						}
					} else {
						string ss(argv[i]);
						filename = ss;
					}
				}
				if (keystring.empty()){
					cerr << "E: Please check the input parameters. You did not specify the key parameter. Please try again." << endl;
					showHelpInfo();
					return 1;
				} else {
					if (keystring.size() != 32){
						cerr << "E: Please check the key parameter. It is either too short or too long for this operation." << endl;
						return 1;
					} else {
						if (!funcCheckHex(keystring)){
							cerr << "E: The key contains incorrect characters. The key should be a hex string." << endl;
							return 1;
						}
					}
				}
				if (tablefile.empty()) {
					cerr << "E: Please check the input parameters. You did not specify the table file. Please try again." << endl;
					showHelpInfo();
					return 1;
				} else {
					ifstream file(tablefile.c_str(), ios::in|ios::binary);
					if (file.is_open())
					{
						file.seekg(0, ios::end);
						size_t readBufSize = file.tellg();
						if (readBufSize == 0) {
							cerr << "E: The table file is empty. Please try another file." << endl;
							return 1;
						}
						file.seekg(0, ios::beg);
						vector<unsigned char> vector(readBufSize);
						file.read((char*) &vector[0], readBufSize);
						file.close();
						string str(vector.begin(), vector.end());
						tablestring = str;
						size_t rows = count(tablestring.begin(), tablestring.end(), '\n');
						if (rows != 3){
							cerr << "E: Terminated since the table file is malformed." << endl;
							return 1;
						}
						if (!funcCheckTable(tablestring))
							return 1;
					} else {
						cerr << "E: Unable to open the file with the tables. The file is not existent or corrupted." << endl;
						return 1;
					}
				}
				if (filename.empty()){
					while(!cin.eof()){
						char readBuf[256];
						cin.read(readBuf, sizeof(readBuf));
						size_t readBufSize = 0;
						readBufSize += cin.gcount();
						input.append(readBuf, cin.gcount());
					}
				} else {
					ifstream file(filename.c_str(), ios::in|ios::binary);
					if (file.is_open())
					{
						file.seekg(0, ios::end);
						size_t readBufSize = file.tellg();
						if (readBufSize == 0) {
							cerr << "E: The file is empty. Please try another file." << endl;
							return 1;
						}
						file.seekg(0, ios::beg);
						vector<unsigned char> vector(readBufSize);
						file.read((char*) &vector[0], readBufSize);
						file.close();
						string s(vector.begin(), vector.end());
						input = s;
					} else {
						cerr << "E: Unable to open file. The file is not existent or corrupted." << endl;
						return 1;
					}
				}
				if (input.size() % 16 != 0){
					cerr << "E: The length of the input is less than 16 bytes. Please try another file or input string." << endl;
					return 1;
				}
				if (operation == 4)
					cout << funcAESEncrypt(keystring, tablestring, input);
				if (operation == 5)
					cout << funcAESDecrypt(keystring, tablestring, input);
			} else {
				cerr << "E: Please check the input parameters. You did not specify them, or you specified them incorrectly. Please try again." << endl;
				showHelpInfo();
				return 1;
			}
		} else if (operation == 6){
			if (argc == 2){
				argc--, argv++;
				string p = "";
				for (int i = 0; i < argc; i++){
					int index = (int)(strchr(argv[i], '-') - argv[i]);
					if (index == 0) {
						if (strstr(argv[i], "-p=") != NULL) {
							string s(argv[i]);
							p = s.substr(3, strlen(argv[i]));
						}
					}
				}
				if (p.empty()) {
					cerr << "E: Please check the input parameters. You did not specify parameters." << endl;
					showHelpInfo();
					return 1;
				} else {
					if (p.size() != 8){
						cerr << "E: Either P1 or P2 is not of a length 8." << endl;
						return 1;
					} else {
						if (!funcCheckHex(p)){
							cerr << "E: P1 contains incorrect characters. It should contain a hex string only." << endl;
							return 1;
						}
					}
				}
				cout << funcInverse(p);
			} else {
				cerr << "E: Please check the input parameters. You did not specify them, or you specified them incorrectly. Please try again." << endl;
				showHelpInfo();
				return 1;
			}
		}
		return 0;
	} else {
		showHelpInfo();
		return 1;
	}
}
