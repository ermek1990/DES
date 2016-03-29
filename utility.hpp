/*
 * utility.hpp
 *
 *  Created on: Mar 20, 2016
 *      Author: fantom
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <stdio.h>

typedef unsigned int UInt;
typedef unsigned long ULong;

std::vector<UInt> funcLongToVector(ULong val);

ULong funcVectorToLong(std::vector<UInt> in_vec);

std::vector<UInt> funcHexToVector(std::string hexstring);

UInt funcHexToChar(std::string input);

std::string funcIntToHex(unsigned int n);

std::string funcPrintV(std::vector<UInt> in_vec);

bool funcCheckHex(std::string input);

bool funcCheckHexTab(std::string input, std::string param);

UInt funcXTime(char a);

UInt funcBigDot(UInt a, UInt b);

std::vector<UInt> funcCircleX(std::vector<UInt> a, std::vector<UInt> s);

#endif /* UTILITY_HPP_ */
