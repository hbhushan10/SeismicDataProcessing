/*
 * utils.cpp
 *
 *  Created on: 04-Mar-2026
 *      Author: abhishek
 */

#include "utils.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}
void Utils::get_filesize(std::ifstream& inp, long int& size)
{
	inp.clear();
    inp.seekg(0,std::ios::end);
    size = inp.tellg();
    inp.clear();
    inp.seekg(0,std::ios::beg);
}

void Utils::get_ns(std::ifstream& inp, short int& ns)
{
    char head[HDRBYTES];
    inp.clear();
    inp.seekg(0,std::ios::beg);
    inp.read(head,HDRBYTES);
    ns = *((short int*)(head + NS));
    inp.clear();
    inp.seekg(0,std::ios::beg);
}

void Utils::get_dt(std::ifstream& inp, short int& dt)
{
    char head[HDRBYTES];
    inp.clear();
    inp.seekg(0,std::ios::beg);
    inp.read(head,HDRBYTES);
    dt = *((short int*)(head + DT));
    inp.clear();
    inp.seekg(0,std::ios::beg);
}

void Utils::get_ntrace(long int size, short ns, long int& ntrace)
{
    ntrace = size / (HDRBYTES + (ns*sizeof(float)));
}

void Utils::get_imgGP_first_trace(ifstream& inp, int &imgp, int &fldr_old){
	inp.clear();
	inp.seekg(0, std::ios::beg);
	char head[HDRBYTES];
	inp.read(head, HDRBYTES);
	imgp     = *((int*) (head + IMGP));
	fldr_old = *((int*) (head + FLDR));
	inp.clear();
	inp.seekg(0, std::ios::beg);
}

void Utils::selfdoc(void)
{
    FILE *fp;
    string sdoc =
    R"(SRMIGDENOISE- Performs Denoising on 2D Shot image gathers


                Syntax --> srmigdenoise  image_gathers Dimension_flag bin_x/dx
				Dimension_flag : 1- 1D (Depth)
								 2- 2D (Depth and X)

    "**************** end self doc **************************************)";

    fflush(stdout);
    fp = popen("more -p 1>&2", "w");
    fprintf(fp, "%s", sdoc.c_str());
    pclose(fp);
}

string Utils::set_outputfile_name(string imagefile,  int  dimFlag, string ext){
	string ofile;
    ofile = imagefile;
    size_t pos = ofile.find_last_of(".");

    if(pos != string::npos){
        ofile.erase(pos);
    }
	if (dimFlag == 1) {
		ofile.append("_1D_Denoise");
	} else if (dimFlag == 2) {
		ofile.append("_2D_Denoise");
	} else if (dimFlag == 3) {
		ofile.append("_3D_Denoise");
	}
    ofile.append(ext);
    return ofile;
}


