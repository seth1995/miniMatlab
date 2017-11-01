/**	
 	GA_Util.h 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/**************************************************************************
File Name:  GAutil.h 		--- header for any GA program ---

Date:  6/26/95
Revised:

Note:  All interface function names are prefixed by 'ga_'!
***************************************************************************/

void	ga_skip(/*int skipcount*/);
	/* skip skipcount lines on output device */

float	ga_power(/*float x, y*/);
	/* raise x to the y-th power */

float	ga_random();
	/* generate a random number in [0.0, 1.0] */

int	ga_flip(/* float probability */);
	/* simulate a biased coin flipping with 'probability' head-up */

int	ga_int_random(/* int I1, I2 */);
	/* generate an integer randomly in [I1, I2] */

