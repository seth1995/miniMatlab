/* 	CommandLineParse.yy
 * 	A commad line parser for OASIS GUI.
 * 	This parser is designed to parse command lines like in MATLAB.
 *
 *	(c) Guoyong Shi, Shanghai Jiao Tong University
 *	06-02-2007; 2017/7/6
 */


%skeleton "lalr1.cc"
%defines
%locations

%code requires{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
class CmdLineParserUtility;

extern FILE* yyin;

//#include "plot.h"
#define MAXFLOAT 0x1.fffffep+127f
// Expression entries are arranged as a linked list.
struct _expr_entry {
	_expr_entry() { name = 0;  val = 0;  len = 0; next = 0; }
	char 	*name;
	double 	*val;	/* stores a vector of doubles (scalar if length is 1) */
	int		len;
	struct _expr_entry	*next;
};

typedef struct _expr_entry	EXPR_ENTRY;

struct _dvec {
	// _dvec() { name = 0;  val = 0;  len = 0; } // _dvec cannot have constructor if it is used in a union
	char *name;	  	/* name of the vector; NULL for unnamed vector */
	double *val;	/* A list of values */
	int len;
};

typedef struct _dvec VECTOR_t;

}

%define parser_class_name {CmdLineParser}
%output "./CmdLineParser.cpp"
%verbose

%{

#include "Macro_Defs.h"
#include "CmdLineParserUtility.h"  // Needed in addition to the previous #include ...


/*
extern int yylex(Cmd::CmdLineParser::semantic_type *yylval, \
					Cmd::CmdLineParser::location_type *yylloc);
*/					

%}

%lex-param   { CmdLineParserUtility *parserUtil }
%parse-param { CmdLineParserUtility *parserUtil }

/* the types in union are for token typing */
%union {
    int ival;
    double dval;
    char ch;
    char *name;
    struct _expr_entry *symb;
    struct _dvec vect;
}

/* tokens used for return(...) in the lex file */
%token<dval>	NUM
%token<ch>		ADDOP MULTOP EOL 
%token<name>	ID FUNC CONST STRING_S
%token<name>	PLOTOP FIGUREOP CLEAR_STR PRINT_STR ALL_STR

// Token END is used in the definition of function "terminate" in the lex file.
%token END      0 	"end of file"

/* typing the itermediate grammar entities */
/* Make all internediate variables vectors */
%type<vect> 	E T F G 
%type<vect>  	VEC 
%type<name>		token_str

%start L

%%

	/************************************************************** 
	Grammar: 
		L -> ID = E ; L |  	(* assignment *)
			 ID ; L 	|	(* show the value of a variable *)
			 E ; L 		|	(* an expr but no assignment *)
			 PLOT ; L 	| 	(* for plotting *)
			 FIGURE ; L	|	(* figure; or figure(n); *)
			 CLEAR ; L	|	(* clear all variables; *)
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
		PLOT -> PLOTOP ( E ; E ) |	(* plot command *)
		FIGURE -> FIGUREOP (NUM) |	(* figure(n) *)
		       FIGUREOP		(* figure *)
		COMMAND_ALL "clear"|"print" "all"
	***************************************************************/

        /* Intermediate variables are kept in the symbol table.
		   FUNC ( E ) is added to deal with math functions.
		   NO_FUNC ( E ) is added to deal with unknown functions.
		 */

        /* Notice also that each action counts as a numeric item.   */
        /* So $1 is ID, $2 is an action, $3 is =, and $4 is E, etc. */

L :     ID '=' E {
  		int i;
  		char tmp_str[512];
  		
#ifdef TRACE_
		if ($3.len == 1) {
			cout << LINE_INFO;
			printf("\n[bison] Result %s = %g", $1, $3.val[0]);  
			fflush(stdout);
		}
#endif

		if ($3.len > 0) {
			EXPR_ENTRY *entry;
			/* Get the entry once (also do saving) */
			entry = parserUtil->getVariable($1);
			
			entry->len = $3.len;
			entry->val = (double *) malloc($3.len*sizeof(double));
			
			for (i = 0; i < $3.len; i++)
				entry->val[i] = $3.val[i];
				
			/* If the expr has no name, then release the memory (no need to save) */
			if (!$3.name)  free((double *) $3.val);

			/*
			sprintf(tmp_str, "  %s = ", $1);
			parserUtil->dumpResultString(tmp_str);
			parserUtil->printValueList(entry->val, entry->len);
			*/
			parserUtil->dumpVarEntryToResult(entry);
		}
		else {
			sprintf(tmp_str, "  %s = no result", $1);
			parserUtil->dumpResultString(tmp_str);
			//parserUtil->dumpResultString("No result.");
		}
	} 
	EOL L
	| E  {
		int i;
		char tmp_str[512];

		if ($1.len > 0) 
		{
			if ($1.name) {  /* the expr has a name */
				/* such as typing "<ID>;" */
               	sprintf(tmp_str, "  %s = ", $1.name);
				parserUtil->dumpResultString(tmp_str);
				parserUtil->dumpValueListToResult($1.val, $1.len);
			} 
			else { 	/* the expr does not have a name */
				/* Assign it to the var "ans" */
				parserUtil->ansEntry = parserUtil->getVariable("ans");
				/* Release the old memory */
				if (parserUtil->ansEntry->val) 	
					free((double *) parserUtil->ansEntry->val);
				parserUtil->ansEntry->len = $1.len;
				/* Allocate new memory */
				parserUtil->ansEntry->val = (double *) malloc($1.len*sizeof(double));
				for (i = 0; i < $1.len; i++) 	
					parserUtil->ansEntry->val[i] = $1.val[i];
				/* Release the memory */
				free((double *) $1.val);

				/*
               	sprintf(tmp_str, "  ans = ");
				parserUtil->dumpResultString(tmp_str);
				parserUtil->printValueList(parserUtil->ansEntry->val, 
						parserUtil->ansEntry->len);
				*/
				parserUtil->dumpVarEntryToResult(parserUtil->ansEntry);
			}
		}
	}
	EOL L
	| PLOT {
			//printf("\n<plot> command");  fflush(stdout);
            //sprintf(tmp_str, "\nPlotting done");
            //sprintf(tmp_str, " "); dumpResultString(tmp_str);
	}
	EOL L
	| FIGURE {
	}
	EOL L
	| COMMAND_ALL {
	}
	EOL L
    | /* empty */
    ;

E :     E ADDOP T {
		int i, len=0;
		char tmp_str[512];
		
#ifdef TRACE_
		if ($1.len && $3.len) {
			cout << LINE_INFO;
			printf("Adding <%g> and <%g>", $1.val[0], $3.val[0]);
			printf("\n$1.len = %d and $3.len = %d", $1.len, $3.len);
		}
#endif		
		if ($1.len == 0 || $3.len == 0) {
				sprintf(tmp_str, "  Invalid operand\n");
				parserUtil->dumpResultString(tmp_str);
				$$.len = 0;
				$$.val = NULL;
		} 
		else {
  			if ($2 == '+') {
				//cout << LINE_INFO << "Adding two numbers" << endl;
				
				if ($1.len == $3.len) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						$$.val[i] = $1.val[i] + $3.val[i]; 
						/*
						cout << LINE_INFO;
						printf("i = %d; Sum = %g", i, $$.val[i]);
						*/
					}
				} else if ($1.len == 1 && $3.len > 1) {
					len = $3.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[0] + $3.val[i]; 
				} else if ($1.len > 1 && $3.len == 1) {
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						$$.val[i] = $1.val[i] + $3.val[0]; 
				} else {
					//cout << LINE_INFO << "Code is here" << endl;
					sprintf(tmp_str, "  Unmatched dimensions\n");
					parserUtil->dumpResultString(tmp_str); 
					$$.name = NULL;
					$$.len = 0;
					$$.val = NULL;
					//yyerrok;
				}
			} 
			else if ($2 == '-') {          
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
					sprintf(tmp_str, "  Unmatched dimensions\n");
					parserUtil->dumpResultString(tmp_str); 
					$$.name = NULL;
					$$.len = 0;
					$$.val = NULL;
					//yyerrok;
				}
			}
			$$.len = len;
		}
		$$.name = NULL;

		/* Release the memory if needed */
		if (!$1.name && $1.val)  free((double *) $1.val);
		if (!$3.name && $3.val)  free((double *) $3.val);
		}
        | T { 
			//cout << LINE_INFO << "Passing T --> E" << endl;
			$$ = $1; 
		}
        ;

T :     T MULTOP G  {
		int i, len=0;
		char tmp_str[512];
		/* Check the validity of the operands */
		if ($1.len == 0 || $3.len == 0) {
			cout << LINE_INFO << endl;
			sprintf(tmp_str, "Invalid operand");
			parserUtil->dumpResultString(tmp_str);
			$$.len = 0;
			$$.val = NULL;
		} 
		else {
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
					parserUtil->dumpResultString(tmp_str); 
				}
  			} 
  			else if ($2 == '/') { /* for division '/' */
#ifdef TRACE_
				cout << LINE_INFO;
				printf("Dividing <%g> and <%g>", $1.val[0], $3.val[0]);
#endif				
				if ($1.len == $3.len) { /* vec / vec */
					len = $1.len;
					$$.val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						if ($3.val[i] == 0) {
							sprintf(tmp_str, "\nDivided by zero."); 
							parserUtil->dumpResultString(tmp_str); 
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
							parserUtil->dumpResultString(tmp_str); 
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
						parserUtil->dumpResultString(tmp_str); 
						for (i = 0; i < len; i++) 
							$$.val[i] = MAXFLOAT;  /* defined in <math.h>*/
					} else {
						for (i = 0; i < len; i++) {
							$$.val[i] = $1.val[i] / $3.val[0]; 
						}
					}
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					parserUtil->dumpResultString(tmp_str); 
				}
			}
			else {
				sprintf(tmp_str, "\nUnknown mul operation"); 
				parserUtil->dumpResultString(tmp_str); 
			}
			$$.len = len;
		}
		$$.name = NULL;

		/* release memory if needed */
		if (!$1.name && $1.val)  free((double *) $1.val);
		if (!$3.name && $3.val)  free((double *) $3.val);
		}
        | G { 	
			//cout << LINE_INFO << "Passing G --> T" << endl;
			$$ = $1; 
		}
		;

G:		F 	{	
			//cout << LINE_INFO << "Passing F --> G" << endl;
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
			char tmp_str[512];
			
			/* printf("\nProcessing expression: F : F : F"); fflush(stdout);*/

			x1 = $1.val[0]; dx = $3.val[0]; x2 = $5.val[0];

			/* Ill cases */
			if ( (dx==0) || (dx>0 && x1>x2) || (dx<0 && x1<x2) ) { 
				$$.len = 0; 
				$$.val = NULL; 
				sprintf(tmp_str, "  Invalid vector: %g : %g : %g \n", \
					$1.val[0], $3.val[0], $5.val[0]);
				parserUtil->dumpResultString(tmp_str);
			} 
			else {
				x = x1;
				/* Count the vector length */
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

			/* Release the memory if needed */
			if (!$1.name)  free((double *) $1.val);
			if (!$3.name)  free((double *) $3.val);
			if (!$5.name)  free((double *) $5.val);
		}
		| F ':' F {
			int i;
			double x, x1, x2, dx = 1;
			char tmp_str[512];
			/* printf("\nProcessing pattern E : E"); fflush(stdout);*/
			x1 = $1.val[0]; x2 = $3.val[0];
			if (x1 > x2) { 
				sprintf(tmp_str, "  Invalid vector: %g : %g \n", \
					$1.val[0], $3.val[0]);
				parserUtil->dumpResultString(tmp_str);
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
#ifdef TRACE_
			cout << LINE_INFO;
			printf("Parsed a number <%g>", $1);
#endif
		}
		| CONST {
			char tmp_str[512];
			if (strcmp($1, "pi") == 0) {
				$$.name = NULL;
  				$$.val = (double *) malloc(sizeof(double));
				$$.val[0] = PI;
				$$.len = 1;		/* scalar */
			} else {
				sprintf(tmp_str, "\nUnknown constant: %s ", $1);
				parserUtil->dumpResultString(tmp_str); 
			}
		}
  		| ID '(' E ')' {
  			char tmp_str[512];
			sprintf(tmp_str, "\nUnknown function name: %s ", $1);
			parserUtil->dumpResultString(tmp_str); 
			$$.name = NULL;
			$$.len = 0;
			$$.val = NULL;
		}
		;
        | ID {
        	char tmp_str[512];
#ifdef TRACE_
        	cout << LINE_INFO;
        	printf("Parsed var \"%s\"", $1);
#endif
			EXPR_ENTRY *expr_entry = parserUtil->lookupVariable($1);
			if (! expr_entry) {
				sprintf(tmp_str, "  Undefined variable: %s\n", $1);
				parserUtil->dumpResultString(tmp_str);
				//printf("\nUndefined variable: %s\n", $1);
				$$.name = NULL;
				$$.val = NULL;
				$$.len = 0;
			} 
			else {
#ifdef TRACE_
				cout << LINE_INFO;
				printf("Found variable %s = %g", expr_entry->name,
						expr_entry->val[0]);
#endif
				$$.name = expr_entry->name; 
				$$.val = expr_entry->val; 
				$$.len = expr_entry->len; 
			}
		}
        | '(' E ')'   {
        	/*
        	cout << LINE_INFO << "E = " << $2.val[0] << endl;
        	*/
			$$ = $2;
		}
  		| FUNC '(' E ')' {
  			char tmp_str[512];
			if ($3.len == 0) {
				sprintf(tmp_str, "\nInvalid argument");
				parserUtil->dumpResultString(tmp_str); 
				$$.name = NULL;
				$$.len = 0;
				$$.val = NULL;
			} else {
				$$ = parserUtil->evaluateFunction($1, $3);
				/* Release memory if needed */
				if (!$3.name)  free((double *) $3.val);
			}
		}
  		| ADDOP F  {
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

PLOT :		PLOTOP '(' E ',' E  ')' {
			char tmp_str[512];
  			/*printf("\nE1 has length %d; E2 has length %d", $3.len, $5.len);
			fflush(stdout);*/
			if ($3.len > 0 && $3.len == $5.len) {
				parserUtil->plotVector2D($3, $5);
				/* The plot command only plots to the latest active
				   figure window. If no figure exists, it creates one. */
			} 
			else if ($3.len != $5.len) {
				sprintf(tmp_str, "  The two vectors do not have equal length.\n");
				parserUtil->dumpResultString(tmp_str); 
			}
  		}
		;

FIGURE :	FIGUREOP '(' NUM ')' {
			char tmp_str[512];
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			if ( $3 > 0 && floor($3) == $3 ) {
				parserUtil->request_figure($3);
				/* If the figure is not there, make a new figure;
				   otherwise, make it active. */
			} 
			else {
				sprintf(tmp_str, "\nFigure handle must be 1, 2, ...");
				parserUtil->dumpResultString(tmp_str); 
			}
  		}
		| FIGUREOP {
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			/* Start a new figure */
			parserUtil->new_plotting_window( NULL );
			/*	sprintf(tmp_str, "\n[bison] Figure handle must be 1, 2, ...");
				dumpResultString(tmp_str); */
  		}
		;

COMMAND_ALL:	token_str token_str {  /* command "clear/print all" */
			if (strcmp($1, "clear") == 0 && strcmp($2, "all") == 0)
				parserUtil->clearAllVariables();
			else if (strcmp($1, "print") == 0 && strcmp($2, "all") == 0)
				parserUtil->dumpAllVariables();
		}
		;

token_str: STRING_S {
			$$ = strdup($1);
		}
		;
		
%%


/*
	User defined (overloading) error messaging.
	This function is called when the parser encounters error.
*/
void Cmd::CmdLineParser::error(const Cmd::CmdLineParser::location_type &loc, 
								const std::string &msg)
{
	stringstream ss;
	ss << loc << ": " << msg << endl;
	cout << LINE_INFO << "\n\t" << ss.str() << endl;

	parserUtil->dumpResultString(ss.str().c_str());

	//parserUtil->printParseError(loc, msg);
}


//#include "CmdLineParserUtility.h"


#ifdef NO_BISON_MAIN

main()
{
	yyparse();
}
#endif

