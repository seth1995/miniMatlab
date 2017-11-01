/* 	cmd_line_parse.y
 * 	A commad line parser for OASIS GUI.
 * 	This parser is designed to have a computation capability like MATLAB.
 *
 *	(c) Guoyong Shi, Shanghai Jiao Tong University
 *	06-02-2007
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "plot.h"

#define NSYM 	65535
#define PI 		3.14159265
#define EE		2.71828183		

char tmp_str[512];

char *parse_result = NULL;
int	 len_ans = 4000;  // initial length of answer

int	 width_in_char = 90;  /* width of command window in chars */

typedef struct _symtab	SYMTAB;

/* For now, the _symtab struct is mainly for storing vector/scalar variables.*/
struct _symtab {
       char 	*name;
	   double 	*val;	/* stores a scalar or a vector */
	   int		len;
	   SYMTAB	*next;
};

SYMTAB	*symtab = NULL;	/* a list of symtabs */
SYMTAB 	*ans;		/* for expression without assignment */
SYMTAB 	*symtab_p;	/* a temporary symtab */

/* symlook() & symcheck() are defined in cmd_line_parse.lex. */
SYMTAB 	*symlook();
SYMTAB 	*symcheck();

void export_string(char *str);
void print_answer(double *val, int vlen);
DOUBLE_VECTOR eval_func(char *fname, DOUBLE_VECTOR vin);
double round(double x);

%}

/* the types in union are for token typing */
%union {
    int 		ival;
    double 	dval;
    char 		ch;
    char 		*name;
    SYMTAB 	*symb;
    DOUBLE_VECTOR 	vect;
}

/* tokens used for return(...) in the lex file */
%token 	<dval> 	NUM
%token	<name> ID FUNC CONST PLOT FIG
%token 	<ch> 	ADDOP MULTOP EOL 

/* typing the itermediate grammar entities */
/* Make all internediate variables vectors */
%type 	<vect> 	E T F G 
%type 	<vect>  VEC 

%start L

%%

	/************************************************************** 
	Grammar: 
		L -> ID = E ; L |  	(* assignment *)
			 ID ; L 	|	(* show the value of a variable *)
			 E ; L 		|	(* an expr but no assignment *)
			 P ; L 		| 	(* for plotting *)
			 FGR ; L	|	(* figure; or figure(n); *)
			 empty    		(* termination *) 
		E -> E ADDOP T 	|	(* addition/subtraction *) 
			 ADDOP E  	|	(* sign an expr *)
			 T  			(* relay *)
		T -> T MULOP G 	| 	(* multiplication/division *)
			 G				(* relay *)
		G -> F 			|	(* relay *) 
			 VEC			(* relay *)
		VEC -> F : F : F | 	(* a vector defined in triple *)
		       F : F		(* a vector defined in pair *)
		F -> NUM 	|		(* a scalar number *) 
			 CONST 	| 		(* a defined const like "pi" *)
			 ID ( E ) | 	(* an unknown function *)
			 ID 	|		(* an ID already assigned *) 
			 ( E ) 	| 		(* an expr *)
			 FUNC ( E ) 	(* a defined function *)
			 ADDOP F		(* adding a sign +/- *)
		P -> PLOT ( E ; E ) |	(* plot command *)
		FGR -> FIG (NUM) |	(* figure(n) *)
		       FIG			(* figure *)
	***************************************************************/

        /* Intermediate variables are kept in the symbol table.
		   FUNC ( E ) is added to deal with math functions.
		   NO_FUNC ( E ) is added to deal with unknown functions.
		 */

        /* Notice also that each action counts as a numeric item.   */
        /* So $1 is ID, $2 is an action, $3 is =, and $4 is E, etc. */

L :     ID '=' E {
  		int i;
		/*if ($3.len == 1) {
			printf("\n[bison] Result %s = %g", $1, $3.val[0]);  
			fflush(stdout);
		}*/

		if ($3.len > 0) {
			/* get or create a named symtab */
			symtab_p = symlook($1);
			/* assign necessary info */
			symtab_p->len = $3.len;
			symtab_p->val = (double *) malloc($3.len*sizeof(double));
			for (i = 0; i < $3.len; i++)
				symtab_p->val[i] = $3.val[i];
			/* If the expr has no name, release the memory */
			if (!$3.name)  free((double *) $3.val);

			sprintf(tmp_str, "\n%6c%s = ", ' ', $1);
			export_string(tmp_str);

			print_answer(symtab_p->val, symtab_p->len);

			/*printf("\n** %s <= %g", $1, symtab_p->val[0]); fflush(stdout);*/
		}
	} 
	EOL L
	| E  {
		int i;

		if ($1.len > 0) {
			if ($1.name) {  /* the expr has a name */
				/* such as typing "<ID>;" */
               	sprintf(tmp_str, "\n%6c%s = ", ' ', $1.name);
				export_string(tmp_str);
				print_answer($1.val, $1.len);
			} else { 	/* the expr does not have a name */
				/* assign it to the var "ans" */
				ans = symlook("ans");
				/* release the old memory */
				if (ans->val) 	free((double *) ans->val);
				ans->len = $1.len;
				/* allocate new memory */
				ans->val = (double *) malloc($1.len*sizeof(double));
				for (i = 0; i < $1.len; i++) 	ans->val[i] = $1.val[i];
				/* release the memory */
				free((double *) $1.val);

               	sprintf(tmp_str, "\n%6cans = ", ' ');
				export_string(tmp_str);
				print_answer(ans->val, ans->len);
			}
		}
	}
	EOL L
	| P {
			//printf("\n<plot> command");  fflush(stdout);
            //sprintf(tmp_str, "\nPlotting done");
            //sprintf(tmp_str, " "); export_string(tmp_str);
		}
		EOL L
		| FGR {
		}
		EOL L
        | /* empty */
        ;

E :     E ADDOP T {
  			int i, len;
		if ($1.len == 0 || $3.len == 0) {
				/*sprintf(tmp_str, "\nInvalid operand");
				export_string(tmp_str);*/
				$$.len = 0;
				$$.val = NULL;
		} else {
  			if ($2 == '+') {
				if ($1.len == $3.len) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] + $3.val[i]; 
				} else if ($1.len == 1) {
					len = $3.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[0] + $3.val[i]; 
				} else if ($3.len == 1) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] + $3.val[0]; 
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					export_string(tmp_str); 
					$$.name = NULL;
					$$.len = 0;
					$$.val = NULL;
					return;
				}
			} else {          
				if ($1.len == $3.len) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] - $3.val[i]; 
				} else if ($1.len == 1) {
					len = $3.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[0] - $3.val[i]; 
				} else if ($3.len == 1) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] - $3.val[0]; 
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					export_string(tmp_str); 
					$$.name = NULL;
					$$.len = 0;
					$$.val = NULL;
					return;
				}
			}
			$$.len = len;
		}
		$$.name = NULL;

		/* release memory if needed */
		if (!$1.name && $1.val)  free((double *) $1.val);
		if (!$3.name && $3.val)  free((double *) $3.val);
		}
        | T { 
			$$ = $1; 
			/*printf("\n---step to E"); fflush(stdout);*/
		}
        ;

T :     T MULTOP G  {
  			int i, len;
			/* check validity of operands */
		if ($1.len == 0 || $3.len == 0) {
				/*sprintf(tmp_str, "\nInvalid operand");
				export_string(tmp_str);*/
				$$.len = 0;
				$$.val = NULL;
		} else {
  			if ($2 == '*') { 
				if ($1.len == $3.len) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] * $3.val[i]; 
				} else if ($1.len == 1) {
					len = $3.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[0] * $3.val[i]; 
				} else if ($3.len == 1) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] * $3.val[0]; 
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					export_string(tmp_str); 
				}
  			} else { /* for division '/' */
				if ($1.len == $3.len) { /* vec / vec */
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						if ($3.val[i] == 0) {
							sprintf(tmp_str, "\nDivided by zero."); 
							export_string(tmp_str); 
							$$.val[i] = MAXFLOAT;  /* defined in <math.h>*/
						} else {
							$$.val[i] = $1.val[i] / $3.val[i]; 
						}
					}
				} else if ($1.len == 1) { /* scalr / vec */
					len = $3.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						if ($3.val[i] == 0) {
							sprintf(tmp_str, "\nDivided by zero."); 
							export_string(tmp_str); 
							$$.val[i] = MAXFLOAT;  /* defined in <math.h>*/
						} else {
							$$.val[i] = $1.val[0] / $3.val[i]; 
						}
					}
				} else if ($3.len == 1) { /* vec / scalar */
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					if ($3.val[0] == 0) {
						sprintf(tmp_str, "\nDivided by zero."); 
						export_string(tmp_str); 
						for (i = 0; i < len; i++) 
							$$.val[i] = MAXFLOAT;  /* defined in <math.h>*/
					} else {
						for (i = 0; i < len; i++) {
							$$.val[i] = $1.val[i] / $3.val[0]; 
						}
					}
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					export_string(tmp_str); 
				}
			}
			$$.len = len;
		}
		$$.name = NULL;

		/* release memory if needed */
		if (!$1.name && $1.val)  free((double *) $1.val);
		if (!$3.name && $3.val)  free((double *) $3.val);
		}
        | G { 	
			$$ = $1; 
			/*printf("\n---step to T"); fflush(stdout);*/
		}
		;

G:		F 	{	
 			$$ = $1; 
		}
  		| VEC { 
			$$ = $1; 
			/*printf("\n---step to G"); fflush(stdout);*/
		}
        ;

VEC :	F ':' F ':' F {
			int i;
			double x, x1, x2, dx;
			/* printf("\nProcessing pattern E : E"); fflush(stdout);*/

			x1 = $1.val[0]; dx = $3.val[0]; x2 = $5.val[0];
			/* cases to return <empty> */
			if ( (dx==0) || (dx>0 && x1>x2) || (dx<0 && x1<x2) ) { 
				$$.len = 0; 
				$$.val = NULL; 
				sprintf(tmp_str, "\nInvalid vector: %g : %g : %g", \
					$1.val[0], $3.val[0], $5.val[0]);
				export_string(tmp_str);
			} else {
				x = x1;
				/* Get the vector length */
				if (dx > 0)
					for (i = 0; x < x2+dx*0.5; x += dx, ++i);
					// +dx*0.5 to correct floating point error
				else
					for (i = 0; x > x2+dx*0.5; x += dx, ++i);
				//printf("\ni = %d; dx = %f; x = %f", i, dx, x); fflush(stdout);
				
				$$.len = i;
				$$.val = (double *)malloc(i*sizeof(double));
				for (i = 0; i < $$.len; i++)  
					$$.val[i] = x1+i*dx;
			}
			$$.name = NULL;

			/* release the memory if needed */
			if (!$1.name)  free((double *) $1.val);
			if (!$3.name)  free((double *) $3.val);
			if (!$5.name)  free((double *) $5.val);
		}
		| F ':' F {
			int i;
			double x, x1, x2, dx = 1;
			/* printf("\nProcessing pattern E : E"); fflush(stdout);*/
			x1 = $1.val[0]; x2 = $3.val[0];
			if (x1 > x2) { 
				sprintf(tmp_str, "\nInvalid vector: %g : %g ", \
					$1.val[0], $3.val[0]);
				export_string(tmp_str);
				$$.len = 0; 
				$$.val = NULL; 
				/*return;*/ 
				/* don't return directly; it could cause malfunctioning */
			} else {
				x = x1;
				for (i = 0; x <= x2; x += dx, ++i);
				/*printf("\ni = %d", i); fflush(stdout);*/
				$$.len = i;
				$$.val = (double *)malloc(i*sizeof(double));
				for (i = 0; i < $$.len; i++)  
					$$.val[i] = x1+i*dx;
			}
			$$.name = NULL;

			/* release the memory if needed */
			if (!$1.name)  free((double *) $1.val);
			if (!$3.name)  free((double *) $3.val);
		}
		;

F :     NUM {
  			/* <NUM> is a double */
			$$.name = NULL;
  			$$.val = (double *) malloc(sizeof(double));
  			$$.val[0] = $1; 
			$$.len = 1;		/* scalar */
		}
		| CONST {
			if (strcmp($1, "pi") == 0) {
				$$.name = NULL;
  				$$.val = (double *) malloc(sizeof(double));
				$$.val[0] = PI;
				$$.len = 1;		/* scalar */
			} else {
				sprintf(tmp_str, "\nUnknown constant: %s ", $1);
				export_string(tmp_str); 
			}
		}
  		| ID '(' E ')' {
			sprintf(tmp_str, "\nUnknown function name: %s ", $1);
			export_string(tmp_str); 
			$$.name = NULL;
			$$.len = 0;
			$$.val = NULL;
		}
		;
        | ID {
			symtab_p = symcheck($1);
			if (!symtab_p) {
				sprintf(tmp_str, "\nUndefined variable: %s ", $1);
				export_string(tmp_str);
				$$.name = NULL;
				$$.val = NULL;
				$$.len = 0;
			} else {
				$$.name = symtab_p->name; 
				$$.val = symtab_p->val; 
				$$.len = symtab_p->len; 
				/*printf("\n%s = %g", $1, symtab_p->vec.value); 
				fflush(stdout);*/
			}
		}
        | '(' E ')'   {
				$$ = $2;
		}
  		| FUNC '(' E ')' {
			if ($3.len == 0) {
				sprintf(tmp_str, "\nInvalid argument");
				export_string(tmp_str); 
				$$.name = NULL;
				$$.len = 0;
				$$.val = NULL;
			} else {
				$$ = eval_func($1, $3);
				/* release memory if needed */
				if (!$3.name)  free((double *) $3.val);
			}
		}
  		| ADDOP F 	{
			/* this ADDOP is treated as a SIGN (+/-) */
			int i;
			$$.val = (double *) malloc($2.len*sizeof(double));
			if ($1 == '+') { 
				for (i = 0; i < $2.len; i++) $$.val[i] =  $2.val[i];
			} else {          
				for (i = 0; i < $2.len; i++) $$.val[i] = -$2.val[i];
			}
			$$.name = NULL;
			$$.len = $2.len;

			if (!$2.name)  free((double *) $2.val);
		}
		;

P :		PLOT '(' E ',' E  ')' {
  			/*printf("\nE1 has length %d; E2 has length %d", $3.len, $5.len);
			fflush(stdout);*/
			if ($3.len > 0 && $5.len > 0 && $3.len == $5.len) {
				plot($3, $5);
				/* The plot command only plots to the latest active
				   figure window. If no figure exists, it creates one. */
			} else if ($3.len != $5.len) {
				sprintf(tmp_str, "\nTwo vectors do not have equal length.");
				export_string(tmp_str); 
			}
  		}
		;

FGR :	FIG '(' NUM ')' {
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			if ( $3 > 0 && floor($3) == $3 ) {
				request_figure($3);
				/* If the figure is not there, make a new figure;
				   otherwise, make it active. */
			} else {
				sprintf(tmp_str, "\nFigure handle must be 1, 2, ...");
				export_string(tmp_str); 
			}
  		}
		| FIG {
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			/* Start a new figure */
			new_plotting_window( NULL );
			/*	sprintf(tmp_str, "\n[bison] Figure handle must be 1, 2, ...");
				export_string(tmp_str); */
  		}
		;

%%

extern char command_line[];

#include "line_parse.yy.c"

/* Print the string segment to the <parse_result> string. */
void export_string(char *str)
{
	char *temp;
	int	expand = 0;
	
	//fprintf(stderr, "\n[%s : line %d] export_string()", __FILE__, __LINE__);

	// Initialize <parse_result>
	if (!parse_result)
		parse_result = malloc(len_ans*sizeof(char));
	
	/* Make sure <parse_result> has sufficient space. */
	while ( strlen(parse_result) + strlen(str) > len_ans ) 	{
		len_ans *= 2;	// double <len_ans>
		expand = 1;
	}

	//fprintf(stderr, "\nlength of parse_result = %d", len_ans);

	if (expand) 
		parse_result = (char *) realloc(parse_result, len_ans*sizeof(char));
	
	if (!parse_result) {
		printf("\nOut of memory.\n"); 	exit(0);
	}

	strcat(parse_result, str);
}

void print_answer(double *val, int vlen)
{
	int i, len = 0;

	if (vlen == 0)  return;

	for (i = 0; i < vlen; i++) {
		sprintf(tmp_str, "%g  ", val[i]);
		export_string(tmp_str);
		len += strlen(tmp_str);
		if (len > width_in_char) {
			//strcat(tmp_str, "\n");  
			export_string("\n");  
			len = 0;
		}
	}
}

DOUBLE_VECTOR eval_func(char *fname, DOUBLE_VECTOR vin)
{
	DOUBLE_VECTOR var;
	int    i;

	if (strcmp(fname, "abs") == 0 || strcmp(fname, "fabs") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = fabs(vin.val[i]); 
	}
	else if (strcmp(fname, "ceil") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = ceil(vin.val[i]); 
	}
	else if (strcmp(fname, "floor") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = floor(vin.val[i]); 
	}
	else if (strcmp(fname, "asin") == 0) { 
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] > 1.0 || vin.val[i] < -1.0) {
				sprintf(tmp_str, "\nOut of domain for asin().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = asin(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "acos") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] > 1.0 || vin.val[i] < -1.0) {
				sprintf(tmp_str, "\nOut of domain for asin().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = acos(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "atan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = atan(vin.val[i]); 
	}
	else if (strcmp(fname, "sin") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = sin(vin.val[i]); 
	}
	else if (strcmp(fname, "cos") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = cos(vin.val[i]); 
	}
	else if (strcmp(fname, "tan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = tan(vin.val[i]); 
	}
	else if (strcmp(fname, "ctan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] == 0) {
				sprintf(tmp_str, "\nOut of domain for ctan().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else
				var.val[i] = 1/tan(vin.val[i]);
		}
	}
	else if (strcmp(fname, "sinh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = sinh(vin.val[i]); 
	}
	else if (strcmp(fname, "cosh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = cosh(vin.val[i]); 
	}
	else if (strcmp(fname, "tanh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = tanh(vin.val[i]); 
	}
	else if (strcmp(fname, "exp") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = exp(vin.val[i]); 
	}
	else if (strcmp(fname, "ln") == 0 || strcmp(fname, "log") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for log().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = log(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "log10") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for log10().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = log10(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "sqrt") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for sqrt().");
				export_string(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = sqrt(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "ones") == 0) {
		/* If passed in a vector, it returns a vector of 1's of the same length.
		   If passed in a scalar, it returns a vector of 1's of the length
		   given by floor(fabs(scalar)). */
		int len;
		if (vin.len > 1)	
			len = vin.len;
		else 				
			len = floor(fabs(vin.val[0])); 
		if (len == 0) {
			var.name = NULL;
			var.val = NULL;
			var.len = 0;
			return var;
		}
		var.val = (double *)malloc(len*sizeof(double));
		var.len = len;
		for (i = 0; i < len; i++)  var.val[i] = 1.0;
	}
	else if (strcmp(fname, "round") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  {
			var.val[i] = round(vin.val[i]); 
			// round() is a customized function, defined in this file.
		}
	}
	else {
		sprintf(tmp_str, "\nUnknown function name: %s ", fname);
		export_string(tmp_str); 
		var.len = 0;
		var.val = NULL;
	}

	var.name = NULL;
	return  var;
}

double round(double x)
{
	double 	t, y;
	int		sign;

	if (x >= 0) 	sign = 1;
	else 			sign = -1;

	t = fabs(x);
	if (t - floor(t) < 0.5) 	
		y = sign * floor(t);
	else		
		y = sign * (floor(t)+1);
	return (y);
}
        
#ifdef NO_BISON_MAIN
main()
{
	yyparse();
}
#endif

