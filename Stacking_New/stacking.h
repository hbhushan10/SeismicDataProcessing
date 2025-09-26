/*
 * stacking.h
 *
 *  Created on: 17 Sept 2025
 *      Author: abhishek
 */

#ifndef STACKING_H_
#define STACKING_H_
#include<iostream>
#include<fstream>
#include <cstring>
#include <math.h>
#include <cmath>
#include <limits>

using namespace std;

#define HDRBYTES 240
#define NORMAL 1
#define MEAN 2
#define COSINE 3
#define PI 3.14f
const double EPS = 1e-9;
#define ACOS90 1.57079633f


class Stacking {
	string imagefile;
	string velfile;
	char *heads;
	int vel_start_xline, vel_start_inline, vel_end_xline, vel_end_inline;
	int img_stackedinline, img_stackedxline;
	float dx, dy;
	int centertype, stackingtype;

public:

	//Default constructor
	Stacking();
	virtual ~Stacking();
	const string& getImagefile() const;
	void setImagefile(const string &imagefile);
	int getVelEndInline() const;
	void setVelEndInline(int velEndInline);
	int getVelEndXline() const;
	void setVelEndXline(int velEndXline);
	int getVelStartInline() const;
	void setVelStartInline(int velStartInline);
	int getVelStartXline() const;
	void setVelStartXline(int velStartXline);
	const string& getVelfile() const;
	void setVelfile(const string &velfile);
	void loadvelheaders(std::ifstream& inpv);
	void setImgStackedinline(int imgStackedinline);
	void setImgStackedxline(int imgStackedxline);
	float getDx() const;
	void setDx(float dx);
	float getDy() const;
	void setDy(float dy);
	void StackingImage();
	void writeStacking(float **stackeddata, float **stackedbdata, int totalinline, int totalxline, int startinline_data, int startxline_data, int endinline_data, int endxline_data, short data_ns, short data_dt);
	void do_mean_stacked_data(float **stackeddata, float **stackedbdata,int n,short data_ns);
	void Normal_Stacking(ifstream &inpd,float **stackeddata, float **stackedbdata, int data_ntrace, short data_ns, int startxline_data, int startinline_data, int endxline_data, int endinline_data, int totalxline,  int totalinline);
	void Cosine_Stacking(ifstream &inpd,float **stackeddata, float **stackedbdata, int data_ntrace, short data_ns, int startxline_data, int startinline_data, int endxline_data, int endinline_data, int totalxline,  int totalinline, short data_dt);
	inline float getDepthAngle(int indx, float dx, float dy, float dz, float xln_dist, float inl_dist, float xln_inl_digonal_dist);
	int getImgStackedinline() const {
		return img_stackedinline;
	}

	int getImgStackedxline() const {
		return img_stackedxline;
	}

	int getCentertype() const {
		return centertype;
	}

	void setCentertype(int centertype) {
		this->centertype = centertype;
	}

	int getStackingtype() const {
		return stackingtype;
	}

	void setStackingtype(int stackingtype) {
		this->stackingtype = stackingtype;
	}
};

#endif /* STACKING_H_ */
