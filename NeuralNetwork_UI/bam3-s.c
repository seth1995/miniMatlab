/**	
 	bam3-s.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*******************************************************************
File Name:	BAM3-S.c      study the stability.
Created:	8/11/97
Revised:
Note:		(1) BAM with 3 layers (with a nonlinear hidden layer).
		(2) Link with "GR-UTIL.C".
********************************************************************/

#include	<stdio.h>
#include	<math.h>
#include	<stdlib.h>	/* for function "random()" */
#include	<graphics.h>
#include	"gr-util.h"

#define		Randomize	(2.0*random(2)-1.0)

#define		XN1	 5
#define		YN1	 7
#define		 N1	35	/* dim. of pattern A */

#define		XN2	 5
#define		YN2	 7
#define		 N2	35	/* dim. of pattern B */

#define		  M	10	/* 10 pairs of patterns */
#define		  H	10 	/* dim. of the hidden layer */

int	A[M][N1], B[M][N2]; 	/* vector pattern pairs */
float   V[N1][H], W[H][N2];	/* the weight matrix */
float	Theta[H];		/* the threshold vector */
float	Hid[H];			/* output of the hidden layer */

void	init_para();
void	output_from_A(float a[N1], int b[N2]);
void	output_from_B(int a[N1], float b[N2]);

void	recall();
void	display_layer_xy(int LX[N1], int LY[N2], char ch);


int	main()
{
	char	ch;

	init_para();

	Initialize();

	recall();

	QuitGraphics();

	return(0);
}



void	init_para()
{
	int	i, j, k;

	for(i=0; i<N1; i++) for(j=0; j<H; j++)
	  V[i][j] = Randomize;
	for(i=0; i<H; i++) for(j=0; j<N2; j++)
	  W[i][j] = Randomize;
	for(i=0; i<H; i++)
	  Theta[i] = Randomize;

	for(k=0; k<M; k++) for(i=0; i<N1; i++)	A[k][i] = Randomize;
}

#define	ALPHA	1.0

void	output_from_A(float a[N1], int b[N2])
{
	register int	i, j;

	for(i=0; i<H; i++){
	  Hid[i] = Theta[i];
	  for(j=0; j<N2; j++)  	Hid[i] += W[i][j]*b[j];
	  Hid[i] = (1.0-exp(-ALPHA*Hid[i]))/(1.0+exp(-ALPHA*Hid[i]));
	}
	for(i=0; i<N1; i++){
	  a[i] = 0;
	  for(j=0; j<H; j++)    a[i] += V[i][j]*Hid[j];
	}
}


void	output_from_B(int a[N1], float b[N2])
{
	register int	i, j;

	for(i=0; i<H; i++){
	  Hid[i] = Theta[i];
	  for(j=0; j<N1; j++)  	Hid[i] += V[j][i]*a[j];
	  Hid[i] = (1.0-exp(-ALPHA*Hid[i]))/(1.0+exp(-ALPHA*Hid[i]));
	}
	for(i=0; i<N2; i++){
	  b[i] = 0;
	  for(j=0; j<H; j++)    b[i] += W[j][i]*Hid[j];
	}
}



void	recall()
{
	int     i, j, k;
	float	x[N1], y[N2];	/* nonpolarized layer vectors */
	char	ch;

	for(k=0; k<M; k++){
	/***  display the input pattern A to Layer X  **************/

	 for(;;){
	  ShowMessage(WHITE, " Press any key other than <E> to continue ...");
	  ch = getch();
	  if(toupper(ch)=='E') break;

	  display_layer_xy(A[k], B[k], 'A');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer Y ...");
	  getch();

	  output_from_B(A[k], y);

	  for(j=0; j<N2; j++){
	    if(y[j]>0)		B[k][j] =  1;
	    else		B[k][j] = -1;
	  }

	  display_layer_xy(A[k], B[k], 'B');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer X ...");
	  getch();

	  output_from_A(x, B[k]);

	  for(i=0; i<N1; i++){
	    if(x[i]>0)      	A[k][i] =  1;
	    else 	      	A[k][i] = -1;
	  }

	  display_layer_xy(A[k], B[k], 'A');
	 }
	  ShowMessage(LIGHTMAGENTA, " Press any key to recall the next pair ...");
	  getch();
	}
}


#define		PX1	20    	/* left position of pattern 1 */
#define		PY	20	/* upper position of both patterns */
#define		PD	10	/* box dot size */

void	display_layer_xy(int LX[N1], int LY[N2], char ch)
{
	int	XL1 = PX1, XL2;	/* upper-left corner of two grids */
	int	YU1 = PY, XR1, YD1;
	int     YU2 = PY, XR2, YD2;

	int	x1, y1, x2, y2; /* box dot position */
	int	dot_value;
	register int	x, y;

	/** right and down borders of patterns A and B **/
	XR1 = XL1+XN1*PD;	YD1 = YU1+YN1*PD;
	XL2 = XR1+4*PD;
	XR2 = XL2+XN2*PD;	YD2 = YU2+YN2*PD;

	setcolor(WHITE);

	switch(ch){
	  case	'A':
	    /** grid for pattern A **/
	    for(x=0; x<=XN1; x++) line(XL1+x*10, YU1, XL1+x*10, YD1);
	    for(y=0; y<=YN1; y++) line(XL1, YU1+y*10, XR1, YU1+y*10);

	    /***  show pattern A  ***/
	    y1 = PY+5-PD;
	    for(y=0; y<YN1; y++){
	      y1 += PD; 	x1 = XL1+5-PD;
	      for(x=0; x<XN1; x++){
		x1 += PD;
		dot_value = LX[y*XN1+x];
		if(dot_value ==  1) 	FixBoxDot(x1, y1, 3, GREEN);
		if(dot_value == -1) 	FixBoxDot(x1, y1, 3, BLACK);
	      }
	    }
	    break;
	  case	'B':
	    /** grid for pattern B **/
	    for(x=0; x<=XN2; x++) line(XL2+x*10, YU2, XL2+x*10, YD2);
	    for(y=0; y<=YN2; y++) line(XL2, YU2+y*10, XR2, YU2+y*10);

	    /** show pattern B **/
	    y2 = PY+5-PD;
	    for(y=0; y<YN2; y++){
	      y2 += PD; 	x2 = XL2+5-PD;
	      for(x=0; x<XN2; x++){
		x2 += PD;
		dot_value = LY[y*XN2+x];
		if(dot_value ==  1) 	FixBoxDot(x2, y2, 3, GREEN);
		if(dot_value == -1) 	FixBoxDot(x2, y2, 3, BLACK);
	      }
	    }
	    break;
	}/** switch **/
}
