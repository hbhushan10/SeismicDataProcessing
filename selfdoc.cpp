#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "header_srsort.h"

//using namespace std;



string sdoc =
R"(SEISSORT- Sort seismic data based on maximum two keys(Primary key and Secondary key)

    Syntax --> seissort inputfile primary_key order secondary_key order
    Note: secondary_key and its order is optional   
 
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

**************** end self doc **************************************)";


void selfdoc(void)
{
    FILE *fp;

    fflush(stdout);
    fp = popen("more -p 1>&2", "w");
    fprintf(fp, "%s", sdoc.c_str());
    pclose(fp);
}
