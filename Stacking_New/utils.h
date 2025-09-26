/*
 * utils.h
 *
 *  Created on: 17 Sept 2025
 *      Author: abhishek
 */

#ifndef UTILS_H_
#define UTILS_H_
#include "stacking.h"
#include<cstring>
#include "limits.h"




class Utils {
public:
	static char* heads;

	Utils();
	virtual ~Utils();
	static void get_start_inline_xline_from_vel(ifstream& inpv, Stacking *stk);
	static void get_filesize(std::ifstream& inp, long int& size);
	static void get_ns(std::ifstream& inp, short int& ns);
	static void get_ntrace(long int size, short ns, long int& ntrace);
	static void get_dt(std::ifstream& inp, short int& dt);
	static void get_min_max_in_xline_from_data(ifstream &fp_in,long ntrace,int& startinline, int& startxline, int& endinline, int& endxline);
	static void set_outputfile_name(string imagefile, string &ofile, Stacking *stk,string ext);
	static void selfdoc(void);
};

#endif /* UTILS_H_ */
