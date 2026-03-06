/*
 * denoise.h
 *
 *  Created on: 04-Mar-2026
 *      Author: abhishek
 */

#ifndef DENOISE_H_
#define DENOISE_H_

#include<iostream>
#include<fstream>
#include <cstring>
#include "utils.h"
using namespace std;
/**
 * @brief Denoise class perform denoising operation
 *
 * Denoise filter reduce high frequency noise from data
 * we can apply 1D,2D or 3D filter on data
 */

class Denoise {
private:
	string image_gather_file;
	string output_image_gather_file;
	int nz, nx, ny;
	int dim_flag;
	float dx,dz;

	static Denoise* instance;
	Denoise(){
		nx = ny = nz = dim_flag = 0;
		dx = dz = 0.0f;
		image_gather_file = output_image_gather_file = nullptr;


	}



public:
	static Denoise* getInstance();
	Denoise(const Denoise&) = delete;
	Denoise& operator=(const Denoise&) = delete;
	virtual ~Denoise();
	const string& getImageGatherFile() const;
	void setImageGatherFile(const string &imageGatherFile);
	int getNx() const;
	void setNx(int nx);
	int getNy() const;
	void setNy(int ny);
	int getNz() const;
	void setNz(int nz);
	int getDimFlag() const;
	void setDimFlag(int dimFlag);
	void Denoise_1D();
	void applyDenoise();
	void denoise_filter_1D(float* &Img, float* &denoise,  int nz, int nx);
	void expand(float *b, int nnx, int nnz);
	void write_filtered_image_gather(float* &filtshotdata,char* &heads,short ns, int trace_count,ofstream &outputF);

	float getDx() const;
	void setDx(float dx);
	float getDz() const;
	void setDz(float dz);
	const string& getOutputImageGatherFile() const;
	void setOutputImageGatherFile(const string &outputImageGatherFile);
};

#endif /* DENOISE_H_ */
