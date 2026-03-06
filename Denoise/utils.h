/*
 * utils.h
 *
 *  Created on: 04-Mar-2026
 *      Author: abhishek
 */

#ifndef UTILS_H_
#define UTILS_H_

#include<iostream>
#include<fstream>
#include "header_macros.h"
#include "denoise.h"
using namespace std;

#define HDRBYTES 240

class Utils {
public:
	Utils();
	virtual ~Utils();
	static void get_filesize(std::ifstream& inp, long int& size);
	static void get_ns(std::ifstream& inp, short int& ns);
	static void get_dt(std::ifstream& inp, short int& dt);
	static void get_ntrace(long int size, short ns, long int& ntrace);
	static void selfdoc(void);
	static void get_imgGP_first_trace(ifstream& inp, int &imgp, int &fldr_old);
	static string set_outputfile_name(string imagefile,  int  dimFlag, string ext);
};

#endif /* UTILS_H_ */
