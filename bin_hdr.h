#ifndef BIN_HDR_H
#define BIN_HDR_H

typedef struct {	/* bhed - binary header */

	int jobid;	/* job identification number */

	int lino;	/* line number (only one line per reel) */

	int reno;	/* reel number */

	short ntrpr;	/* number of data traces per record */

    short nart;	/* number of auxiliary traces per record */

	unsigned short hdt; /* sample interval in micro secs for this reel */

	unsigned short dto; /* same for original field recording */

	unsigned short hns; /* number of samples per trace for this reel */

	unsigned short nso; /* same for original field recording */

	short format;	/* data sample format code:
				1 = floating point, 4 byte (32 bits)
				2 = two's complement integer, 4 byte (32 bits)
				3 = two's complement integer, 2 byte (16 bits)
				4 = fixed point with gain, 4 byte (32 bits)
				5 = IEEE floating point, 4 byte (32 bits)
				8 = two's complement integer, 1 byte (8 bits)
			*/

	short fold;	/* CDP fold expected per CDP ensemble */

	short tsort;	/* trace sorting code: 
				1 = as recorded (no sorting)
				2 = CDP ensemble
				3 = single fold continuous profile
				4 = horizontally stacked */

	short vscode;	/* vertical sum code:
				1 = no sum
				2 = two sum ...
				N = N sum (N = 32,767) */

	short hsfs;	/* sweep frequency at start */

	short hsfe;	/* sweep frequency at end */

	short hslen;	/* sweep length (ms) */

	short hstyp;	/* sweep type code:
				1 = linear
				2 = parabolic
				3 = exponential
				4 = other */

	short schn;	/* trace number of sweep channel */

	short hstas;	/* sweep trace taper length at start if
			   tapered (the taper starts at zero time
			   and is effective for this length) */

	short hstae;	/* sweep trace taper length at end (the ending
			   taper starts at sweep length minus the taper
			   length at end) */

	short htatyp;	/* sweep trace taper type code:
				1 = linear
				2 = cos-squared
				3 = other */

	short hcorr;	/* correlated data traces code:
				1 = no
				2 = yes */

	short bgrcv;	/* binary gain recovered code:
				1 = yes
				2 = no */

	short rcvm;	/* amplitude recovery method code:
				1 = none
				2 = spherical divergence
				3 = AGC
				4 = other */

	short mfeet;	/* measurement system code:
				1 = meters
				2 = feet */

	short polyt;	/* impulse signal polarity code:
				1 = increase in pressure or upward
				    geophone case movement gives
				    negative number on tape
				2 = increase in pressure or upward
				    geophone case movement gives
				    positive number on tape */

	short vpol;	/* vibratory polarity code:
				code	seismic signal lags pilot by
				1	337.5 to  22.5 degrees
				2	 22.5 to  67.5 degrees
				3	 67.5 to 112.5 degrees
				4	112.5 to 157.5 degrees
				5	157.5 to 202.5 degrees
				6	202.5 to 247.5 degrees
				7	247.5 to 292.5 degrees
				8	293.5 to 337.5 degrees */

	char unassigned[240];	/* unassigned */
	
	short revno;     /* SEG Y Format Revision Number */
	
	short trflag;    /* Fixed length trace flag
			 	1 = indicates that all traces in this SEG Y file 
			 	    are guaranteed to have the same sample interval 
			 	    and number of samples, as specified in Textual File 
			 	    Header bytes 3217-3218 and 3221-3222
			 	0 = indicates that the length of the traces in this SEG Y 					
			 	    file may vary and the number of samples in bytes   					    
			 	    115-116 of the Trace Header must be examined to 					    
			 	    determine the actual length of each trace */
	short extno;     /* Number of 3200 byte, Extended Textual File Header 
				0 = No Extended Textual File Header records
				1 = Variable number of Extended Textual File Header records  */
				
	char hunassigned[94];


} bhed;



static struct {
        char *key;      char *type;     int offs;
} bhdr[] = {
            {"jobid",             "i",            0},
            {"lino",              "i",            4},
            {"reno",              "i",            8},
            {"ntrpr",             "h",            12},
            {"nart",              "h",            14},
            {"hdt",               "h",            16},
            {"dto",               "h",            18},
            {"hns",               "h",            20},
            {"nso",               "h",            22},
            {"format",            "h",            24},
            {"fold",              "h",            26},
            {"tsort",             "h",            28},
            {"vscode",            "h",            30},
            {"hsfs",              "h",            32},
            {"hsfe",              "h",            34},
            {"hslen",             "h",            36},
            {"hstyp",             "h",            38},
            {"schn",              "h",            40},
            {"hstas",             "h",            42},
            {"hstae",             "h",            44},
            {"htatyp",            "h",            46},
            {"hcorr",             "h",            48},
            {"bgrcv",             "h",            50},
            {"rcvm",              "h",            52},
            {"mfeet",             "h",            54},
            {"polyt",             "h",            56},
            {"vpol",              "h",            58},
            {"unassigned",        "c",           298},
            {"revno",             "h",           300},
            {"trflag",            "h",           302},
            {"extno",             "h",           304}
};



#endif
