#include<iostream>
#include "utils.h"
#include "denoise.h"
using namespace std;


// Initialize static pointer
Denoise* Denoise::instance = nullptr;
int main(int argc, char **argv)
{
	//V1.3
	if (argc != 4) {
		Utils::selfdoc();
		exit(0);
	}

	Denoise *denoise = Denoise::getInstance();///< create singleton object of Denoise class

	denoise->setImageGatherFile(string(argv[1]));
	denoise->setDimFlag(atoi(argv[2]));
	denoise->setDx(atoi(argv[3]));

	denoise->setOutputImageGatherFile(Utils::set_outputfile_name(denoise->getImageGatherFile(), denoise->getDimFlag(), ".sr"));

	cout<<"Output File Name : "<<denoise->getOutputImageGatherFile()<<endl;

	denoise->applyDenoise();





}
