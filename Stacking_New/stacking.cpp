/*
 * stacking.cpp
 *
 *  Created on: 17 Sept 2025
 *      Author: abhishek
 */

//V2.4

#include "stacking.h"
#include "utils.h"
#include "header_macros.h"

//Default constructor
	Stacking::Stacking():imagefile(""),velfile(""){
		vel_start_xline = 0;
		vel_start_inline = 0;
		vel_end_xline = 0;
		vel_end_inline = 0;
		heads = nullptr;
	}

Stacking::~Stacking() {
	// TODO Auto-generated destructor stub
	delete []heads;
	heads = nullptr;
}

const string& Stacking::getImagefile() const {
	return imagefile;
}

void Stacking::setImagefile(const string &imagefile) {
	this->imagefile = imagefile;
}

int Stacking::getVelEndInline() const {
	return vel_end_inline;
}

void Stacking::setVelEndInline(int velEndInline) {
	vel_end_inline = velEndInline;
}

int Stacking::getVelEndXline() const {
	return vel_end_xline;
}

void Stacking::setVelEndXline(int velEndXline) {
	vel_end_xline = velEndXline;
}

int Stacking::getVelStartInline() const {
	return vel_start_inline;
}

void Stacking::setVelStartInline(int velStartInline) {
	vel_start_inline = velStartInline;
}

int Stacking::getVelStartXline() const {
	return vel_start_xline;
}

void Stacking::setVelStartXline(int velStartXline) {
	vel_start_xline = velStartXline;
}

const string& Stacking::getVelfile() const {
	return velfile;
}

void Stacking::setVelfile(const string &velfile) {
	this->velfile = velfile;
}

void Stacking::setImgStackedinline(int imgStackedinline) {
	img_stackedinline = imgStackedinline / dx;

}

void Stacking::setImgStackedxline(int imgStackedxline) {
	img_stackedxline = imgStackedxline / dy;
}

void Stacking::loadvelheaders(std::ifstream& inpv){



	short ns, dt;
	int i, j;
	float *data;
	long hsize;

	char head[HDRBYTES];
	long int siz, nx, ny;

	nx = getVelEndXline() - getVelStartXline()  + 1;
	ny = getVelEndInline() - getVelStartInline()  + 1;

	hsize = HDRBYTES;
	hsize =  hsize * nx * ny;
	heads = new char[hsize];

	cout<<__LINE__<<endl;
	Utils::get_ns(inpv,ns);
	Utils::get_dt(inpv,dt);
	data = new float[ns];

	cout<<ny<<" "<<nx<<endl;
	for(i=0;i<ny;i++)
	{
	   for(j=0;j<nx;j++)
	   {
		   inpv.read(head,HDRBYTES);
		   inpv.read((char *)data,sizeof(float)*ns);
	       siz = 1;
           siz = siz * (i *nx + j);
           memcpy(&heads[siz*HDRBYTES], head, HDRBYTES);
        }
    }
	inpv.close();
    delete []data;
    data = NULL;

//    delete []heads;
 //   heads = NULL;

}

float Stacking::getDx() const {
	return dx;
}

void Stacking::setDx(float dx) {
	this->dx = dx;
}

float Stacking::getDy() const {
	return dy;
}

void Stacking::setDy(float dy) {
	this->dy = dy;
}

void Stacking::StackingImage() {
	short data_ns,data_dt;
	long data_ntrace,fsize;
	int startinline_data, startxline_data, endinline_data, endxline_data;
	int totalinline,totalxline;
	float *trace, **stackeddata, **stackedbdata;
	char head[HDRBYTES];
	int inln, xln, srcx, srcy, cdpx, cdpy, sx, k;
	int max_left,max_right,max_top,max_bottom;



	ifstream inpd(getImagefile().c_str(),ios::in | ios::binary);

	if(!inpd.is_open()){
			cerr<<"Error in opening Velocity file "<<getImagefile()<<endl;
			exit(0);
	}

	Utils::get_ns(inpd,data_ns);
	Utils::get_dt(inpd,data_dt);
	Utils::get_filesize(inpd, fsize);
	Utils::get_ntrace(fsize, data_ns, data_ntrace);
	Utils::get_min_max_in_xline_from_data(inpd, data_ntrace, startinline_data, startxline_data, endinline_data, endxline_data,max_left, max_right, max_top, max_bottom,getCentertype());




	if(startxline_data < getVelStartXline()) startxline_data = getVelStartXline();
	if(startinline_data < getVelStartInline()) startinline_data = getVelStartInline();


	if(endxline_data > getVelEndXline())  endxline_data = getVelEndXline();
	if(endinline_data > getVelEndInline()) endinline_data = getVelEndInline();

	cout<<"Stacking Area is "<<endl;
	cout<<"From X line  "<<startxline_data<<"  to "<<endxline_data<<endl;
	cout<<"From in line "<<startinline_data<<"  to "<<endinline_data<<endl;



	totalinline = endinline_data - startinline_data + 1;
	totalxline = endxline_data - startxline_data +  1;

	cout<<"endxline_data  : "<<endxline_data<<" "<<startxline_data<<endl;

	cout<<"Total Inline Points: "<<totalinline<<", Total xline Points : "<<totalxline<<endl;

	if(totalinline < 0 || totalxline < 0)
	{
		cout<<"Error!!! totalinline or totalxline can not be zero"<<endl;
		exit(0);
	}

	stackeddata =  new float*[totalinline * totalxline];
	stackedbdata =  new float*[totalinline * totalxline];

	for(int i=0 ; i < (totalinline * totalxline) ; i++)
	{
	    stackeddata[i] = new float[data_ns]();
	    stackedbdata[i] = new float[data_ns]();
	}

	for(int i = 0; i < (totalinline * totalxline); i++)
	{
	   memset(stackeddata[i],0x00,sizeof(float)*data_ns);
	   memset(stackedbdata[i],0x00,sizeof(float)*data_ns);
	}
	//cout<<totalinline<<" "<<totalxline<<" on line "<<__LINE__<<endl;
	//cout<<img_stackedinline<<" "<<img_stackedinline<<endl;


    

	if(getStackingtype() == NORMAL || getStackingtype() == MEAN)
	{
		//Normal Stacking
		Normal_Stacking(inpd,stackeddata, stackedbdata, data_ntrace,data_ns,startxline_data, startinline_data, endxline_data, endinline_data,totalxline,  totalinline);
		//MEAN Stacking
		if(getStackingtype() == MEAN){
			cout<<"Calling Mean stacking"<<endl;
			do_mean_stacked_data(stackeddata,stackedbdata,totalinline * totalxline,data_ns);
		}
	}
	else if(getStackingtype() == COSINE)
	{
        cout<<" Calling CoSINE "<<endl;
		Cosine_Stacking(inpd,stackeddata, stackedbdata, data_ntrace,data_ns,startxline_data, startinline_data, endxline_data, endinline_data,totalxline,  totalinline, data_dt, max_left, max_right, max_top, max_bottom);
	}
    

	inpd.close();

    writeStacking(stackeddata, stackedbdata, totalinline,totalxline,startinline_data, startxline_data, endinline_data,endxline_data, data_ns, data_dt);

    for(int i=0;i<(totalinline * totalxline);i++)
    {
        delete[] stackeddata[i];
        delete[] stackedbdata[i];
    }


    delete[] stackeddata;
    stackeddata = NULL;

    delete[] stackedbdata;
    stackedbdata = NULL;

}



void Stacking::writeStacking(float **stackeddata, float **stackedbdata, int totalinline, int totalxline, int startinline_data, int startxline_data, int endinline_data, int endxline_data, short data_ns, short data_dt){
	string ofile, obfile;
	int k;

	Utils::set_outputfile_name(getImagefile(), ofile, this,".sr");
	Utils::set_outputfile_name(getImagefile(), obfile, this,".bin");

	cout<<"Output SR File : "<<ofile<<endl;
	cout<<"Output BIN File : "<<obfile<<endl;

	ofstream outsr(ofile,ios::binary);
	ofstream outbin(obfile,ios::binary);

	    int egx, egy, x, y;
	    char head2[HDRBYTES];
	    for(int i = 0; i < totalinline ; i++)
	      {
	           for(int j=0 ; j< totalxline ; j++)
	           {

	               k = 1;
	               k = k * (i * totalxline + j);
		           x = j + startxline_data;
	               y = i + startinline_data;

	               x = x - getVelStartXline();
	               y = y - getVelStartInline();
	               y = y* (getVelEndXline() - getVelStartXline() +1) + x;
	               memset(head2,'\0',HDRBYTES);
	               memcpy(head2, &heads[y*HDRBYTES],  HDRBYTES);
	               egx = *((int*)            (head2 + XLINE));
	               egy = *((int*)            (head2 + INLINE));

	               if( egx != (j + startxline_data) || egy != (i + startinline_data)){
	                   cout<<"Error!! xline inline not matching "<<endl;
	                   cout<<egx<<" "<<egy<<" "<<startxline_data<<" "<<startinline_data<<endl;
	                   exit(0);
	               }

	               *((int*)            (head2 + SX)) = 0;
	               *((int*)            (head2 + SY)) = 0;
	               *((int*)            (head2 + FLDR)) = 0;

	               *((int*)            (head2 + TRACR)) = k+1;
	               *((int*)            (head2 + TRACL)) = k+1;
	               *((int*)            (head2 + TRACF)) = k+1;
	               *((short*)          (head2 + TRID)) = 2;
		               //*((int*)            (head2 + GX)) = j + startxline;
		               //*((int*)            (head2 + GY)) = i + startinline;
	               *((short*)          (head2 + DT)) =(short) data_dt;
	               *((short*)          (head2 + NS)) = (short) data_ns;
	               outsr.write(head2, HDRBYTES);
	               outsr.write((const char *) stackeddata[k], data_ns* sizeof(float));
	               outbin.write((const char *)stackedbdata[k], data_ns* sizeof(float));
	           }

		      }

		    outsr.close();
		    outbin.close();


            stackeddata = NULL;
            stackedbdata = NULL;

/*		       delete []stackeddata;
		       delete []stackedbdata;
		       stackeddata = NULL;
		       stackedbdata = NULL;  */

}

void Stacking::do_mean_stacked_data(float **stackeddata, float **stackedbdata,int n,short data_ns){
	for (int i=0;i<n;i++){
		for(int j=0;j<data_ns;j++){
			if(stackedbdata[i][j]!=0)
			stackeddata[i][j] = stackeddata[i][j] /stackedbdata[i][j] ;
		}
	}
    stackeddata = NULL;
    stackedbdata = NULL;
}

void Stacking::Normal_Stacking(ifstream &inpd,float **stackeddata, float **stackedbdata, int data_ntrace, short data_ns, int startxline_data, int startinline_data, int endxline_data, int endinline_data, int totalxline, int totalinline){

	int inln, xln, srcx, srcy, cdpx, cdpy, sx, k;
	char head[HDRBYTES];
	float *trace = new float[data_ns];
	int srcxcdp,srcycdp;
	if(getCentertype()==SHOTCENTER){
		srcxcdp  = SRCXCDP;
		srcycdp  = SRCYCDP;
		cout<<srcxcdp<<"NORMAL"<<srcycdp<<endl;
	}
	else{
		srcxcdp  = FOLDX;
		srcycdp  = FOLDY;
		cout<<srcxcdp<<"FOLD"<<srcycdp<<endl;
	}



	for(int i=0;i<data_ntrace;i++)
		{

		    inpd.read(head, HDRBYTES);
		    inpd.read((char *)trace, sizeof(float) *data_ns);

		    sx    = *((int*)  (head + SX));
		    xln   = *((int*)  (head + XLINE));
		    inln  = *((int*)  (head + INLINE));

		    srcx = *((int*)  (head + srcxcdp));
		    srcy = *((int*)  (head + srcycdp));




		    if( (abs(srcx - xln) <= img_stackedinline) && (xln >=startxline_data) && (xln <= endxline_data) &&  (abs(srcy - inln) <= img_stackedxline) &&(inln >=startinline_data) && (inln <= endinline_data) && (sx != 0))
		    {
		        xln = xln - startxline_data;
		        inln = inln - startinline_data;
		        k = 1;
		        k = k * ((inln * totalxline) + xln);


		        for(int j=0;j<data_ns;j++)
		        {
		            stackeddata[k][j] = stackeddata[k][j] + trace[j];
		            stackedbdata[k][j]++;
		        }
		     }
		     else if(sx == 0)
		     {
		        xln = xln - startxline_data;
		        inln = inln - startinline_data;
		        k = 1;
		        k = k * ((inln * totalxline) + xln);

		       for(int j=0;j<data_ns;j++)
		       {
		           stackeddata[k][j] = stackeddata[k][j] + trace[j];
		           stackedbdata[k][j]++;
		        }
		    }

	   }
    
    stackeddata = NULL;
    stackedbdata = NULL;
}

void Stacking::Cosine_Stacking(ifstream &inpd,float **stackeddata, float **stackedbdata, int data_ntrace, short data_ns, int startxline_data, int startinline_data, int endxline_data, int endinline_data, int totalxline,  int totalinline, short data_dt
		, int max_left,int max_right,int max_top, int max_bottom){

	int inln, xln, srcx, srcy, cdpx, cdpy, sx, k;
	float xln_dist, inl_dist,xln_inl_digonal_dist;
	char head[HDRBYTES];
	float *trace = new float[data_ns];
	float cosineAngle;
	float dx = getDx();
	float dy = getDy();
	float dz = data_dt/1000;
	long size,ysize,xsize,zsize;
	int dist_s_x,dist_s_y;

	size = 1;
	size = size * (max_left + max_right + 1) *  (max_top + max_bottom + 1) * data_ns;
	ysize = (max_top + max_bottom + 1);
	xsize = (max_left + max_right + 1);
	zsize = data_ns;

	float *stencil=new float[size];

	for(int y=0;y<ysize;y++){
		for(int x=0;x<xsize;x++){
			xln_dist = abs((max_left - x)*dx);
			inl_dist = abs((max_bottom - y)*dy);
			xln_inl_digonal_dist = sqrtf((xln_dist*xln_dist) + (inl_dist * inl_dist));
			for(int z=0;z<zsize;z++){
				cosineAngle = getDepthAngle(z,dx, dy, dz, xln_dist, inl_dist,xln_inl_digonal_dist);
				stencil[z + x*zsize + y*zsize*xsize]=cosf(cosineAngle);
			}
		}
	}

	ofstream outf("out.bin",ios::binary);
	outf.write((char *)stencil, size*sizeof(float));

	inpd.read(head, HDRBYTES);

	int srcxcdp, srcycdp;
	if (getCentertype() == SHOTCENTER) {
		srcxcdp = SRCXCDP;
		srcycdp = SRCYCDP;
	} else {
		srcxcdp = FOLDX;
		srcycdp = FOLDY;
	}



	/*if(srcx == 0 || srcy == 0)
	{
		cout<<"Error!! srcx  or srcy of trace is zero"<<endl;
		exit(0);
	} */
	inpd.seekg(0,std::ios::beg);


	for(int i=0;i<data_ntrace;i++)
	{
		inpd.read(head, HDRBYTES);
		inpd.read((char *)trace, sizeof(float) *data_ns);
		sx    = *((int*)  (head + SX));
		xln   = *((int*)  (head + XLINE));
		inln  = *((int*)  (head + INLINE));
		srcx  = *((int*)  (head + srcxcdp));
		srcy  = *((int*)  (head + srcycdp));

		if( (abs(srcx - xln) <= img_stackedinline) && (xln >=startxline_data) && (xln <= endxline_data) &&  (abs(srcy - inln) <= img_stackedxline) &&(inln >=startinline_data) && (inln <= endinline_data) && (sx != 0))
		{
			//xln, inln, srcx, srcy are parameter extracted from trace of image gather
			//max_left and max_bottom are index of source position in stencil
			//dist_s_x,dist_s_y are reference variable for getting position of trace wrt stencil
			//function extracting dist_s_x,dist_s_y
			Utils::getStencilIndex(xln,inln,srcx,srcy,max_left,max_bottom, dist_s_x,dist_s_y);
			//cout<<"xln  : "<<xln<<" inln : "<<inln<<" ";
			xln = xln - startxline_data;
			inln = inln - startinline_data;
			k = 1;
			k = k * ((inln * totalxline) + xln);
			//cout<<"totalxline : "<<totalxline<<endl;

			//cout<<"xln : "<<xln<<" inln : "<<inln<<" k : "<<k<<endl;
			for(int j=0;j<data_ns;j++)
			{
				//cout<<stencil[j+dist_s_x*zsize+dist_s_y*zsize*xsize]<<endl;
			    stackeddata[k][j] = stackeddata[k][j] + (trace[j]*stencil[j+dist_s_x*zsize+dist_s_y*zsize*xsize]);
			    stackedbdata[k][j]++;
			}
		}
	}

/*
	for(int i=0;i<data_ntrace;i++)
	{

	    inpd.read(head, HDRBYTES);
	    inpd.read((char *)trace, sizeof(float) *data_ns);
	    sx    = *((int*)  (head + SX));
	    xln   = *((int*)  (head + XLINE));
	    inln  = *((int*)  (head + INLINE));
	    srcx  = *((int*)  (head + SRCXCDP));
	    srcy  = *((int*)  (head + SRCYCDP));


	    if( (abs(srcx - xln) <= img_stackedinline) && (xln >=startxline_data) && (xln <= endxline_data) &&  (abs(srcy - inln) <= img_stackedxline) &&(inln >=startinline_data) && (inln <= endinline_data) && (sx != 0))
	    {


	        xln_dist = abs((srcx - xln)*dx);
	        inl_dist = abs((srcy - inln)*dy);
	        xln_inl_digonal_dist = sqrtf((xln_dist*xln_dist) + (inl_dist * inl_dist));



	        xln = xln - startxline_data;
	        inln = inln - startinline_data;
	        k = 1;
	        k = k * ((inln * totalxline) + xln);

	        for(int j=0;j<data_ns;j++)
	        {
	        	cosineAngle = getDepthAngle(j,dx, dy, dz, xln_dist, inl_dist,xln_inl_digonal_dist);
	            stackeddata[k][j] = stackeddata[k][j] + (trace[j]*cosf(cosineAngle));
	            stackedbdata[k][j]++;
	        }
			        //exit(0);
	     }
	     else if(sx == 0)
	     {
	        xln = xln - startxline_data;
	        inln = inln - startinline_data;
	        k = 1;
	        k = k * ((inln * totalxline) + xln);

	         for(int j=0;j<data_ns;j++)
		     {
		         stackeddata[k][j] = stackeddata[k][j] + trace[j];
		         stackedbdata[k][j]++;
		     }
		 }

	}*/
    stackeddata = NULL;
    stackedbdata = NULL;
}
inline float Stacking::getDepthAngle(int indx, float dx, float dy, float dz, float xln_dist, float inl_dist, float xln_inl_digonal_dist){
	float depth_dist,  angl_rad, angl_deg, xln_inl_digonal_dist_depth;
	float temp;


	depth_dist   = indx * dz;
	xln_inl_digonal_dist_depth = sqrtf((xln_inl_digonal_dist*xln_inl_digonal_dist) + (depth_dist * depth_dist));

	//cout<<"xln_inl_digonal_dist_depth : "<<xln_inl_digonal_dist_depth<<" xln_inl_digonal_dist : "<<xln_inl_digonal_dist<<" ";
	angl_rad = acosf(xln_inl_digonal_dist/(xln_inl_digonal_dist_depth+EPS));

	angl_rad = ACOS90 - abs(angl_rad);
	angl_deg = (angl_rad * 180)/PI;
	//cout<<"angl_deg : "<<angl_deg<<endl;
	return angl_rad;

}
