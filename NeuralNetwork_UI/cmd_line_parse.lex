/* cmd_line_parse.lex
 *
 * This scanner together with the parser "ex2.y" translates an
 * expression to a postfixed version.
 * How to run:
 * $ ex2
 * x = 1 + (2+3)*4;    (type in an expression)
 * ... 
 * ;      (quit by ;)
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_line_parse.tab.h"

extern void export_string(char *str);

extern char *parse_result;
char tmp_str[512];

yyerror(char* msg)
{
	/* fprintf(stderr, msg); */
	sprintf(tmp_str, "\n%s! %s", msg, "Type an expression: var = ... ;");
	export_string(tmp_str);

	/* exit(0); */
}

%}

LF			[\n]
delim     	[ \t\n]
WS        	{delim}+
alpha    	[a-zA-Z]
letter    	[a-zA-Z]
DIGIT     	[0-9]
IDENTIFIER	{letter}({letter}|{DIGIT})*
/* An identifier could be a variable for assignment, a function name,
   an unknown function name, and other command names such as plot, etc. */
alpha_dig	{alpha}|{DIGIT}
alpha_num	{alpha}|{DIGIT}|[.]

INTEGER		{DIGIT}+
/* nonnegative integers */
FLOAT		({DIGIT}*\.{DIGIT}+)|({DIGIT}+\.{DIGIT}*)
/* .12 or 0.12 are ok */

SIGN		"-"|"+"
SCN			[Ee]({SIGN}?){INTEGER}  
/* scientific notation: 1e-10 */	

FUNC1		"abs"|"fabs"|"ceil"|"floor"
FUNC2		"asin"|"acos"|"atan"|"sin"|"cos"|"tan"|"ctan"
FUNC3		"sinh"|"cosh"|"tanh"
FUNC4		"exp"|"ln"|"log"|"log10"|"sqrt" 
FUNC5		"ones"|"round"

FUNC		{FUNC1}|{FUNC2}|{FUNC3}|{FUNC4}|{FUNC5}

/* Commands for plotting */
PLOT_C		plot
OPEN_C		open
FIGURE_C	figure
CLOSE_C		close

%%

({INTEGER}{SCN}?)|({FLOAT}{SCN}?) {
		cmdlval.dval = atof(cmdtext); 
		return (NUM);
		}

"pi"	{
		cmdlval.name = strdup(cmdtext);
		return (CONST);
		}

{FUNC}	{
		/* Put this part before the {VARIABLE} part.*/
		/* printf("\n[lex] Got a function %s", cmdtext); fflush(stdout); */
		cmdlval.name = strdup(cmdtext);
		return (FUNC);
		}

{PLOT_C} {
		cmdlval.name = strdup(cmdtext);
		return (PLOT);
		}

{FIGURE_C} {
		cmdlval.name = strdup(cmdtext);
		return (FIG);
		}

{IDENTIFIER} {
		/* printf("\n[lex] Got an ID: %s ", cmdtext);  fflush(stdout); */
		/* Should put symlook() to the parser, because ID could be
		   other names such as unknown functions, plot, and others. */
		cmdlval.name = strdup(cmdtext); 
		return (ID);
		}
"+"|"-" {
		cmdlval.ch = cmdtext[0]; 
		return (ADDOP);
		}
"*"|"/" {
		cmdlval.ch = cmdtext[0]; 
		return (MULTOP);
		}
";" 	{
		cmdlval.ch = cmdtext[0]; 
		return (EOL);
		}
","     {return (',');}

"("     {return ('(');}
")"     {
		cmdlval.ch = cmdtext[0]; 
		return (')');
		}
"="     {return ('=');}
":"     {return (':');}
{WS}    {}

%%

/* symlook() looks for a given symbol name. If not found, it creates
   a new symtab structure and returns the pointer to it. */

SYMTAB* symlook(char *name)
{
  	SYMTAB *sp, **spp;

    /* Look through the symtab list */
    for (spp = &symtab; *spp; spp = &((*spp)->next))
    {
	 	/* Is the symbol name already in the symbol table? */
        if (strcmp((*spp)->name, name) == 0)
			return *spp; 
	}
    /* Now we are at the end of the symtab list. */
	sp = (SYMTAB *) malloc(sizeof(SYMTAB));
	sp->name = strdup(name);  	/* copies and creates space for name */ 
	sp->val = NULL;    		/* empty value array */ 
	sp->len = 0;      
	sp->next = NULL;
	
	/*printf("\nsymtab <%s> created", sp->name); fflush(stdout);*/

	/* Hook up the new SYMTAB at the end of SYMTAB list. */
	*spp = sp;		
	return sp;
}


/* symcheck() checks whether a given symbol name is in the list of
   symtabs. If found, it returns the pointer to it; otherwise, it
   returns NULL. */

SYMTAB*  symcheck(char *name)
{
  	SYMTAB *sp;

	if (!symtab)  	return NULL;

    /* Look through the symtab list */
    for (sp = symtab; sp; sp = sp->next)
    {
	 	/* Is the symbol name already in the symbol table? */
        if (strcmp(sp->name, name) == 0)
			return sp; 
   }
   return NULL;
}

/*
main()
{
	cmdlex();
}
*/
