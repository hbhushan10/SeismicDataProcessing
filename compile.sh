#Here mpiicpc is name of compiler
#-ltbb is flag to enable intel TBB
 


mpiicpc  -ltbb -fopenmp  -o seissort main.cpp selfdoc.cpp utils.cpp -std=c++17
