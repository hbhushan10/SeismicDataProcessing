/*
 * utils.cpp
 *
 *  Created on: 17 Sept 2025
 *      Author: abhishek
 */

#include "utils.h"
#include "header_macros.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}

void Utils::get_start_inline_xline_from_vel(ifstream& inpv, Stacking *stk)
{
	int  start_xline, start_inline, end_xline, end_inline;
	char head[HDRBYTES];
	short ns, dt;
	long fsize;
	Utils::get_ns(inpv,ns);
	Utils::get_dt(inpv,dt);
	inpv.read(head,HDRBYTES);
	start_xline =  *((int*)            (head + XLINE));
	start_inline = *((int*)            (head + INLINE));


	inpv.clear();
	inpv.seekg(0,std::ios::beg);

	Utils::get_filesize(inpv,fsize);

	fsize = fsize - (HDRBYTES + (ns*sizeof(float)));
	inpv.seekg(fsize,std::ios::beg);
	inpv.read(head,HDRBYTES);

	end_xline =  *((int*)            (head + XLINE));
	end_inline =  *((int*)            (head + INLINE));

	stk->setVelStartXline(start_xline);
	stk->setVelStartInline(start_inline);
	stk->setVelEndInline(end_inline);
	stk->setVelEndXline(end_xline);


}

void Utils::get_filesize(std::ifstream& inp, long int& size)
{
	inp.seekg(0,std::ios::end);
	size = inp.tellg();
	inp.seekg(0,std::ios::beg);
}
void Utils::get_ns(std::ifstream& inp, short int& ns)
{
	char head[HDRBYTES];
	inp.seekg(0,std::ios::beg);
	inp.read(head,HDRBYTES);
	ns = *((short int*)(head + NS));
	inp.seekg(0,std::ios::beg);
}

void Utils::get_dt(std::ifstream& inp, short int& dt)
{
	char head[HDRBYTES];
	inp.seekg(0,std::ios::beg);
	inp.read(head,HDRBYTES);
	dt = *((short int*)(head + DT));
	inp.seekg(0,std::ios::beg);
}

void Utils::get_ntrace(long int size, short ns, long int& ntrace)
{
    ntrace = size / (HDRBYTES + (ns*sizeof(float)));
}

void Utils::get_min_max_in_xline_from_data(ifstream &inpd,long ntrace,int &min_in, int &min_x, int &max_in, int &max_x){
	int  i, xln, inln;
	char head[HDRBYTES];
	short ns, dt;
	Utils::get_ns(inpd,ns);
	Utils::get_dt(inpd,dt);

	float *trace = new float[ns];

	min_in = INT_MAX;
	min_x = INT_MAX;
	max_in = INT_MIN;
	max_x = INT_MIN;

	for(i=0;i<ntrace;i++)
	    {
			inpd.read(head, HDRBYTES);
			inpd.read((char*)trace, sizeof(float)* ns);
	        xln = *((int*)  (head + XLINE));
	        inln = *((int*)  (head + INLINE));

	        if(xln < min_x)
	            min_x = xln;
	        if(xln > max_x)
	            max_x = xln;

	        if(inln < min_in)
	            min_in = inln;
	        if(inln > max_in)
	            max_in = inln;

	    }

	inpd.clear();
	inpd.seekg(0,std::ios::beg);



}

void Utils::set_outputfile_name(string imagefile, string &ofile, Stacking *stk, string ext){
	ofile = imagefile;
	size_t pos = ofile.find_last_of(".");

	if(pos != string::npos){
		ofile.erase(pos);
	}
	ofile.append(to_string(int(stk->getImgStackedinline()*stk->getDx())));
	ofile.append("Inlineoff_");
	ofile.append(to_string(int(stk->getImgStackedxline()*stk->getDy())));
	ofile.append("Xlineoff");
	if(stk->getStackingtype() == MEAN)
		ofile.append("_MEAN");
	if(stk->getStackingtype() == COSINE)
		ofile.append("_COSINE");
	if(stk->getStackingtype() == NORMAL)
			ofile.append("_NORMAL");
	ofile.append(ext);

}


void Utils::selfdoc(void)
{
    FILE *fp;
    string sdoc =
    R"(SRMIG3DSTACK - Performs 3D stacking of Shot Image gathers


                Syntax --> srmig3dstack  velocity_file_in_sr_format shot_imagegather_file_in_sr_format stack_offset_along_inline_in_meters stack_offset_along_xline_in_meters xlinespacing inlinespacing Center Type_stacking
						    Center:  1- Shot Centric, 2- Fold Centric 
							Type of Stacking: 1-  Normal,  2- Mean, 3- Cosine Tapering 4-Weighted(OFFSET)

    "**************** end self doc **************************************)";

    fflush(stdout);
    fp = popen("more -p 1>&2", "w");
    fprintf(fp, "%s", sdoc.c_str());
    pclose(fp);
}





