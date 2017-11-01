/**	
 	bam-op.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*******************************************************************
File Name:	BAM-op.c
Created:	8/7/97
Revised:
Note:		(1) BAM with T. Wang's algorithm.
		(2) Link with "GR-UTIL.C".
		(3) M = 45 OK; M = 50 Failed.
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

#define		  M	 21     /* number of pattern pairs */
#define		 NC     1000	/* number of learning cycles */
				/* one cycle covers all pattern pairs */
#define	       RATE      1    	/* learning rate */

#define		DATA_FILE	"pp.00"


int	A[M][N1], B[M][N2]; 	/* vector pattern pairs */
float   W[N1][N2];		/* the weight matrix */

void	read_data();

void	init_para();
void	output_from_A(float a[N1], int b[N2]);
void	output_from_B(int a[N1], float b[N2]);
void	update_by_A(int error_A[N1], int k);
				/* update by layer-A error */
void	update_by_B(int error_B[N2], int k);
				/* update by layer-B error */
void	learn();

void	recall();
void	display_layer_xy(int LX[N1], int LY[N2], char ch);


int	main()
{
	char	ch;

	read_data();
	learn();

	printf("\n To recall? (Y/N) ");	ch = getch();
	if(toupper(ch)=='Y')
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
	    file_name[3] = 'a'+k;
	    file_name[4] = '\0';

	  /*
	  if(k<10)	file_name[4] = '0'+k;
	  if((k>=10)&&(k<40)){
	    file_name[3] = '0'+k/10;
	    file_name[4] = '0'+k%10;
	  }
	  if(k>=40){
	    file_name[3] = 'a'+k-40;
	    file_name[4] = '\0';
	  }
	  */
	  if((fp = fopen(file_name, "r")) == NULL){
	    printf("\n Failure to open file <%s> !\n", file_name);
	    printf("\n Program terminated. \n");
	    exit(1);
	  }

	  /* controvert matrix data into vector data */
	  printf("\n\n Reading file <%s> ...", file_name);

	  for(i=0; i<YN1; i++) for(j=0; j<XN1; j++)
	    fscanf(fp, " %2d", &A[k][i*XN1+j]);

	  for(i=0; i<YN2; i++)  for(j=0; j<XN2; j++)
	    fscanf(fp, " %2d", &B[k][i*XN2+j]);

	  fflush(fp);
	  fclose(fp);

	  printf("\n End reading file <%s>.", file_name);
	}
}


void	init_para()
{
	int	i, j;

	for(i=0; i<N1; i++) for(j=0; j<N2; j++)
	  W[i][j] = 0;
}

void	output_from_A(float a[N1], int b[N2])
{
	register int	i, j;

	for(i=0; i<N1; i++){
	  a[i] = 0;
	  for(j=0; j<N2; j++)    a[i] += W[i][j]*b[j];
	}
}


void	output_from_B(int a[N1], float b[N2])
{
	register int	i, j;

	for(j=0; j<N2; j++){
	  b[j] = 0;
	  for(i=0; i<N1; i++)    b[j] += W[i][j]*a[i];
	}
}


void	learn()
{
	register int	i, j, k, cycle;
	float	output_A[N1], output_B[N2];
	unsigned char	All_Positive;

	printf("\n\n Press any key to start learning ...");
	getch();

	init_para();

	for(cycle=0; cycle<NC; cycle++){

	  printf("\n Cycle %2d ...", cycle+1);

	  All_Positive = 1;
	  for(k=0; k<M; k++){

	    output_from_B(A[k], output_B);

	    for(j=0; j<N2; j++){
	      if((output_B[j]*B[k][j])<=0){
		for(i=0; i<N1; i++)  W[i][j] += RATE*A[k][i]*B[k][j];
		All_Positive = 0;
	      }

	    }

	    output_from_A(output_A, B[k]);

	    for(i=0; i<N1; i++)
	      if((output_A[i]*A[k][i])<=0){
		for(j=0; j<N2; j++)  W[i][j] += RATE*A[k][i]*B[k][j];
		All_Positive = 0;
	      }
	  }
	  if(All_Positive){
	    printf("\n All pattern pairs have been stored");
	    printf(" befor all cycles are ended. \n");
	    break;
	  }
	  else
	    printf("\n Learning is not complete. \n");
	}

}


void	recall()
{
	int     i, j, k;
	int	LX[N1], LY[N2];	/* polarized layer vectors */
	float	x[N1], y[N2];	/* nonpolarized layer vectors */
	char	ch;

	printf("\n Press any key to start recall ... ");
	getch();

	Initialize();	/* start graphics mode */

	for(k=0; k<M; k++){

	/***  display the input pattern A to Layer X  **************/

	  display_layer_xy(A[k], LY, 'A');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer Y ...");
	  getch();

	  output_from_B(A[k], y);

	  for(j=0; j<N2; j++){
	    if(y[j]>0)		LY[j] =  1;
	    else		LY[j] = -1;
	  }

	  display_layer_xy(LX, LY, 'B');

	  ShowMessage(LIGHTMAGENTA, " Press any key to recall Layer X ...");
	  getch();

	  output_from_A(x, LY);

	  for(i=0; i<N1; i++){
	    if(x[i]>0)      	LX[i] =  1;
	    else 	      	LX[i] = -1;
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
		dot_value = LX[y*XN1+x];
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
		dot_value = LY[y*XN2+x];
		if(dot_value ==  1) 	FixBoxDot(x2, y2, GREEN);
		if(dot_value == -1) 	FixBoxDot(x2, y2, BLACK);
	      }
	    }
	    break;
	}/** switch **/
}
