/**
	Macro_Defs.h 

	QT4 GUI for AnalogDesigner (AD)
 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
*/


#ifndef MACRO_DEFS_H
#define MACRO_DEFS_H


#include  <cstdlib>
#include  <climits>
#include  <iostream>
#include  <string>
#include  <vector>
#include  <list>
#include  <complex>


using namespace std;

#define DEBUG
#define TRACE

#define UNUSED_VAR(x)  (void) x


#define LINE_INFO   "\n[" << __FILE__ << " : line " << __LINE__ << " : " << __FUNCTION__ << "()] - "

#define PRINT_LINE_INFO  printf("\n[%s : line %d] %s() : ",  __FILE__, __LINE__, __FUNCTION__);


/*------- Constants used in the parser ----------------*/

#define 	ST_MAX_VECTOR_LENGTH    	1024 	// (Symbol Table)

#define  	MAX_NAME_LEN 				1024
#define  	MAX_STRING_LEN     			1024
#define		MAX_TOKEN_LEN				1024

// Max number of device terminals. An artificially defined subckt may have more terminals.
#define 	MAX_NUM_TERMS  			64	

// Maximum number of tokens in one line of netlist.
#define 	MAX_LINE_TOKENS			100

#define		NETLIST_TITLE_LEN 			256

#define		YYMAXDEPTH				40

#define		INPUT_BUFSIZE				2048
#define		BUFSIZE						1024

#define  	EOFTOKEN					-1	

#define 	HASH_SIZE					29
/* Size of various hash tables, best if it is a prime number and 
   the larger the better for speed. */


/* --------- Some generic type definitions: (type defs from XSPICE::type_defs.h) ------------- */

typedef void *VOID_PTR;
typedef const void *CONST_VOID_PTR;

typedef complex<double> c_double;	// double complex number

class Device;

/**
	Structure to hold device/model parameter information
	index = {RES_RESIST, RES_TEMP, RES_LENGTH ...}
	For quick accessing a para using "switch/case" ...
	Here index is not necessarily the order of the parameter in the list.
	parameter data type, e.g., (DT_INT, DT_DOUBLE, ...)

	For backward compatibility with Spice3f, we keep using the string type (const char*)
	rather than (string). This struct is to be used by the derived Analysis classes.
*/
struct Para_Info {
  	const char     *name;     	// parameter name
  	int     		index;       	// depending on the owner of the parameter
  	int  			datatype;     // DT_INT, DT_DOUBLE, ... 
  	const char      *info;      	// parameter information
};

/* Define a complex number struct, much simpler than a complex number class. */
struct C_Number {
	double	real;
	double 	imag;
};		// Will use this struct in place of Double_Complex


struct Double_Complex {
	double	re;
	double 	im;
};


struct Int_Vector {
	int		size;
	int		*ival;
};

struct Double_Vector {
	int		size;
	double	*dval;
};

struct Complex_Vector {
	int		size;
	Double_Complex	*cvec;
};

struct String_Vector {
	int		size;
	string	*svec;
};


// Spice definition (recommended to use) 

union G_Value {
	int 		ival;             		/* integer or bool valued data */
	double 	dval;          			/* real valued data */
	Double_Complex cval;       		/* complex valued data */
	string 	*sval;           		// Must use pointer for <string> type in a union
	struct {
		int 		size;       	/* length of vector */
		union {
			int 			*ivec;      	/* pointer to integer vector */
			double 		*dvec;   	/* pointer to real vector */
			Double_Complex *cvec;	/* pointer to complex vector: cVec[k].re,  cVec[k].im */
			const string 	*svec;    	/* pointer to string vector */
		} vec;
	} array;		/** An array of same-type values **/
};



// Enumerated data types (for accessing a G_Value field.)
enum DATA_TYPE {
	DT_NONE = 0,	/* no data type */
	DT_BOOL,
	DT_INT, 
	DT_LONG, 
	DT_FLOAT, 
	DT_DOUBLE, 
	DT_CHAR, 
	DT_STRING,
	DT_COMPLEX, 
	DT_INT_VEC, 
	DT_DOUBLE_VEC,
	DT_COMPLEX_VEC, 
	DT_STRING_VEC,
	DT_INT_VEC_VEC, 
	DT_DOUBLE_VEC_VEC,
	DT_COMPLEX_VEC_VEC,
	DT_STRING_VEC_VEC
};


// Spice (src/include/optdefs.h)

#define OPT_GMIN 1
#define OPT_RELTOL 2
#define OPT_ABSTOL 3
#define OPT_VNTOL 4
#define OPT_TRTOL 5
#define OPT_CHGTOL 6
#define OPT_PIVTOL 7
#define OPT_PIVREL 8
#define OPT_TNOM 9
#define OPT_ITL1 10
#define OPT_ITL2 11
#define OPT_ITL3 12
#define OPT_ITL4 13
#define OPT_ITL5 14
#define OPT_DEFL 15
#define OPT_DEFW 16
#define OPT_DEFAD 17
#define OPT_DEFAS 18
#define OPT_BYPASS 19
#define OPT_MAXORD 20

#define OPT_ITERS 21
#define OPT_TRANIT 22
#define OPT_TRANPTS 23
#define OPT_TRANACCPT 24
#define OPT_TRANRJCT 25
#define OPT_TOTANALTIME 26
#define OPT_TRANTIME 27
#define OPT_LOADTIME 28
#define OPT_DECOMP 29
#define OPT_SOLVE 30
#define OPT_TRANDECOMP 31
#define OPT_TRANSOLVE 32
#define OPT_TEMP 33
#define OPT_OLDLIMIT 34
#define OPT_TRANCURITER 35
#define OPT_SRCSTEPS 36
#define OPT_GMINSTEPS 37
#define OPT_MINBREAK 38
#define OPT_NOOPITER 39
#define OPT_EQNS 40
#define OPT_REORDTIME 41
#define OPT_METHOD 42
#define OPT_TRYTOCOMPACT 43
#define OPT_BADMOS3 44
#define OPT_KEEPOPINFO 45

#define OPT_SYMBOLIC 46		// added by G.Shi for Symbolic AC
#define OPT_DDD 47			// added by G.Shi for Symbolic AC
#define OPT_GPDD 48			// added by G.Shi for Symbolic AC

/** The following are HSPICE OPTIONS  **/

#define OPT_DCSTEP 		50
/* Three initial real points for Muller method in PZ analysis (below) */
#define OPT_X0R 			51	// (X0R, X1R, X2R) initial points for the Muller method (PZ)
#define OPT_X1R 			52
#define OPT_X2R 			53
/* Scaling factors (below) */
#define OPT_FSCAL 		54   // Scaling factor for (f) or (s)
#define OPT_GSCAL 		55	// Scaling factor for G
#define OPT_CSCAL 		56	// Scaling factor for C
#define OPT_LSCAL 		57	// Scaling factor for L


// Constants for symbol table (ST) processing
#define ST_OK		    			0
#define ST_SYM_FOUND	    		0
#define ST_SYM_NOT_FOUND    	1


//---------- Other constant definitions ---------------

#define MAX_NUM_DEVS 	32

#ifndef TRUE
#define TRUE				1
#endif
#ifndef FALSE
#define FALSE			0
#endif

#ifndef YES
#define YES    			1
#endif
#ifndef NO
#define NO     			0
#endif

#define XSP_TRUE		1
#define XSP_FALSE		0

#define  XSP_YES    		1	// XSPICE_YES
#define  XSP_NO     		0	// XSPICE_NO
#define  XSP_FAIL		-1
#define  XSP_ERROR		1

#define 	XSP_OK			0	// OK means no error

#ifndef OK
#define OK				0	// OK means no error
#endif

// An error is supposed to be a nonzero number.
#ifndef FAIL
#define FAIL				-100
#endif
#ifndef STOP
#define STOP				-200
#endif
#ifndef ERROR
#define ERROR			1
#endif

#define ID_ANALYSIS 		0x01
#define ID_TASK 			0x02
#define ID_INSTANCE 		0x04
#define ID_MODEL 		0x08
#define ID_SIGNAL 		0x10
#define ID_OTHER 		0x20

// Define analysis types  (G.Shi, 11-08-2010)
#define ANAL_NONE		0
#define ANAL_AC			1
#define ANAL_DCT		2
#define ANAL_DIST		3		// distortion
#define ANAL_NOISE		4
#define ANAL_OP			5
#define ANAL_PZ			6
#define ANAL_SENS		7
#define ANAL_TRAN		8

// Electric type of a node variable
#define SP_NONE 	 0
#define SP_VOLTAGE 	3
#define SP_CURRENT 	4	

// Input/Output type
enum IO_Type {NONE_IO=0, CURRENT_IO, VOLTAGE_IO, VCVS_IO, CCCS_IO, VCCS_IO, CCVS_IO};

// The I/O port attributes
enum PORT_I_V {VOLTAGE_IN = 10, VOLTAGE_OUT, CURRENT_IN, CURRENT_OUT};
/*
#define VOLTAGE_IN 		5
#define VOLTAGE_OUT 	6
#define CURRENT_IN		7
#define CURRENT_OUT		8
*/

// Define output variable prefix
#define VAR_SUFFIX_NONE	0	// no spec
#define VAR_SUFFIX_R		1	// real part in AC results
#define VAR_SUFFIX_I		2	// imaginary part in AC results
#define VAR_SUFFIX_M		3	// magnitude in AC results
#define VAR_SUFFIX_P		4	// phase in AC results
#define VAR_SUFFIX_DB		5	// magnitude in dB for AC results

//=========== Analysis Mode ==============
/* Old 'mode' parameters (lower 4 bits) for analysis type */
#define MODE 		0x3			// ----0011;   MODETRAN | MODEAC
#define MODETRAN 	0x1			// ----0001
#define MODEAC 		0x2			// ----0010

/* Old 'modedc' parameters (bits 4-7) for DC analysis type */
// The following mode flags control the iteration flow in Solver::DC_Iteration().

#define MODEDC 0x70				// 0111----;  conjuction of the following three bits

#define MODEDCOP 0x10			// 0001----
#define MODETRANOP 0x20			// 0010----	// A tran point is an OP at that time point.
#define MODEDCT 0x40			// 0100----   // was "MODEDCTRANCURVE"

/* Old 'initf' parameters (higher 8 bits) for flow control in NI_Iterate */
//#define INITF 0x3f00				// 0011'1111----'----; conjunction of the following six bits
// (changed to)
#define MODEINIT 0x3f00				// 0011'1111----'----; conjunction of the following six bits

#define MODEINITFLOAT 0x100		// 0000'0001----'----		(used in continue mode)
/* (next) initial voltages at MOS junctions : (VDS, VGS, VBS) */
#define MODEINITJCT 0x200		// 0000'0010----'----	 	(used in start mode)
#define MODEINITFIX 0x400		// 0000'0100----'----
#define MODEINITSMSIG 0x800		// 0000'1000----'----  	(for small signal)
#define MODEINITTRAN 0x1000		// 0001'0000----'----
#define MODEINITPRED 0x2000		// 0010'0000----'----

#define MODEUIC 0x10000l		// the suffix 'l' stands for long data type (for user initial condition)
//===========================================================


/* source function types (shared with current sources) */
/*
#define PULSE 	1
#define SINE 		2
#define EXP 		3
#define SFFM 	4
#define PWL 		5
*/

/* Source element parameters */
#define SRC_DC 			1
#define SRC_AC 			2
#define SRC_AC_MAG 		3
#define SRC_AC_PHASE 	4
#define SRC_PULSE 		5
#define SRC_SINE 		6
#define SRC_EXP 			7
#define SRC_PWL 		8
#define SRC_SFFM 		9
#define SRC_BR 			10
#define SRC_FUNC_TYPE 	11
#define SRC_FUNC_ORDER 12
#define SRC_FUNC_COEFFS 13
#define SRC_AC_REAL 	14
#define SRC_AC_IMAG 	15
#define SRC_POS_NODE 	16
#define SRC_NEG_NODE 	17
#define SRC_CURRENT 	18
#define SRC_POWER 		19
#define SRC_DIST_F1 		20
#define SRC_DIST_F2 		21
#define SRC_VOLTAGE 	22

// ==== Integration Methods ==============

#define TRAPEZOIDAL 		1
#define GEAR 			2


#define XSP_INTEGER 		0x2
#define XSP_REAL 		0x4		
#define XSP_COMPLEX 	0x8
#define XSP_VECTOR 		0x8000

/* Parameter data types */

#define INTEGER 	0x2
#define REAL 	0x4			// REAL also defined in spConfig.h (has been changed to spREAL there)
#define COMPLEX 0x8
#define VECTOR 	0x8000

#define INTVEC      	(INTEGER|VECTOR)
#define REALVEC     	(REAL|VECTOR)
#define CPLXVEC     	(COMPLEX|VECTOR)

#define FLAG 		0x1
#define NODE 		0x10
#define STRING 		0x20
#define INSTANCE 	0x40
#define PARSETREE 	0x80

/* indicates that for a query the integer field will have a selector
 * in it to pick a sub-field */
#define SELECT 		0x800
#define VSELECT 		0x400

#define FLAGVEC     	(FLAG|VECTOR)
#define NODEVEC     	(NODE|VECTOR)
#define STRINGVEC   	(STRING|VECTOR)
#define INSTVEC    	(INSTANCE|VECTOR)

#define REQUIRED 	0x4000
#define VARTYPES 	0x80ff

#define SET 		0x2000
#define ASK 		0x1000

//==== Spice Errors =====================

/* 
	Common error message descriptions
	Spice in "src/include/iferrmsg.h" 
*/

#define E_PAUSE -1      /* pausing on demand */

#define E_PANIC 1       /* vague internal error for "can't get here" cases */
#define E_EXISTS 2      /* warning/error - attempt to create duplicate */
                        /* instance or model. Old one reused instead */
#define E_NODEV 3       /* attempt to modify a non-existant instance */
#define E_NOMOD 4       /* attempt to modify a non-existant model */
#define E_NOANAL 5      /* attempt to modify a non-existant analysis */
#define E_NOTERM 6      /* attempt to bind to a non-existant terminal */
#define E_BADPARM 7     /* attempt to specify a non-existant parameter */
#define E_NOMEMORY 	8       /* insufficient memory available - VERY FATAL */
#define E_NODECON 		9     /* warning/error - node already connected, old */
                        				/* connection replaced */
#define E_UNSUPP 10     /* the specified operation is unsupported by the */
                        /* simulator */
#define E_PARMVAL 11    /* the parameter value specified is illegal */
#define E_NOT_EMPTY 12   /* deleted still referenced item. */
#define E_NOCHANGE 13   /* simulator can't tolerate any more topology changes */
#define E_NOTFOUND 14   /* simulator can't find something it was looking for */
#define E_BAD_DOMAIN 15 /* output interface begin/end domain calls mismatched */
#define E_FORMAT 16		/* format error in netlist specification */
#define E_BADCODE 17	/* bad code flag for code debugging */

#define E_PRIVATE 100   /* messages above this number are private to */
                        /* the simulator and MUST be accompanied by */
                        /* a proper setting of errMsg */
                        /* this constant should be added to all such messages */
                        /* to ensure error free operation if it must be */
                        /* changed in the future */

/* Constants defined in Spice3 "include/sperror.h". */
#define E_BADMATRIX (E_PRIVATE+1)/* ill-formed matrix can't be decomposed */

#define E_SINGULAR (911) 		/* Temporarily used  */
/* Matrix is singular; MUST BE EQUAL TO THAT DEFINED IN the SPARSE package */

#define E_ITERLIM (E_PRIVATE+3)  /* iteration limit reached,operation aborted */
#define E_ORDER (E_PRIVATE+4)    /* integration order not supported */
#define E_METHOD (E_PRIVATE+5)   /* integration method not supported */
#define E_TIMESTEP (E_PRIVATE+6) /* timestep too small */
#define E_TRANSMISSIONLINE (E_PRIVATE+7)    /* transmission line in pz analysis */
#define E_MAGEXCEEDED (E_PRIVATE+8) /* pole-zero magnitude too large */
#define E_SHORT (E_PRIVATE+9)   /* pole-zero input or output shorted */
#define E_IN_IS_OUT (E_PRIVATE+10)    /* pole-zero input is output */
#define E_ASKCURRENT (E_PRIVATE+11) /* ac currents cannot be ASKed */
#define E_ASKPOWER (E_PRIVATE+12)   /* ac powers cannot be ASKed */
#define E_NODUNDEF (E_PRIVATE+13) /* node not defined in noise anal */
#define E_NOACINPUT (E_PRIVATE+14) /* no ac input src specified for noise */
#define E_NOF2SRC (E_PRIVATE+15) /* no source at F2 for IM disto analysis */
#define E_NODISTO (E_PRIVATE+16) /* no distortion analysis - NODISTO defined */
#define E_NONOISE (E_PRIVATE+17) /* no noise analysis - NONOISE defined */


//==== Define Constants ==================

#ifndef 	PI
#define 	PI		3.14159265358979
#endif

// From Spice [src/include/spice.h]
#ifndef	M_PI
#define 	M_PI		3.14159265358979323846
#endif
#ifndef	M_E
#define 	M_E  	   	2.7182818284590452354
#endif
#ifndef	M_LOG2E
#define 	M_LOG2E	1.4426950408889634074	// log2(e)
#endif
#ifndef	M_LOG10E
#define 	M_LOG10E  	0.43429448190325182765	// log10(e)
#endif

#ifndef 	M_LN2
#define 	M_LN2   0.69314718055994530942	// = ln(2)
#endif
#ifndef 	M_LN10
#define 	M_LN10  2.30258509299404568402	// = ln(10)
#endif



/* Spice [src/include/hw_ieee.h]
 *  IEEE Floating point
 */

#define MAX_EXP_ARG	709.0

#ifndef DBL_EPSILON
# define DBL_EPSILON		8.9e-15
#endif

#ifndef DBL_MAX
# define DBL_MAX	1.79769313486231e+308
#endif

#ifndef 	DBL_LARGE 
#define	DBL_LARGE	1e20
#endif 

#ifndef DBL_MIN
# define DBL_MIN		2.22507385850721e-308
#endif
#ifndef SHRT_MAX
# define SHRT_MAX	32766
#endif
#ifndef INT_MAX
# define INT_MAX		2147483646
#endif
#ifndef LONG_MAX
# define LONG_MAX	2147483646
#endif

#define MAXPOSINT	INT_MAX
//----- IEEE Floating point (above) --------------------------


#define RAD2DEG		180.0/PI
#define INFINITESIMAL	1e-15

const double zero = 0.0;
const double one  = 1.0;

//const COMPLEX  c_zero(zero, zero);
//const COMPLEX  c_one (one,  zero);

const double  pi = 3.14159265358979323846;
const double  twopi = 2.0 * pi;
const double  deg2rad = pi / 180.0;

// === Physical constants ===
// Boltzman constant
const double kBoltzman = 1.3806266e-23;

// Electron charge
const double eCharge = 1.6021918e-19;

// Permittivity of free space
const double epsilon0 = 8.854194e-12;

// Permeability of free space
const double mu0 = pi*4e-7;

// Speed of light in free space
const double c0 = 2.998633e+08;

/* assuming silicon - make definition for epsilon of silicon */
#define EPSSIL (11.7 * 8.854214871e-12)

// From Spice [src/include/const.h]
#define CHARGE (1.6021918e-19)
#define CONSTboltz (1.3806226e-23)

#define CONSTCtoK (273.15)	// degree Celsius to Kelvin
#define ROOMTEMP 300.15 		/* in absolute temperature (27 degrees Celsius) */

// Pre-calculate some constants 
const double	CONSTroot2 = sqrt(2.);
const double	CONSTvt0 = CONSTboltz * (27 + CONSTCtoK ) / CHARGE;  // k*T0 / q
const double	CONSTKoverQ = CONSTboltz / CHARGE;		//  k / q
const double	CONSTe = exp((double)1.0);

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef FABS
#define FABS(a) ( ((a) < 0) ? -(a) : (a) )
#endif
#ifndef SWAP_INT
#define SWAP_INT(x, y) { int tmp;  tmp = (x), x = (y), y = tmp; }
#endif

/*
#define sgn0(X)	((X) < 0 ? -1 : (X) == 0 ? 0 : 1)
#define sgn(X)	((X) < 0 ? -1 : 1) // 0 is evaluated to +1
*/

#ifndef SGN0
#define SGN0(X)	((X) < 0 ? -1 : (X) == 0 ? 0 : 1)  // 0 is evaluated to 0
#endif
#ifndef SGN
#define SGN(X)	((X) < 0 ? -1 : 1) // 0 is evaluated to +1
#endif

#ifndef FREE
#define FREE(x) { if (x) {free((char *)(x)); (x) = 0;} }
#endif


#endif

