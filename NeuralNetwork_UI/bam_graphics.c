/**	
 	bam_graphics.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*******************************************************************
File Name:	bam_graphics.c
Created:	7/18/1997
Revised:
Note:		(1) BAM learning.
			This program create a dot matrix editor.

********************************************************************/

#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <graphics.h>


int	GraphDriver;
int	GraphMode;
double	AspectRatio;
int	MaxX, MaxY;
int	MaxColors;
int	ErrorCode;

/********* Function prototypes *********************/

void	Initialize(void);
void	QuitGraphics(void);

void    DrawSomething(void);
void	DrawCoordinates(void);
void	ShowMessage(int color, char *bottom_line);

void	EditDotMatrix(void);
void	Box(int x, int y, int color);
void	FixBox(int x, int y);


int	main()
{
	Initialize();

	/*DrawSomething();
	DrawCoordinates();*/

	ShowMessage(WHITE, " Press <E/e> to end edit ... ");

	EditDotMatrix();

	ShowMessage(CYAN, " Press any key to quit ... ");

	getch();
	QuitGraphics();
	return(0);
}

void	Initialize(void)
{
	int	xasp, yasp;

	GraphDriver = DETECT;
	initgraph(&GraphDriver, &GraphMode, "");
	ErrorCode = graphresult();
	if(ErrorCode != grOk){
	  printf("Graphics System Error: %s\n", grapherrormsg(ErrorCode));
	  exit(1);
	}
	MaxColors = getmaxcolor()+1;
	MaxX = getmaxx();
	MaxY = getmaxy();
	getaspectratio(&xasp, &yasp);
	AspectRatio = (double)xasp/(double)yasp;
	cleardevice();
	/* setfillstyle(SOLID_FILL, WHITE);
	bar(0, 0, MaxX, MaxY); */
}

void	QuitGraphics(void)
{
	cleardevice();		/* Clear the graphics screen */
	closegraph();		/* Return the system to text mode */
}

void	DrawSomething(void)
{
	int	color, height;
	int	i, j, x, y;
	int	k;

	height = textheight("H");
	/*
	setviewport(0, 0, MaxX, MaxY, 1);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle(EMPTY_FILL, 0);
	setcolor(WHITE);
	bar(0, MaxY-(height+4), MaxX, MaxY);
	*/
	setcolor(WHITE);
	rectangle(0, MaxY-(height+4), MaxX, MaxY);

	/*
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	settextjustify(LEFT_TEXT, TOP_TEXT);
	*/
	outtextxy(4, MaxY-(height+2), "Press any key ...");

	/*while(!kbhit()){*/
	i = j = 10;	x = y = 80;
	for(k=0; k<500; k+=5){
	  color = random(MaxColors-1)+1;
	  setcolor(color);
	  line(i+k, j, x, y+k);
	}
}

void    DrawCoordinates(void)
{
	int	X0 = 320, Y0 = 230, X1 = 20, X2 = 620, Y1 = 460, Y2 = 10;
	int	dx = 8, dy = 4;
	/*int	X_unit = 1, Y_unit = 100;  dots for one unit */
	int	k;			/* step index */
	double	X_max = 10.0, Y_max = 2.0; /* real valued x-y ranges */
	double	xs = 0.0, ys = 0.5;	/* the starting point */
	double	x, y, delta_x = 0.01;

	ShowMessage(WHITE, "Press any key ...");
	line(X1, Y0, X2, Y0);
	line(X2-dx, Y0-dy, X2, Y0);	/* draw arrow-X */
	line(X2-dx, Y0+dy, X2, Y0);

	line(X0, Y1, X0, Y2);
	line(X0-dy, Y2+dx, X0, Y2);	/* draw arrow-Y */
	line(X0+dy, Y2+dx, X0, Y2);

	outtextxy(X0+2, Y0+2, "O");
	outtextxy(X2+2, Y0+2, "X");
	outtextxy(X0+4, Y2-2, "Y");
	/*-------------------------------------------------*/
	/*x = xs;*/	y = ys;
	for(k=1; k<=1000; k++){
	  y = y*(1-y)*delta_x+y;
	  putpixel(X0+(int)((xs+k*delta_x)/X_max*(X2-X0)), Y0+(int)(y/Y_max*(Y2-Y0)), BLACK);
	}
}

void	ShowMessage(int color, char *bottom_line)
{
	int	height;

	height = textheight("H");
	setfillstyle(SOLID_FILL, BLACK);
	bar(0, MaxY-(height+4), MaxX, MaxY);
	setcolor(color);
	rectangle(0, MaxY-(height+4), MaxX, MaxY);
	outtextxy(10, MaxY-(height+2), bottom_line);
}

void	EditDotMatrix(void)
{
	int	XL = 100, XR = 260, YD = 260, YU = 100;
	int	x1, y1;
	int	c = 0, i;

	setcolor(WHITE);
	for (i=0; i<=16; i++) {
	  line(XL, YU+i*10, XR, YU+i*10);
	  line(XL+i*10, YU, XL+i*10, YD);
	}

	x1 = XL+5;	y1 = YU+5;
	while (c!='e' && c!='E')
	{
	  Box(x1, y1, LIGHTRED);
	  c = getch();
	  Box(x1, y1, BLACK);
	  
	  if(c==71||c==72||c==73){  y1 -= 10; } /* up */
	  if(c==71||c==75||c==79){  x1 -= 10; } /* left */
	  if(c==73||c==77||c==81){  x1 += 10; } /* right */
	  if(c==79||c==80||c==81){  y1 += 10; } /* down */
	  
	  if(x1<XL+5)	x1 = XR-5;
	  if(x1>XR-5)	x1 = XL+5;
	  if(y1<YU+5)	y1 = YD-5;
	  if(y1>YD-5)	y1 = YU+5;
	  
	  Box(x1, y1, LIGHTRED);
	  
	  if(c==32)	FixBox(x1, y1);
	}
}

void	Box(int x, int y, int color)
{
	setcolor(color);
	rectangle(x-4, y-4, x+4, y+4);
}

void	FixBox(int x, int y)
{
	int	painted;

	painted = getpixel(x, y);
	if(!painted){
	  setfillstyle(SOLID_FILL, YELLOW);
	  bar(x-4, y-4, x+4, y+4);
	}
	else{
	  setfillstyle(SOLID_FILL, BLACK);
	  bar(x-4, y-4, x+4, y+4);
	}
}

