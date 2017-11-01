/**	
 	GA_Util.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/***************************************************************************
File Name:  	util.c	--- original code for GA utilities ---
Date:  		6/26/95
Revised:	1/15/97
Note:  		This file has been compiled into  'util.o' 
		for linking use!     % cc -c util.c 
***************************************************************************/

#include	<stdio.h>
#include	<math.h>


/* Skip skipcount lines on output device */
void	ga_skip(skipcount)
	int	skipcount;
{
	int	j;

	for(j=1; j<=skipcount; j++)
		printf("\n");
}


/* Raise x to the y-th power */
float	ga_power(x, y)
	float	x, y;
{
	return(exp(y*log(x)));
}


/* Generate a real random number in [0.0, 1.0] */
float	ga_random()
{
	double	MAX_NUM = 2147483647.0;
	float	x;

	x = (float)(random()/MAX_NUM);
	return(x);
}


/* Simulate flipping a biased coin */
int	ga_flip(P)
	float	P;	/* the probability of head-up */
{
	if(P == 0.0)	return(0);
	else		return((ga_random()<=P));
}


/* Random integer generator in [I1, I2] */
int	ga_int_random(I1, I2)
	int 	I1, I2;
{
	int	I;

	if(I1 > I2){ I = I1; I1 = I2; I2 = I;}
	I = (int)(ga_random()*(I2-I1+1)) + I1;
	if(I > I2)	I = I2;
	return(I);
}



