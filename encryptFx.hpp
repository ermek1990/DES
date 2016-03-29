/*
 * encryptFx.hpp
 *
 *  Created on: Feb 27, 2016
 *      Author: fantom
 */

#ifndef ENCRYPTFX_HPP_
#define ENCRYPTFX_HPP_

#include <stdio.h>

std::string funcModProd(std::string p1, std::string p2);

std::string funcKeyExpandPrint(std::string keystring, std::string tables);

std::string funcInverse(std::string p);

std::string funcAESEncrypt(std::string keystring, std::string tablestring, std::string input);

std::string funcAESDecrypt(std::string keystring, std::string tablestring, std::string input);

#endif /* ENCRYPTFX_HPP_ */
