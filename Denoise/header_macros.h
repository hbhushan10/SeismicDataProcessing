/*
 * header_macros.h
 *
 *  Created on: 17 Sept 2025
 *      Author: abhishek
 */

#ifndef HEADER_MACROS_H_
#define HEADER_MACROS_H_


#define TRACL  0
#define TRACR  4
#define FLDR   8
#define TRACF  12
#define IMGP   16
#define CDP    20
#define CDPT   24
#define TRID   28
#define OFFSET 36
#define GELEV  40
#define SDEPTH 48
#define SCALEL 68
#define SCALCO 70
#define SX     72
#define SY     76
#define GX     80
#define GY     84
#define COUNIT 88
#define NS     114
#define DT     116
#define CDPX   180
#define CDPY   184
#define INLINE 188
#define XLINE  192
#define SRCYCDP 200
#define SOFFSET 204
#define SRCXCDP 208
#define SRCCDP 208
#define GEOMTYPE 212
#define D2     232
#define D3     236
#define FOLDX   224
#define FOLDY   228



#define DXFACT 218
#define DYFACT 220
#define DZFACT 222


//Following are SEGY Binary header information
#define B_INLINE 04   //05 to 08 4 bytes
#define B_TRACESPERINLINE 12 //13 to 14
#define B_DT 16 // 17 to 18
#define B_NS 20 //21 to 22
#define B_DATAFORMAT 24 //25 to 26
#define B_UNIT 54 //55 to 56
#define B_SEGYVERSION 300 //301 to 302


#endif /* HEADER_MACROS_H_ */
