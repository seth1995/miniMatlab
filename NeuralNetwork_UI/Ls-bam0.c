/**	
 	Ls-bam0.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*******************************************************************
File Name:	LS-BAM0.c
Created:	8/2/97
Revised:
Notes:		(1) Least squares learning of BAM.
		(2) The original version
********************************************************************/

#include	<stdio.h>
#include	<math.h>
#include	<stdlib.h>	/* for function "random()" */
#include	<graphics.h>
#include	"gr-util.h"

#define		XN1	12
#define		YN1	12
#define		 N1	144	/* dim. of pattern A */
#define		XN2	11
#define		YN2	5
#define		 N2	55	/* dim. of pattern B */
#define		  M	3	/* number of pattern pairs */

#define		DATA_FILE	"p12x5.0"


void	read_data();
void	learn();
void	recall();
void	display_layer_xy(int LX[N1], int LY[N2], char ch);


int	A[N1][M], B[N2][M]; /* vector pattern pairs */
float   W[N1][N2];	    /* the weight matrix */


int	main()
{

	read_data();
	learn();
	recall();

	return(0);
}


void	read_data()
{
	int 	i, j, k;
	FILE	*fp;
	static char	file_name[10] = DATA_FILE;

	printf("\n It's reading data from pattern files ...");

	for(k=0; k<M; k++){
	  file_name[6] = '1'+k;	/* set pattern file name */
	  if((fp = fopen(file_name, "r")) == NULL){
	    printf("\n Failure to open file <%s> !\n", file_name);
	    printf("\n Program terminated. \n");
	    exit(1);
	  }

	  /* controvert matrix data into vector data */
	  printf("\n Reading file <%s> ...", file_name);

	  for(i=0; i<YN1; i++)  for(j=0; j<XN1; j++)
	    fscanf(fp, " %2d", &A[i*XN1+j][k]);

	  for(i=0; i<YN2; i++)  for(j=0; j<XN2; j++)
	    fscanf(fp, " %2d", &B[i*XN2+j][k]);

	  fflush(fp);
	  fclose(fp);

	  printf("\n End reading file <%s>.", file_name);

	}
}


void	learn()
{
	int	i, j, k, q, t;
	int	XX[N1][N1], YY[N2][N2], XY[N1][N2];
	float	lambda, XXW, WYY;
	float	W1[N1][N2];

	printf("\n It's now learning the matrix W ...");

	for(i=0; i<N1; i++) for(j=0; j<N1; j++){
	  XX[i][j] = 0;
	  for(k=0; k<M; k++)
	    XX[i][j] += A[i][k]*A[j][k];
	}

	for(i=0; i<N2; i++) for(j=0; j<N2; j++){
	  YY[i][j] = 0;
	  for(k=0; k<M; k++)
	    YY[i][j] += B[i][k]*B[j][k];
	}

	for(i=0; i<N1; i++) for(j=0; j<N2; j++){
	  XY[i][j] = 0;
	  for(k=0; k<M; k++) XY[i][j] += A[i][k]*B[j][k];
	}

	lambda = 0.001;
	for(i=0; i<N1; i++) for(j=0; j<N2; j++)
	  W[i][j] = lambda*2.0*XY[i][j];

	/** do iteration **/
	/*
	for(t=0; t<10; t++){
	  printf("\n Wait ...");

	  for(i=0; i<N1; i++) for(j=0; j<N2; j++)
	    W1[i][j] = W[i][j];

	  for(i=0; i<N1; i++) for(j=0; j<N2; j++){
	    XXW = 0.0;
	    for(q=0; q<N1; q++)  XXW += XX[i][q]*W1[q][j];
	    WYY = 0.0;
	    for(q=0; q<N2; q++)  WYY += W1[i][q]*YY[q][j];

	    W[i][j] = W1[i][j]-lambda*(XXW+WYY-2.0*XY[i][j]);
	  }
	}
	*/
}


void	recall()
{
	int     i, j, k;
	int	LX[N1], LY[N2];	/* polarized layer vectors */
	float	x[N1], y[N2];	/* intermediate layer vectors */


	printf("\n Press any key to start recall ... ");
	getch();

	Initialize();	/* start graphics mode */

	for(k=0; k<M; k++){

	/***  input pattern A to Layer X  ***********************/
	  for(i=0; i<N1; i++) 	LX[i] = x[i] = A[i][k];

	  display_layer_xy(LX, LY, 'A');


	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer Y ... ");
	  getch();

	  for(j=0; j<N2; j++)		LY[j] = -1;

	  for(j=0; j<N2; j++)	      	y[j] = 0.0;
	  for(j=0; j<N2; j++)  for(i=0; i<N1; i++)
	    y[j] += W[i][j]*x[i];

	  for(j=0; j<N2; j++){
	    if(y[j]>0)		LY[j] =  1;
	    if(y[j]<0)		LY[j] = -1;
	  }

	  display_layer_xy(LX, LY, 'B');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer X ...");
	  getch();

	  for(i=0; i<N1; i++)        	x[i] = 0.0;
	  for(i=0; i<N1; i++)  for(j=0; j<N2; j++)
	    x[i] += W[i][j]*y[j];

	  for(i=0; i<N1; i++){
	    if(x[i]>0)      	LX[i] =  1;
	    if(x[i]<0)      	LX[i] = -1;
	  }

	  display_layer_xy(LX, LY, 'A');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall the next pair ...");
	  getch();
	}

	ShowMessage(CYAN, " Recall ended. Press any key to end the program ...");
	getch();

	QuitGraphics();
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
		dot_value = LX[y*YN1+x];
		if(dot_value ==  1) 	FixBoxDot(x1, y1, GREEN);
		if(dot_value == -1) 	FixBoxDot(x1, y1, BLACK);
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
		dot_value = LY[y*YN2+x];
		if(dot_value ==  1) 	FixBoxDot(x2, y2, GREEN);
		if(dot_value == -1) 	FixBoxDot(x2, y2, BLACK);
	      }
	    }
	    break;
	}/** switch **/
}
