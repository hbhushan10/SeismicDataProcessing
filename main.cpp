#include<iostream>
#include<fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include<thread>
#include <mpi.h>
#include <tbb/tbb.h>
#include <tbb/task_arena.h>
#include "header_srsort.h"
using namespace std;
#define HDRBYTE 240
#include <omp.h>

map<string,int> Keys::hdroff;

tbb::spin_mutex mtx; 

void write_chunk(std::ifstream &inp, std::ofstream &otp, std::mutex &file_mutex, size_t start, size_t end) {
    std::lock_guard<std::mutex> guard(file_mutex);
    //file.seekp(start);
    //file.write(data.data() + start, end - start);
}



int main(int argc , char **argv)
{

    char head[HDRBYTE];
    string outputfile;
    short int ns;
    long int ntrace,size, file_size;
    int key1value, key2value, cores;    
    long int i, j;
     string type1flag, type2flag;
    char postfix[256];   
    double t1,t2,t3,t4; 
    long int chunk_traces = 0, chunk_size = 0; 


    MPI_Init(&argc, &argv);

    long int  start_off;
    int start_index;


    string key1name,key2name;
    int order1,order2;  //1 mean ascending order, 2 mean descending order
    order1 = order2 = -1;
    if(argc == 6)
    {
        key1name.assign(argv[2]);
        order1 = atoi(argv[3]);
        if(order1 != 1 && order1 !=2)
        {
            cout<<"Error!!! Invalid order for Key1 "<<key1name<<", enter 1 for ascending order and 2 for descending order "<<endl;
            exit(0);
        }
        key2name.assign(argv[4]);
        order2 = atoi(argv[5]);
        if(order2 != 1 && order2 !=2)
        {
            cout<<"Error!!! Invalid order for Key2 "<<key2name<<", enter 1 for ascending order and 2 for descending order "<<endl;
            exit(0);
        } 
    }    

    else if(argc == 4)
    {
        key1name.assign(argv[2]);
        order1 = atoi(argv[3]);
        if(order1 != 1 && order1 !=2)
        {
            cout<<"Error!!! Invalid order for Key1 "<<key1name<<", enter 1 for ascending order and 2 for descending order "<<endl;
            exit(0);
        }
        /*key2name.assign(argv[4]);
        order2 = atoi(argv[5]);
        if(order2 != 1 && order2 !=2)
        {
            cout<<"Error!!! Invalid order for Key2 "<<key2name<<", enter 1 for ascending order and 2 for descending order "<<endl;
            exit(0);
        }*/
    }
    else
    {
        selfdoc();
        exit(0);
    }



    string inputfile (argv[1]); /* "/home/abhishek/Documents/Bhushan/Marmousi_vel_model_org_Modified-cdpx_Modified-cdpy.sr_resampled_xfact2_zfact2.sr"*/
    
    if(argc == 6)
        sprintf(postfix,"Sorted_Primary_%s_Secondary_%s",argv[2],argv[4]);
    else if(argc == 4)
        sprintf(postfix,"Sorted_Primary_%s",argv[2]);

    set_outputfile_name(inputfile, outputfile, postfix);

    ifstream inp(inputfile,ios::binary);
    ofstream otp(outputfile,ios::binary);
    


    if(!inp.is_open())
    {
        cout<<"Unable to open file "<<inputfile<<endl;
        return 0;
    }
    
    get_filesize(inp,file_size);
    
    
    get_ns(inp,ns);
    cout<<"Input File                        : "<<inputfile<<endl;
    cout<<"Number of Samples in Data (ns)    : "<<ns<<endl; 
    
    get_ntrace(file_size,ns,ntrace);    
    cout<<"Number of Traces in Data (ntrace) : "<<ntrace<<endl; 
   
    const size_t num_cores = tbb::this_task_arena::max_concurrency();
    cout<<"Number of Cores : "<<num_cores<<endl;

    omp_set_num_threads(num_cores);

    chunk_traces = ntrace/num_cores + 1; // Size of each chunk (1 MB)
    chunk_size =   chunk_traces * (HDRBYTE + (ns *sizeof(float)));


    //SortKeys *array = new SortKeys[ntrace];

    transform(key1name.begin(), key1name.end(), key1name.begin(),[](unsigned char c){ return toupper(c); });   
    transform(key2name.begin(), key2name.end(), key2name.begin(),[](unsigned char c){ return toupper(c); });
    Keys::createMap();


    cout<<Keys::hdroff.size()<<endl;
     
    int off1 = Keys::getoffset(key1name);
    int off2 = Keys::getoffset(key2name);
    
    cout<<"key1 Name : "<<key1name<<", Order : "<<(order1 == 1 ? "Ascending": "Descending")<<", Offset : "<<off1<<endl;
    cout<<"key2 Name : "<<key2name<<", Order : "<<(order2 == 1 ? "Ascending": "Descending")<<", Offset : "<<off2<<endl;

    if(off1 == -1)
    {
        cout<<"Error!! Invalid Key1"<<endl;
        exit(0);
    }

    if(off2 == -1)
    {
        cout<<"Warning!! Invalid Key2 or not present"<<endl;
        
    }

    Keys *keys = new Keys(ntrace);


    inp.seekg(0,ios::beg);
    vector<float> floatArray(ns);

    //char data[ns*4];
    float *data =new float[ns];

    t1 = MPI_Wtime();
    tbb::task_group group;


    if(off2 == -1) //for single key
    {
        for(std::size_t i = 0 ;i<num_cores;i++)
        {
            group.run([i,start_off,size,&inputfile, ns,chunk_traces,chunk_size,file_size,&keys,off1](){
                char head[HDRBYTE];

                int thread_index = tbb::this_task_arena::current_thread_index();

                float *data =new float[ns];
                int key1;
                long int start_index = i * chunk_traces;
                long int start_off = i * chunk_size;
                long int size =   std::min(chunk_size, (file_size - start_off));

                std::ifstream thread_file(inputfile, std::ios::binary);
                thread_file.seekg(start_off, std::ios::beg);

                long int k = start_index;

                while(thread_file.read(head,HDRBYTE) &&  k < (start_index+chunk_traces))
                {
                    thread_file.read((char*) data,ns*sizeof(float));
                    key1 = *((int*)(head + off1));
                    keys->addkeys(k,key1,0);
                    k++;
                }
                thread_file.close();

            });
        
        } //end of for  

        group.wait();

    }  //end of if
    else  //for two keys
    {
        for(std::size_t i = 0 ;i<num_cores;i++)
        {
            group.run([i,start_off,size,&inputfile, ns,chunk_traces,chunk_size,file_size,&keys,off1,off2](){
                char head[HDRBYTE];

                int thread_index = tbb::this_task_arena::current_thread_index();

                float *data =new float[ns];
                int key1, key2;
                long int start_index = i * chunk_traces;
                long int start_off = i * chunk_size;
                long int size =   std::min(chunk_size, (file_size - start_off));

                std::ifstream thread_file(inputfile, std::ios::binary);
                thread_file.seekg(start_off, std::ios::beg);

                long int k = start_index;

                while(thread_file.read(head,HDRBYTE) &&  k < (start_index+chunk_traces))
                {
                    thread_file.read((char*) data,ns*sizeof(float));
                    key1 = *((int*)(head + off1));
                    key2 = *((int*)(head + off2));
                    keys->addkeys(k,key1,key2);
                    k++;
                }
                thread_file.close();

            });

        }//end of for
        group.wait();
    } //end of else
    
    

    t2 = MPI_Wtime();
    cout<<"Read Data takes "<<((t2-t1)/60)<<" mins"<<endl;

    cout<<"order : "<<order1<<" order2 : "<<order2<<endl;
    //Sort on the basis of key1
    vector<int> indexvector = Keys::Sort(keys,ntrace,order1,order2); 

    delete keys;

    t3 = MPI_Wtime();
    cout<<"Sort Header values takes "<<((t3-t2)/60)<<" mins"<<endl;    

    cout<<"Parallel Sorting COmpleted"<<endl;

    inp.seekg(0,ios::beg); 


    cout<<"ns : "<<ns<<endl;

   for(std::size_t i = 0 ;i<num_cores;i++)
   {

        group.run([i,start_off,size,&inputfile, ns,chunk_traces,chunk_size,file_size,&keys,off1, indexvector,ntrace, &inp, &otp](){
            char head[HDRBYTE];

            int thread_index = tbb::this_task_arena::current_thread_index();

            float *d =new float[ns];

            long int start_index = i * chunk_traces;
            long int end_index = (start_index + chunk_traces) < ntrace ? start_index + chunk_traces : ntrace;


            std::ifstream thread_file(inputfile, std::ios::binary);
            thread_file.seekg(start_off, std::ios::beg);


            long int k = start_index;
            long int j;
            long int index;
            for(j=start_index;j<end_index;j++)
            {
                index = indexvector[j];
                thread_file.seekg(((HDRBYTE+(ns*sizeof(float)))*index),ios::beg);
                thread_file.read(head,HDRBYTE);
                thread_file.read((char *)d,ns*sizeof(float));

                tbb::spin_mutex::scoped_lock lock(mtx);
                {
                    otp.seekp(((HDRBYTE+(ns*sizeof(float)))*j),ios::beg);
                    otp.write(head,HDRBYTE);
                    otp.write((char*)d,ns*sizeof(float));                
                } 
            }
            thread_file.close();

        });

    }
    group.wait();
    cout<<"Program Ended"<<endl;





    t4 = MPI_Wtime();

    cout<<"Total Completation Time is  "<<((t4-t1)/60)<<" mins"<<endl;

    inp.close();
    otp.close(); 
    cout<<"outputfile : "<<outputfile<<endl;
    cout<<"##############Sorting COmpleted Successfully###############"<<endl;

}



