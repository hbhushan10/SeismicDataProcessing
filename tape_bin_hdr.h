#ifndef TAPE_BIN_HDR_H
#define TAPE_BIN_HDR_H

typedef union {
    char s[8];
    short h;
    unsigned short u;
    long l;
    unsigned long v;
    int i;
    unsigned int p;
    float f;
    double d;
    unsigned int U:16;
    unsigned int P:32;
} Value;


typedef struct {        /* bhedtape - binary header */

        unsigned int jobid:32;  /* job identification number */

        unsigned int lino:32;   /* line number (only one line per reel) */

        unsigned int reno:32;   /* reel number */

        unsigned int ntrpr:16;  /* number of data traces per record */

        unsigned int nart:16;   /* number of auxiliary traces per record */

        unsigned int hdt:16;    /* sample interval (microsecs) for this reel */

        unsigned int dto:16;    /* same for original field recording */

        unsigned int hns:16;    /* number of samples per trace for this reel */

        unsigned int nso:16;    /* same for original field recording */

        unsigned int format:16; /* data sample format code:
                                1 = floating point (4 bytes)
                                2 = fixed point (4 bytes)
                                3 = fixed point (2 bytes)
                                4 = fixed point w/gain code (4 bytes) */

        unsigned int fold:16;   /* CDP fold expected per CDP ensemble */

        unsigned int tsort:16;  /* trace sorting code: 
                                1 = as recorded (no sorting)
                                2 = CDP ensemble
                                3 = single fold continuous profile
                                4 = horizontally stacked */

        unsigned int vscode:16; /* vertical sum code:
                                1 = no sum
                                2 = two sum ...
                                N = N sum (N = 32,767) */

        unsigned int hsfs:16;   /* sweep frequency at start */

        unsigned int hsfe:16;   /* sweep frequency at end */

        unsigned int hslen:16;  /* sweep length (ms) */

        unsigned int hstyp:16;  /* sweep type code:
                                1 = linear
                                2 = parabolic
                                3 = exponential
                                4 = other */

        unsigned int schn:16;   /* trace number of sweep channel */

        unsigned int hstas:16;  /* sweep trace taper length at start if
                           tapered (the taper starts at zero time
                           and is effective for this length) */

        unsigned int hstae:16;  /* sweep trace taper length at end (the ending
                           taper starts at sweep length minus the taper
                           length at end) */

        unsigned int htatyp:16; /* sweep trace taper type code:
                                1 = linear
                                2 = cos-squared
                                3 = other */

        unsigned int hcorr:16;  /* correlated data traces code:
                                1 = no
                                2 = yes */

        unsigned int bgrcv:16;  /* binary gain recovered code:
                                1 = yes
                                2 = no */

        unsigned int rcvm:16;   /* amplitude recovery method code:
                                1 = none
                                2 = spherical divergence
                                3 = AGC
                                4 = other */

        unsigned int mfeet:16;  /* measurement system code:
                                1 = meters
                                2 = feet */

        unsigned int polyt:16;  /* impulse signal polarity code:
                                1 = increase in pressure or upward
                                    geophone case movement gives
                                    negative number on tape
                                2 = increase in pressure or upward
                                    geophone case movement gives
                                    positive number on tape */

        unsigned int vpol:16;   /* vibratory polarity code:
                                code    seismic signal lags pilot by
                                1       337.5 to  22.5 degrees
                                2        22.5 to  67.5 degrees
                                3        67.5 to 112.5 degrees
                                4       112.5 to 157.5 degrees
                                5       157.5 to 202.5 degrees
                                6       202.5 to 247.5 degrees
                                7       247.5 to 292.5 degrees
                                8       293.5 to 337.5 degrees */

        unsigned char unassigned[240];      /* unassigned */
        
        unsigned int revno:16;     /* SEG Y Format Revision Number */
        
        unsigned int trflag:16;    /* Fixed length trace flag
			 	1 = indicates that all traces in this SEG Y file 
			 	    are guaranteed to have the same sample interval 
			 	    and number of samples, as specified in Textual File 
			 	    Header bytes 3217-3218 and 3221-3222
			 	0 = indicates that the length of the traces in this SEG Y 
			 	    file may vary and the number of samples in bytes   
			 	    115-116 of the Trace Header must be examined to
			 	    determine the actual length of each trace */
	unsigned int extno:16;     /* Number of 3200 byte, Extended Textual File Header 
				0 = No Extended Textual File Header records
				1 = Variable number of Extended Textual File Header 					records  */
				
	unsigned char hunassigned[94];   /* unassigned*/

} tapebhed;


static struct {
        char *key;      char *type;     int offs;
} tapebhdr[] = {
           {"jobid",             "P",            0},
           {"lino",              "P",            4},
           {"reno",              "P",            8},
           {"ntrpr",             "U",            12},
           {"nart",              "U",            14},
           {"hdt",               "U",            16},
           {"dto",               "U",            18},
           {"hns",               "U",            20},
           {"nso",               "U",            22},
           {"format",            "U",            24},
           {"fold",              "U",            26},
           {"tsort",             "U",            28},
           {"vscode",            "U",            30},
           {"hsfs",              "U",            32},
           {"hsfe",              "U",            34},
           {"hslen",             "U",            36},
           {"hstyp",             "U",            38},
           {"schn",              "U",            40},
           {"hstas",             "U",            42},
           {"hstae",             "U",            44},
           {"htatyp",            "U",            46},
           {"hcorr",             "U",            48},
           {"bgrcv",             "U",            50},
           {"rcvm",              "U",            52},
           {"mfeet",             "U",            54},
           {"polyt",             "U",            56},
           {"vpol",              "U",            58},
           {"unassigned",        "C",           298},
           {"revno",             "U",           300},
           {"trflag",            "U",           302},
           {"extno",             "U",           304}
};


#endif	/* end  TAPE_BIN_HDR_H */
