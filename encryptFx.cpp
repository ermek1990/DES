/*
 * encryptPx.cpp
 *
 *  Created on: Feb 27, 2016
 *      Author: fantom
 */

#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "utility.hpp"
#include "encryptFx.hpp"
#include "tablechecker.hpp"

using namespace std;
//Change the constants depending on mode specifications
const int Nk = 4;  // length of key
const int Nb = 4;  // length of block
const int Nr = 10; // number of rounds

UInt funcGetValSBox(int pos, string sbox)
{
	return funcHexToChar(sbox.substr(2 * pos, 2));
}

string funcInvertSBox(string sbox){
	int box[256];
	int ibox[256];
	for (int i = 0; i < 256; i++){
		UInt pos = funcGetValSBox(i, sbox);
		box[i] = (int)pos;
		ibox[box[i]] = i;
	}
	string isbox = "";
	for (int i = 0; i < 256; i++){
		isbox += funcIntToHex((unsigned)ibox[i]);
	}
	return isbox;
}

vector<UInt> funcSubWord(vector<UInt> in, string sbox){
	vector<UInt> v;
	for (int i = 0; i < Nb; i++){
		v.push_back(funcGetValSBox((int)(unsigned char)in[i], sbox));
	}
	return v;
}

vector<UInt> funcRotWord(vector<UInt> word){
	UInt c = word[0];
	word.push_back(c);
	word.erase(word.begin());
	return word;
}

vector<UInt> funcInvRotWord(vector<UInt> word){
	UInt c = word[Nb - 1];
	word.insert(word.begin(), c);
	word.pop_back();
	return word;
}

vector< vector<UInt> > funcAddRoundKey(vector< vector<UInt> > in_vec, vector< vector<UInt> > rkey){
	for (size_t i = 0; i < in_vec.size(); i++){
		for(int j = 0; j < Nk; j++){
			in_vec[i][j] ^= rkey[i][j];
		}
	}
	return in_vec;
}

vector< vector<UInt> > funcSubBytes(vector< vector<UInt> > in_vec, string sbox){
	vector< vector<UInt> > vec;
	for (int i = 0; i < Nb; i++){
		vec.push_back(funcSubWord(in_vec[i], sbox));
	}
	return vec;
}

vector< vector<UInt> > funcShiftRows(vector< vector<UInt> > in_vec){
	vector< vector<UInt> > vec1;
	for (int i = 0; i < 4; i++){
		vector<UInt> temp;
		for (int j = 0; j < 4; j++)
			temp.push_back(in_vec[j][i]);
		int k = i;
		while (k > 0){
			temp = funcRotWord(temp);
			k--;
		}
		vec1.push_back(temp);
	}
	vector< vector<UInt> > vec2;
	for (int i = 0; i < 4; i++){
		vector<UInt> temp;
		for (int j = 0; j < 4; j++)
			temp.push_back(vec1[j][i]);
		vec2.push_back(temp);
	}
	return vec2;
}

vector< vector<UInt> > funcInvShiftRows(vector< vector<UInt> > in_vec){
	vector< vector<UInt> > vec1;
	for (int i = 0; i < 4; i++){
		vector<UInt> temp;
		for (int j = 0; j < 4; j++)
			temp.push_back(in_vec[j][i]);
		int k = i;
		while (k > 0){
			temp = funcInvRotWord(temp);
			k--;
		}
		vec1.push_back(temp);
	}
	vector< vector<UInt> > vec2;
	for (int i = 0; i < 4; i++){
		vector<UInt> temp;
		for (int j = 0; j < 4; j++)
			temp.push_back(vec1[j][i]);
		vec2.push_back(temp);
	}
	return vec2;
}

vector< vector<UInt> > funcMixColumns(vector< vector<UInt> > in_vec, vector<UInt> p){
	vector< vector<UInt> > vec;
	for (int i = 0; i < Nb; i++){
		vector<UInt> temp;
		for (int j = Nb; j > 0; j--)
			temp.push_back(in_vec[i][j - 1]);
		vec.push_back(funcCircleX(p, temp));
	}
	return vec;
}

vector< vector<UInt> > funcExtractRndKey(vector< vector<UInt> > rkeys, int rnd){
	vector< vector<UInt> > vec;
	for (size_t i = 0; i < rkeys.size(); i++){
		if (i >= (unsigned)(rnd * 4) && i <= (unsigned)(rnd * 4 + 3))
			vec.push_back(rkeys[i]);
		if (i == (unsigned)(rnd * 4 + 3))
			break;
	}
	return vec;
}

string funcPrintVector(vector< vector<UInt> > vector, int rnd, string ending){
	stringstream ss;
	if (rnd < 10)
		ss << "round[ ";
	else
		ss << "round[";
	ss << dec << rnd << "]." << ending;
	if (ending[0] == 'i'){
		if (ending != "ioutput")
			ss << "   ";
		else
			ss << "  ";
	} else {
		if (ending != "output")
			ss << "    ";
		else
			ss << "   ";
	}
	for (size_t i = 0; i < vector.size(); i++){
		for (int j = 0; j < Nb; j++)
		{
			ss << hex << setfill('0') << setw(2) << (int)(unsigned char)vector[i][j];
		}
	}
	ss << '\n';
	return ss.str();
}

string funcModProd(string p1, string p2){
	vector<UInt> pv1 = funcHexToVector(p1), pv2 = funcHexToVector(p2);
	vector<UInt> res = funcCircleX(pv1, pv2);
	stringstream ss;
	for (std::vector<UInt>::size_type i = 0; i != pv1.size(); i++){
		ss << hex << "{" << setfill('0') << setw(2) <<  pv1[i] << "}";
	}
	ss << " CIRCLEX ";
	for (std::vector<UInt>::size_type i = 0; i != pv2.size(); i++){
		ss << hex << "{" << setfill('0') << setw(2) <<  pv2[i] << "}";
	}
	ss << " = ";
	for (std::vector<UInt>::size_type i = res.size(); i != 0; i--){
		ss << hex << "{" << setfill('0') << setw(2) <<  res[i - 1] << "}";
	}
	ss << '\n';
	return ss.str();
}

vector< vector<UInt> > funcKeyExpand(string keystring, string tables){
	int Rcon[14] = {
			0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
			0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab
	};
	string sbox = "", line = "";
	stringstream ss(tables);
	while (getline(ss, line)){
		size_t pos_end = line.find('=', 0);
		if (pos_end != string::npos){
			string param = line.substr(0, pos_end);
			if (param == "S")
				sbox = line.substr(pos_end + 1, line.size() - 1);
		}
	}
	if (!funcCheckHexTab(sbox, "S")){
		std::exit(1);
	}
	vector<UInt> key = funcHexToVector(keystring);
	vector< vector<UInt> > rkeys;
	ss.str(string());
	ss.clear();
	for (int i = 0; i < Nk; i++){
		vector<UInt> temp_vec;
		temp_vec.push_back(key[4 * i]);
		temp_vec.push_back(key[4 * i + 1]);
		temp_vec.push_back(key[4 * i + 2]);
		temp_vec.push_back(key[4 * i + 3]);
		rkeys.push_back(temp_vec);
	}
	for (size_t i = Nk; i < (Nb * (Nr + 1)); i++){
		vector<UInt> temp_vec = rkeys[i - 1];
		if (i % Nk == 0) {
			temp_vec = funcRotWord(temp_vec);
			temp_vec = funcSubWord(temp_vec, sbox);
			temp_vec[0] ^= Rcon[i / Nk];
		} else if ((Nk > 6) && (i % Nk == 4)){
			temp_vec = funcSubWord(temp_vec, sbox);   // Never reaches here if Nb = 4
		}
		for(int j = 0; j < Nk; j++){
			temp_vec[j] ^= rkeys[i - Nk][j];
		}
		rkeys.push_back(temp_vec);
	}
	return rkeys;
}

string funcKeyExpandPrint(string keystring, string tables){
	vector< vector<UInt> > roundkeys = funcKeyExpand(keystring, tables);
	stringstream ss;
	for (size_t i = 0; i < roundkeys.size(); i++){
		if (i < 10)
			ss << "w[ " << dec << i << "]: ";
		else
			ss << "w[" << dec << i << "]: ";
		for (int j = 0; j < Nb; j++)
		{
			ss << hex << setfill('0') << setw(2) << roundkeys[i][j];
		}
		ss << '\n';
	}
	return ss.str();
}

vector< vector<UInt> > funcVectorDiv(vector< vector<UInt> > r, int k){
	vector< vector<UInt> > res_vec;
	vector<UInt> res_quo, dvd = r[k - 2], dvs = r[k - 1];
	while (dvd.size() >= dvs.size()){
		while(dvd[0] == 0)
			dvd.erase(dvd.begin());
		while(dvs[0] == 0)
			dvs.erase(dvs.begin());
		vector<UInt> temp;
		UInt n = 0;
		for (n = 0; n < 256; n++){
			UInt c = funcBigDot(dvs[0], n);
			if (dvd.size() == 1){
				if (c == 0x01){
					for (UInt k = 0; k < 256; k++){
						UInt cc = funcBigDot(c, k);
						if (cc == (dvd[0] ^ 0x01)){
							UInt m = funcBigDot(k, n);
							res_quo.push_back(m);
							temp.push_back(cc);
							break;
						}
					}
				}
			} else {
				if (c == dvd[0]){
					res_quo.push_back(n);
					temp.push_back(dvd[0]);
					break;
				}
			}
		}
		for (size_t s = 0; s < dvs.size(); s++){
			if (s != 0){
				UInt c = funcBigDot(dvs[s], n);
				temp.push_back(c);
			}
			dvd[s] ^= temp[s];
		}
		while(dvd[0] == 0)
			dvd.erase(dvd.begin());
		if (dvd.size() == 1 && dvd[0] == 0x01)
			break;
	}
	while (dvd.size() != 4)
		dvd.insert(dvd.begin(), 0);
	while (res_quo.size() != 4)
		res_quo.insert(res_quo.begin(), 0);
	res_vec.push_back(dvd);
	res_vec.push_back(res_quo);
	return res_vec;
}

string funcInverse(string p){
	stringstream ss;
	vector<UInt> z0 = funcHexToVector("00000000");
	vector<UInt> z1 = funcHexToVector("00000001");
	vector< vector<UInt> > quo, rem, x;
	rem.push_back(funcHexToVector("0100000001"));
	rem.push_back(funcHexToVector(p));
	x.push_back(z0);
	x.push_back(z1);
	quo.push_back(z0);
	quo.push_back(z0);
	int i = 0;
	for (i = 0; i < 2; i++){
		ss << "i=" << dec << i + 1
				<< ", rem[i]=" << funcPrintV(rem[i])
				<< ", quo[i]=" << funcPrintV(quo[i])
				<< ", aux[i]=" << funcPrintV(x[i]) << endl;
	}
	while (rem[i - 1] != z1 && rem[i - 1] != z0){
		vector< vector<UInt> > v = funcVectorDiv(rem, i);
		quo.push_back(v[1]);
		rem.push_back(v[0]);
		vector<UInt> temp_x;
		vector<UInt> vec = funcCircleX(x[i - 1], quo[i]);
		for (int t = 0; t < 4; t++){
			UInt cx = x[i - 2][t] ^ vec[3 - t];
			temp_x.push_back(cx);
		}
		x.push_back(temp_x);
		ss << "i=" << dec << i + 1 << ", rem[i]=" << funcPrintV(rem[i]) << ", quo[i]=" << funcPrintV(quo[i]) << ", aux[i]=" << funcPrintV(x[i]) << '\n';
		i++;
	}
	if (rem[i - 1] == z1){
		ss << "Multiplicative inverse of " << funcPrintV(rem[1]) << " is " << funcPrintV(x[i - 1]) << endl;
	} else if (rem[i - 1] == z0){
		ss << funcPrintV(rem[1]) << " does not have a multiplicative inverse." << endl;
	}
	return ss.str();
}

string funcAESEncrypt(string keystring, string tablestring, string input){
	string sbox = "", line = "";
	stringstream ss(tablestring);
	vector<UInt> pvec;
	while (getline(ss, line)){
		size_t pos_end = line.find('=', 0);
		if (pos_end != string::npos){
			string param = line.substr(0, pos_end);
			if (param == "S")
				sbox = line.substr(pos_end + 1, line.size() - 1);
			if (param == "P"){
				pvec = funcHexToVector(line.substr(pos_end + 1, line.size() - 1));
			}
		}
	}
	ss.str(string());
	ss.clear();
	vector< vector<UInt> > rkeys = funcKeyExpand(keystring, tablestring);
	int blocks = (int)input.size() / 16;
	for (int z = 0; z < blocks; z++){
		string block = input.substr(z * 16, 16);
		ss << "round[ 0].input    ";
		for (size_t i = 0; i < block.size(); i++)
			ss << hex << setfill('0') << setw(2) << (int)(unsigned char)block[i];
		ss << endl;
		vector< vector<UInt> > state;
		for(int i = 0; i < 4; i++)
		{
			vector<UInt> temp_v;
			for(int j = 0; j < Nb; j++)
			{
				temp_v.push_back((UInt)block[i * 4 + j]);
			}
			state.push_back(temp_v);
		}
		vector< vector<UInt> > rkey = funcExtractRndKey(rkeys, 0);
		ss << funcPrintVector(rkey, 0, "k_sch");
		state = funcAddRoundKey(state, rkey);
		for (int r = 1; r < Nr; r++) {
			ss << funcPrintVector(state, r, "start");
			state = funcSubBytes(state, sbox);
			ss << funcPrintVector(state, r, "s_box");
			state = funcShiftRows(state);
			ss << funcPrintVector(state, r, "s_row");
			state = funcMixColumns(state, pvec);
			ss << funcPrintVector(state, r, "m_col");
			rkey.clear();
			rkey = funcExtractRndKey(rkeys, r);
			ss << funcPrintVector(rkey, r, "k_sch");
			state = funcAddRoundKey(state, rkey);
		}
		ss << funcPrintVector(state, 10, "start");
		state = funcSubBytes(state, sbox);
		ss << funcPrintVector(state, 10, "s_box");
		state = funcShiftRows(state);
		ss << funcPrintVector(state, 10, "s_row");
		rkey.clear();
		rkey = funcExtractRndKey(rkeys, 10);
		state = funcAddRoundKey(state, rkey);
		ss << funcPrintVector(rkey, 10, "k_sch");
		ss << funcPrintVector(state, 10, "output");
	}
	return ss.str();
}

string funcAESDecrypt(string keystring, string tablestring, string input){
	string sbox = "", line = "";
	stringstream ss(tablestring);
	vector<UInt> invpvec;
	while (getline(ss, line)){
		size_t pos_end = line.find('=', 0);
		if (pos_end != string::npos){
			string param = line.substr(0, pos_end);
			if (param == "S")
				sbox = line.substr(pos_end + 1, line.size() - 1);
			if (param == "INVP"){
				invpvec = funcHexToVector(line.substr(pos_end + 1, line.size() - 1));
			}
		}
	}
	ss.str(string());
	ss.clear();
	sbox = funcInvertSBox(sbox);
	vector< vector<UInt> > rkeys = funcKeyExpand(keystring, tablestring);
	int blocks = (int)input.size() / 16;
	for (int z = 0; z < blocks; z++){
		string block = input.substr(z * 16, 16);
		ss << "round[ 0].iinput   ";
		for (size_t i = 0; i < block.size(); i++)
			ss << hex << setfill('0') << setw(2) << (int)(unsigned char)block[i];
		ss << endl;
		vector< vector<UInt> > state;
		for(int i = 0; i < 4; i++)
		{
			vector<UInt> temp_v;
			for(int j = 0; j < Nb; j++)
			{
				temp_v.push_back((UInt)block[i * 4 + j]);
			}
			state.push_back(temp_v);
		}
		vector< vector<UInt> > rkey = funcExtractRndKey(rkeys, 10);
		ss << funcPrintVector(rkey, 0, "ik_sch");
		state = funcAddRoundKey(state, rkey);
		for (int r = 1; r < Nr; r++) {
			ss << funcPrintVector(state, r, "istart");
			state = funcInvShiftRows(state);
			ss << funcPrintVector(state, r, "is_row");
			state = funcSubBytes(state, sbox);
			ss << funcPrintVector(state, r, "is_box");
			rkey.clear();
			int rnum = Nr - r;
			rkey = funcExtractRndKey(rkeys, rnum);
			ss << funcPrintVector(rkey, r, "ik_sch");
			state = funcAddRoundKey(state, rkey);
			ss << funcPrintVector(state, r, "ik_add");
			state = funcMixColumns(state, invpvec);
		}
		ss << funcPrintVector(state, 10, "istart");
		state = funcInvShiftRows(state);
		ss << funcPrintVector(state, 10, "is_row");
		state = funcSubBytes(state, sbox);
		ss << funcPrintVector(state, 10, "is_box");
		rkey.clear();
		rkey = funcExtractRndKey(rkeys, 0);
		ss << funcPrintVector(rkey, 10, "ik_sch");
		state = funcAddRoundKey(state, rkey);
		ss << funcPrintVector(state, 10, "ioutput");
	}
	return ss.str();
}
