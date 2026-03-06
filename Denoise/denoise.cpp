/*
 * denoise.cpp
 *
 *  Created on: 04-Mar-2026
 *      Author: abhishek
 */

#include "denoise.h"
/**
 * @brief To create singlton obect
 * @return object created
 */
Denoise* Denoise::getInstance(){
	if(instance == nullptr)
		instance = new Denoise();
	return instance;

}
/**
 * @brief To get member image gather file name
 * @return filename
 */
const string& Denoise::getImageGatherFile() const {
	return image_gather_file;
}
/**
 * @brief To set image gather file name to member variable
 *
 * check file is present in folder or not.
 */
void Denoise::setImageGatherFile(const string &imageGatherFile) {
	image_gather_file = imageGatherFile;
	std::ifstream file(imageGatherFile.c_str());

	if (!file.good()) {
		std::cout << "File " << imageGatherFile << " does not exist exists\n";
		file.close();
		exit(0);
	}
	file.close();
}

/**
 * @brief To get member variable nx
 * @return nx
 */
int Denoise::getNx() const {
	return nx;
}

/**
 * @brief To set nx to member variable nx
 */
void Denoise::setNx(int nx) {
	this->nx = nx;
}

/**
 * @brief To get member variable ny
 * @return ny
 */
int Denoise::getNy() const {
	return ny;
}

/**
 * @brief To set ny to member variable ny
 */
void Denoise::setNy(int ny) {
	this->ny = ny;
}

/**
 * @brief To get member variable nz
 * @return nz
 */
int Denoise::getNz() const {
	return nz;
}

/**
 * @brief To get member variable dim_flag
 * @return dim_flag
 */
int Denoise::getDimFlag() const {
	return dim_flag;
}

/**
 * @brief To set dimention flag to member variable dim_flag
 *
 * validates dim_flag also
 *
 */
void Denoise::setDimFlag(int dimFlag) {
	dim_flag = dimFlag;
	if(dim_flag <0 || dim_flag >3){
		cout<<"Error!! Invalid dimension flag "<<endl;
		exit(0);
	}
}

/**
 * @brief To set nz to member variable nz
 */
void Denoise::setNz(int nz) {
	this->nz = nz;
}

/**
 * @brief Destructor of Denoise class
 */
Denoise::~Denoise() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief To get member variable dx
 * @return dx
 */
float Denoise::getDx() const {
	return dx;
}

/**
 * @brief To get member variable dz
 * @return dz
 */
float Denoise::getDz() const {
	return dz;
}

/**
 * @brief To get member variable output_image_gather_file
 * @return output_image_gather_file
 */
const string& Denoise::getOutputImageGatherFile() const {
	return output_image_gather_file;
}

/**
 * @brief To set member variable output_image_gather_file
 */
void Denoise::setOutputImageGatherFile(const string &outputImageGatherFile) {
	output_image_gather_file = outputImageGatherFile;
}

/**
 * @brief To set member variable dz
 */
void Denoise::setDz(float dz) {
	this->dz = dz;
}

/**
 * @brief To set member variable dx
 */
void Denoise::setDx(float dx) {
	this->dx = dx;
}
/**
 * @brief Apply 1D denoise filter in depth
 *
 *
 * @param Img Input image gather
 * @param denoise Output denoise image gather
 * @param nz is depth samples
 * @param nx is grid point in x direction
 */
void Denoise::denoise_filter_1D(float* &Img, float* &denoise,  int nz, int nx)
{
    int i1, i2, id;
    float diff1 = 0.0f;
    cout<<"nz : "<<nz<<" nx : "<<nx<<endl;
    for(i2=1; i2<nx-1; i2++)
        for(i1=1; i1<nz-1; i1++)
        {
            id  = i1 + i2*nz;
           // cout<<id<<endl;

          //  diff1 = Img[id+nz -1] + Img[id+nz +1] - Img[id-nz -1] + Img[id-nz +1];
            diff1   = (Img[id-1] + Img[id] + Img[id+1])/3;  //new
            denoise[id] = diff1;
        }
    cout<<__LINE__<<endl;
     expand(denoise,nx,nz);
}

/**
 * @brief write final filtered image gathers
 *
 *
 * @param filtshotdata output denoise image gather
 * @param heads copied heads from input image gather
 * @param ns is depth samples
 * @param trace_count total trace count
 * @param outputdataF ofstream object to write in final file
 */
void Denoise::write_filtered_image_gather(float* &filtshotdata,char* &heads,short ns, int trace_count,ofstream &outputdataF){
	char head[HDRBYTES];
	float *data;

	data = new float[ns];
	cout<<__LINE__<<endl;

	for(int i=0;i<trace_count; i++){
		memcpy(data,&filtshotdata[i*ns],ns*sizeof(float));
		memcpy(head,&heads[i*HDRBYTES],HDRBYTES);
		//memcpy(head,heads+i,HDRBYTES);
		outputdataF.write(head, HDRBYTES);
		outputdataF.write((char *)data, ns*sizeof(float));
	}

}

/**
 * @brief expand boundaries of filtered image gather
 *
 * denoising cannot applied on boundaries
 * Hence near traces copied to boundaries
 * Here we copying near 1 value, mean in each direction it become 2 (left+right or top+bottom)
 *
 * @param nnx is final nx
 * @param nny is final ny
 */
void Denoise::expand(float *b, int nnx, int nnz)
{
    int ix,iz;
    int nz1 = nnz-2;
    int nx1 = nnx-2;
    int npml=1;

    for(ix = 0; ix < nnx; ix++)
    {
        for (iz = 0; iz < npml; iz++)       b[ix*nnz+iz] = b[ix*nnz+1];  //top
        for (iz = nz1+npml; iz < nnz; iz++) b[ix*nnz+iz] = b[ix*nnz+npml+nz1-1];    //bottom
    }

    for(iz = 0; iz < nnz; iz++)
    {
        for(ix = 0; ix < npml; ix++)        b[ix*nnz+iz] = b[npml*nnz+iz];  //left
        for(ix = npml+nx1; ix < nnx; ix++)  b[ix*nnz+iz] = b[(npml+nx1-1)*nnz+iz];  //right
    }

}

/*
void denoise_filter_2D(float *Img, float *denoise, float _dz, float _dx, int ns, int nx)
{
    int i1, i2, id, id1, id2;
    float diff1 = 0.0f, diff2 = 0.0f;

    for(i2=1; i2<nx-1; i2++)
        for(i1=1; i1<ns-1; i1++)
        {
            id  = i1 + i2*ns;

            diff1 = Img[id+ns -1] + Img[id+ns +1] - Img[id-ns -1] + Img[id-ns +1];
            diff2 = Img[id+ns -1] + Img[id+ns +1] - Img[id-ns -1] + Img[id-ns +1];

            denoise[id] = _dz*diff1 + _dx*diff2;
        }

    //expand(denoise,nx,ns);
}*/

/**
 * @brief Apply denoising on image gathers fldr wise
 *
 * extract required parameters from image gather like ns, dz, single shot image gather
 *
 */
void Denoise::applyDenoise(){

	short int ns,dt;
	float *data, *shotdata, *filtshotdata;
	char  *heads;
	char head[HDRBYTES];
	int fldr,fldr_old, image_points, trace_count;

	ifstream inputdataF(image_gather_file);
	ofstream outputdataF(output_image_gather_file);

	Utils::get_ns(inputdataF, ns);
	cout<<"ns **: "<<ns<<endl;
	Utils::get_dt(inputdataF, dt);
	this->setDz((float)dt/1000);

	data = new float[ns];

	trace_count = 0;
	fldr_old    = -1;

	//Get imgGP from first trace into image_points
	Utils::get_imgGP_first_trace(inputdataF,image_points,fldr_old);
	//Allocate memory to required space
	shotdata     = new float[image_points*ns];
	filtshotdata = new float[image_points*ns];
	heads        = new char[image_points * HDRBYTES];

	while(inputdataF.read(head,HDRBYTES))
	{
		inputdataF.read((char *)data,ns*sizeof(float));
		fldr = *((int*)(head + FLDR));
		//Copy data unless fldr is same
		if(fldr_old == fldr){
			memcpy(&shotdata[trace_count*ns],data,ns*sizeof(float));
			memcpy(&heads[trace_count*HDRBYTES],head,HDRBYTES);
			trace_count++;
		}
		else{//once fldr is not same mean shot change
			if(dim_flag == 1){
				//call function for 1D denoise
				denoise_filter_1D(shotdata,filtshotdata,(int)ns,trace_count);
			}
			else{
				cout<<"Not Added for other flags"<<endl;
				exit(0);
			}
			//write final processed)denoised) shot image gather
			write_filtered_image_gather(filtshotdata,heads,ns,trace_count,outputdataF);
			//refresh all variable for next shot image gather
			fldr_old = fldr;
			trace_count = 0;
			delete[] shotdata;
			shotdata = NULL;
			delete[] filtshotdata;
			filtshotdata = NULL;
			delete[] heads;
			heads = NULL;

			image_points = *((int*)(head + IMGP));
			shotdata     = new float[image_points*ns];
			filtshotdata = new float[image_points*ns];
			heads        = new char[image_points * HDRBYTES];
			memcpy((void *)&shotdata[trace_count*ns],data,ns*sizeof(float));
			memcpy((void *)&heads[trace_count*HDRBYTES],head,HDRBYTES);
			trace_count++;
		}
	}

	if (dim_flag == 1) {
		denoise_filter_1D(shotdata, filtshotdata, (int) ns, trace_count);
	} else {
		cout << "Not Added for other flags" << endl;
		exit(0);
	}
	write_filtered_image_gather(filtshotdata,heads,ns,trace_count,outputdataF);
	outputdataF.close();

	delete[] shotdata;
	shotdata = NULL;
	delete[] filtshotdata;
	filtshotdata = NULL;
	delete[] heads;
	heads = NULL;


	delete[] data;
	data = NULL;


}
