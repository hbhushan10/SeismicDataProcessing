This is Seismic data sorting Algorithm.
which build upon Intel TBB (Multithreading)
Multithreading parallely distributes tasks among threads which help to reduce execution time as compare with serial program
This program is featured with multi-key (on two keys that is primary key and secondary key) and order(ascending/descending) for sorting

   Following is steps for execution
   Step 1: First give execution permission to compile.sh file by  chmod +x compile.sh
   Step 2: Set compiler name mpiicpc or path 
   (you need to have intel compiler suite or intel oneAPI suite and gnu 8 or above).
   Already set for your reference
   Step 3: compile code with ./compile.sh
   Step 4: you will get seissort exe as output after compile successfully

   Step 5: run seissort with following command

   ->   seissort inputfile.su primary_key order secondary_key order


    Note: secondary_key and its order is optional
         You need input file in su format

    Here,
        order =  1 for sorting in ascending order
        order =  2 for sorting in descending order

        primary_key and secondary_key will be any from following

   ------key  ----  offset
        TRACL  0
        TRACR  4
        FLDR   8
        TRACF  12
        IMGP   16
        CDP    20
        CDPT   24
        TRID   28
        OFFSET 36
        GELEV  40
        SDEPTH 48
        SCALEL 68
        SCALCO 70
        SX     72
        SY     76
        GX     80
        GY     84
        COUNIT 88
        NS     114
        DT     116
        CDPX   180
        CDPY   184
        INLINE 188
        XLINE  192
        SRCYCDP 200
        SOFFSET 204
        SRCXCDP 208
        SRCYCDP 208 
 

    
