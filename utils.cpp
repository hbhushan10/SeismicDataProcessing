#include<iostream>
#include<fstream>
#include <algorithm>
#include <string>
#include <string.h>
#include <libgen.h>
#include <mpi.h>
#include "header_srsort.h"
#include <tbb/tbb.h>
#include <tbb/parallel_sort.h>
using namespace std;
#define HDRBYTE 240


void set_outputfile_name(string file, string &outfile, char *postfix)
{
    char tmp[1024], t[1024];
    char ofile[1024];
    char *loc, *loc1, *loc2;

    strcpy(tmp, file.c_str());

    strcpy(ofile,dirname(tmp));
    strcat(ofile,"/");

    strcpy(tmp, file.c_str());

    loc = strrchr(tmp, '/');
    if(loc == NULL)
    {
        loc = tmp;
        loc1 = strrchr(loc, '.');
        strncpy(loc1, "\0", 1);
        strcat(ofile, loc);
    }
    else
    {
        loc1 = strrchr(loc, '.');
        strncpy(loc1, "\0", 1);
        strcat(ofile, (loc+1));
    }

    strcat(ofile, "_");
    strcat(ofile, postfix);
    strcat(ofile, ".sr");
    outfile.assign(ofile);
}

void get_filesize(ifstream& inp, long int& size)
{
    inp.seekg(0,ios::end);
    size = inp.tellg();
    inp.seekg(0,ios::beg);

}

void get_ns(ifstream& inp, short int& ns)
{
    char head[HDRBYTE];
    inp.read(head,HDRBYTE);
    ns = *((short int*)(head + 114));
}

void get_ntrace(long int size, int ns, long int& ntrace)
{
    ntrace = size / (HDRBYTE + (ns*sizeof(float)));
}



void Keys::createMap()
{
        hdroff.insert({"TRACL",0});
        hdroff.insert({"TRACR",4});
        hdroff.insert({"FLDR",   8});
        hdroff.insert({"TRACF",  12});
        hdroff.insert({"IMGP",   16});
        hdroff.insert({"CDP",    20});
        hdroff.insert({"CDPT",  24});
        hdroff.insert({"TRID", 28});
        hdroff.insert({"OFFSET", 36});
        hdroff.insert({"GELEV",  40});
        hdroff.insert({"SDEPTH", 48});
        hdroff.insert({"SCALEL", 68});
        hdroff.insert({"SCALCO", 70});
        hdroff.insert({"SX",     72});
        hdroff.insert({"SY",     76});
        hdroff.insert({"GX",     80});
        hdroff.insert({"GY",     84});
        hdroff.insert({"COUNIT", 88});
        hdroff.insert({"NS",     114});
        hdroff.insert({"DT",     116});
        hdroff.insert({"CDPX",   180});
        hdroff.insert({"CDPY",   184});
        hdroff.insert({"INLINE", 188});
        hdroff.insert({"XLINE",  192});
        hdroff.insert({"SRCYCDP", 200});
        hdroff.insert({"SOFFSET", 204});
        hdroff.insert({"SRCXCDP", 208});
        hdroff.insert({"SRCCDP", 208});
        hdroff.insert({"GEOMTYPE", 212});
        hdroff.insert({"D2",     232});
        hdroff.insert({"D3",     236});
}

int Keys::getoffset(string keyname)
{
        if(hdroff.find(keyname) != hdroff.end())
        {
            return hdroff[keyname];
        }
        return -1;
}


void Keys::addkeys(int i, int key1, int key2)
{
        index[i] = i;
        vect1[i] = key1;
        vect2[i] = key2;
}

vector<int> Keys::Sort(Keys *arr, int n, int order1, int order2)
    {
        IntArrays *arrays = new IntArrays(arr->index, arr->vect1, arr->vect2,n);
    //    arrays.printArrays();
    cout<<__LINE__<<endl;
    arrays->sortBasedOnPrimaryAndSecondary(order1,order2);
    
    vector<int> loc = std::move(arrays->indexArray);
    delete arrays;
       // std::cout << "After sorting:" << std::endl;
          //  arrays.printArrays();
     return loc;
    }




    // Method to sort based on the primary array, using the secondary array for tie-breaking
 void IntArrays::sortBasedOnPrimaryAndSecondary(int order1, int order2) {


	// Create a vector of indices
        std::vector<size_t> indices(primaryArray.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }

        if(order1 == 2 && order2 == 2)
        {
            // Sort the indices based on the values in primaryArray and secondaryArray
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] > secondaryArray[i2];
                }
                return primaryArray[i1] > primaryArray[i2];
            });
        }


        else if(order1 == 1 && order2 == 1)
        {
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] < secondaryArray[i2];
                }
                return primaryArray[i1] < primaryArray[i2];
            });
        }


        else if(order1 == 2 && order2 == 1)
        {
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] < secondaryArray[i2];
                }
                return primaryArray[i1] > primaryArray[i2];
            });
        }        
        else if(order1 == 1 && order2 == 2)
        {
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] > secondaryArray[i2];
                }
                return primaryArray[i1] < primaryArray[i2];
            });
        }        


        else if(order1 == 1 && order2 == -1)
        {
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] > secondaryArray[i2];
                }
                return primaryArray[i1] < primaryArray[i2];
            });
        }

        else if(order1 == 2 && order2 == -1)
        {
            std::stable_sort(indices.begin(), indices.end(), [this](size_t i1, size_t i2) {
                if (primaryArray[i1] == primaryArray[i2]) {
                    return secondaryArray[i1] > secondaryArray[i2];
                }
                return primaryArray[i1] > primaryArray[i2];
            });
        }
 


        // Create sorted versions of the primary and secondary arrays
        std::vector<int> sortedindex(indexArray.size());
        std::vector<int> sortedPrimary(primaryArray.size());
        std::vector<int> sortedSecondary(secondaryArray.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            sortedindex[i] = indexArray[indices[i]];
            sortedPrimary[i] = primaryArray[indices[i]];
            //cout<<"sortedPrimary[i] : "<<sortedPrimary[i]<<endl;
            sortedSecondary[i] = secondaryArray[indices[i]];
        }

        // Replace the original arrays with the sorted arrays
        primaryArray = std::move(sortedPrimary);
        secondaryArray = std::move(sortedSecondary);
        indexArray = std::move(sortedindex);


}
    // Method to print the arrays
void IntArrays::printArrays() const {
        //std::cout << "Index Array: ";
        /*for ( auto val = indexArray.begin(),   val2 =  primaryArray.begin(), val3 = secondaryArray.begin(); val != indexArray.end(),   val2 !=  primaryArray.end(), val3 != secondaryArray.end(); ++val,++val2,++val3) {
            std::cout << *val << " "<<*val2<<" "<<*val3<<endl;
        }*/
        std::cout << std::endl; 

        std::cout << "Primary Array: ";
        cout<<"Size  : "<<primaryArray.size()<<endl;
        for (const auto& val : primaryArray) {
            std::cout << val << " "<<endl;
        }
        std::cout << std::endl; 
/*
        std::cout << "Secondary Array: ";
        for (const auto& val : secondaryArray) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
*/
}
