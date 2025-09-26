#include "stacking.h"
#include "utils.h"

char* Utils::heads = nullptr;
int main(int argc, char **argv)
{

	if(argc != 9)
	{
	     Utils::selfdoc();
	     exit(0);
	}


	Stacking *stk;
	stk = new Stacking();
	long hsize;

	stk->setVelfile(string(argv[1]));
	stk->setImagefile(string(argv[2]));

	stk->setDx(atof(argv[5]));
	stk->setDy(atof(argv[6]));
	stk->setImgStackedinline(atoi(argv[3]));
	stk->setImgStackedxline(atoi(argv[4]));
	stk->setCentertype(atoi(argv[7]));
	stk->setStackingtype(atoi(argv[8]));





	ifstream inpv(stk->getVelfile().c_str(),ios::in | ios::binary);

	if(!inpv.is_open()){
		cerr<<"Error in opening Velocity file "<<stk->getVelfile()<<endl;
		exit(0);
	}

	Utils::get_start_inline_xline_from_vel(inpv,stk);


	cout<<"Velocity File      : "<<stk->getVelfile()<<endl;
	cout<<"Image Gather File  : "<<stk->getVelfile()<<endl;
	cout<<"Inline stacking offset(Full) : "<<argv[3]<<endl;
	cout<<"Xline stacking offset(Full) : "<<argv[4]<<endl;

	cout<<"Parameters extracted from velocity model"<<endl;
	cout<<"start xline   : "<<stk->getVelStartXline()<<endl;
	cout<<"start inline  : "<<stk->getVelStartInline()<<endl;
	cout<<"end   xline   : "<<stk->getVelEndXline()<<endl;
	cout<<"end   inline  : "<<stk->getVelEndInline()<<endl;




	inpv.clear();
	inpv.seekg(0,std::ios::beg);

	stk->loadvelheaders(inpv);


	stk->StackingImage();






}
