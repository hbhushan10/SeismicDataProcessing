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

void Utils::get_min_max_in_xline_from_data(ifstream &inpd,long ntrace,int &min_in, int &min_x, int &max_in, int &max_x,
	int &max_left, int &max_right, int &max_top, int &max_bottom, int Centertype){

	int  xln, inln,srcx,srcy;
	char head[HDRBYTES];
	short ns, dt;
	Utils::get_ns(inpd,ns);
	Utils::get_dt(inpd,dt);
	long i;
	int dist;



	float *trace = new float[ns];

	min_in = INT_MAX;
	min_x = INT_MAX;
	max_in = INT_MIN;
	max_x = INT_MIN;

    int srcxcdp, srcycdp;
    if (Centertype == SHOTCENTER) {
        srcxcdp = SRCXCDP;
        srcycdp = SRCYCDP;
        cout << srcxcdp << "NORMAL" << srcycdp << endl;
    } else {
        srcxcdp = FOLDX;
        srcycdp = FOLDY;
        cout << srcxcdp << "FOLD" << srcycdp << endl;
    }


	max_left = max_right = max_top = max_bottom = 0;


	for(i=0;i<ntrace;i++)
	    {
			inpd.read(head, HDRBYTES);
			inpd.read((char*)trace, sizeof(float)* ns);
	        xln  = *((int*)  (head + XLINE));
	        inln = *((int*)  (head + INLINE));
	        srcx = *((int*)  (head + srcxcdp));
	        srcy = *((int*)  (head + srcycdp));

	        if(xln < min_x)
	            min_x = xln;
	        if(xln > max_x)
	            max_x = xln;

	        if(inln < min_in)
	            min_in = inln;
	        if(inln > max_in)
	            max_in = inln;

	        dist = xln - srcx;
	        if(dist>1 && dist>max_right)
	        	max_right = dist;
	        else if(dist<1 && abs(dist)>max_left)
	        	max_left = abs(dist);

	        dist = inln - srcy;
	        if(dist>1 && dist>max_top)
	        	max_top = dist;
	        else if(dist<1 && abs(dist)>max_bottom)
	        	max_bottom = abs(dist);
	    }

	inpd.clear();
	inpd.seekg(0,std::ios::beg);

	cout<<"max_left : "<<max_left<<",  max_right : "<<max_right<<" max_top : "<<max_top<<" max_bottom : "<<max_bottom<<endl;


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

	if(stk->getCentertype()==SHOTCENTER)
		ofile.append("_SHOTC");
	else if(stk->getCentertype()==FOLDCENTER)
		ofile.append("_FOLDC");
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
						    Center:  1- Shot Centric stacking, 2- Fold Centric stacking 
							Type of Stacking: 1-  Normal,  2- Mean, 3- Cosine Tapering 

    "**************** end self doc **************************************)";

    fflush(stdout);
    fp = popen("more -p 1>&2", "w");
    fprintf(fp, "%s", sdoc.c_str());
    pclose(fp);
}
//function extracting a trace position wrt to stencil and hold index value in dist_s_x and dist_s_y(referece variables)
void Utils::getStencilIndex(int xln,int inln,int srcx,int srcy,int stencil_sx_index,int stencil_sy_index, int &dist_stencil_x,int &dist_stencil_y){

	int distx = xln -srcx;  //get relative position in x
	int disty = inln - srcy; //get relative position in y


	if (distx < 1) {  //if distx is negative, mean xln is left of srcx
		dist_stencil_x = stencil_sx_index - abs(distx);
	} else if (distx > 1) {//if distx is positive, mean xln is right of srcx
		dist_stencil_x = stencil_sx_index + distx;
	}
	else{
		dist_stencil_x = stencil_sx_index;
	}

	if (disty < 1) {
		dist_stencil_y = stencil_sy_index - abs(disty);
	} else if (distx > 1) {
		dist_stencil_y = stencil_sy_index + disty;
	}
	else{
		dist_stencil_y = stencil_sy_index;
	}


}






