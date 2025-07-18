#include<iostream>
#include<fstream>
#include <algorithm>
#include<map>
#include<vector>
using namespace std;

void selfdoc();
void set_outputfile_name(string file, string &ofile, char *postfix);
void get_filesize(ifstream& inp, long int& size);
void get_ns(ifstream& inp, short int& ns);
void get_ntrace(long int size, int ns, long int& ntrace);

class IntArrays {
public:
    std::vector<int> indexArray;
    std::vector<int> primaryArray;
    std::vector<int> secondaryArray;

public:
    // Constructor to initialize the arrays
    IntArrays(int *index, int *primary, int *secondary, int n)
    {
        this->indexArray.reserve(n);
        this->primaryArray.reserve(n);
        this->secondaryArray.reserve(n);
    
        copy(index, index + n , std::back_inserter(this->indexArray));
        copy(primary, primary + n , std::back_inserter(this->primaryArray));
        copy(secondary, secondary + n , std::back_inserter(this->secondaryArray));
    }

    // Method to sort based on the primary array, using the secondary array for tie-breaking
    void sortBasedOnPrimaryAndSecondary(int order1, int order2);
    void printArrays() const;
};
class Keys
{
    public:
    int *index;
    int *vect1;
    int *vect2;
    
    Keys(long int ntrace)
    {
        index = new int [ntrace];
        vect1 = new int [ntrace];
        vect2 = new int [ntrace];
    }

    static map<string,int>hdroff;

    void addkeys(int index, int key1, int key2);
    static void createMap();
    static int getoffset(string keyname);
    
    static vector<int> Sort(Keys *arr, int n, int order1, int order2);
};



